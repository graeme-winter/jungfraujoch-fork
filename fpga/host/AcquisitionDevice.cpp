// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef JFJOCH_USE_NUMA
#include <numaif.h>
#endif

#include <sys/mman.h>
#include <thread>
#include <bitset>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iomanip>

#include "../../common/JFJochException.h"
#include "AcquisitionDevice.h"


void *mmap_acquisition_buffer(size_t size, int16_t numa_node) {
    void *ret = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ret == nullptr) {
        throw JFJochException(JFJochExceptionCategory::MemAllocFailed, "frame_buffer");
    }
#ifdef JFJOCH_USE_NUMA
    if (numa_node >= 0) {
        unsigned long nodemask = 1L << numa_node;;
        if (numa_node > sizeof(nodemask)*8)
            throw JFJochException(JFJochExceptionCategory::MemAllocFailed, "Mask too small for NUMA node");
        if (mbind(ret, size, MPOL_BIND, &nodemask, sizeof(nodemask)*8, MPOL_MF_STRICT) == -1)
            throw JFJochException(JFJochExceptionCategory::MemAllocFailed, "Cannot apply NUMA policy");
    }
#endif
    memset(ret, 0, size);
    return ret;
}

inline uint16_t to_fixed(double multiplier, double val, uint16_t fractional_bits) {
    if (val == 0)
        return 0;

    // If val is result of division by zero, only reasonable value of output is zero (otherwise number could be interpreted improperly)
    uint32_t int_val = std::lround((multiplier/val) * (1<<fractional_bits));

    // It is unlikely (but not impossible), that gain value will be lower than the smallest possible
    // Then reciprocal of gain could be more than allowed by data format. Protection is added for this condition
    if (int_val > UINT16_MAX) int_val = UINT16_MAX;
    return int_val;
}

AcquisitionDevice::AcquisitionDevice(uint16_t in_data_stream) :
buffer_err(RAW_MODULE_SIZE) {
    logger = nullptr;
    data_stream = in_data_stream;
}

uint16_t AcquisitionDevice::GetPacketCount(size_t frame, uint8_t module) const {
    return counters.GetPacketCount(frame, module);
}

uint16_t AcquisitionDevice::GetPacketCountHalfModule(size_t frame, uint8_t module) const {
    return counters.GetPacketCountHalfModule(frame, module);
}

bool AcquisitionDevice::GetTriggerField(size_t frame, uint8_t module) const {
    return counters.GetTriggerField(frame, module);
}

uint16_t AcquisitionDevice::GetBufferHandle(size_t frame, uint8_t module) const {
    return counters.GetBufferHandle(frame, module);
}

void AcquisitionDevice::FillActionRegister(const DiffractionExperiment& x, ActionConfig &job) {

    job.fpga_ipv4_addr      = x.GetDestIPv4Address(data_stream);
    job.nmodules            = x.GetModulesNum(data_stream);

    // Taken from DiffrationExperiment class
    job.frames_per_trigger = x.GetFrameNumPerTrigger();

    // Convert floats to fixed point numbers
    job.one_over_energy = std::lround((1<<20)/ x.GetPhotonEnergy_keV());

    uint64_t mode = 0;

    switch (x.GetDetectorMode()) {
        case DetectorMode::Conversion:
            mode |= MODE_CONV;
            break;
        case DetectorMode::Raw:
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            mode |= MODE_RAW;
            break;
    }

    if (x.IsUsingInternalPacketGen())
        mode |= MODE_INTERNAL_PACKET_GEN;

    job.mode = mode;
}

void AcquisitionDevice::PrepareAction(const DiffractionExperiment &experiment) {
    // Just as precaution, abort action, if it is running (ActionAbort will internally check for !HW_IsIdle() condition)
    if (!HW_IsIdle())
        throw(JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Hardware action running prior to start of data acquisition"));

    if (experiment.GetModulesNum(data_stream) > HW_GetMaxModuleNum())
        throw(JFJochException(JFJochExceptionCategory::InputParameterAboveMax,
                              "Number of modules exceeds max possible for FPGA"));

    counters.Reset(experiment, data_stream);
}

void AcquisitionDevice::StartAction(const DiffractionExperiment &experiment) {
    // Just as precaution, abort action, if it is running (ActionAbort will internally check for !HW_IsIdle() condition)
    if (!HW_IsIdle())
        throw(JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Hardware action running prior to start of data acquisition"));

    if (experiment.GetModulesNum(data_stream) > HW_GetMaxModuleNum())
        throw(JFJochException(JFJochExceptionCategory::InputParameterAboveMax,
                              "Number of modules exceeds max possible for FPGA"));

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (experiment.GetDetectorMode() == DetectorMode::Conversion)
            buffer_err[i] = PIXEL_OUT_LOST;
        else
            buffer_err[i] = -1;
    }

    // Can already start loading work requests
    read_work_completion_future = std::async(std::launch::async, &AcquisitionDevice::ReadWorkCompletionThread, this);
    send_work_request_future = std::async(std::launch::async, &AcquisitionDevice::SendWorkRequestThread, this);
    for (uint32_t i = 0; i < buffer_c2h.size(); i++)
        SendWorkRequest(i);

    counters.Reset(experiment, data_stream);
    counters.Reset(experiment, data_stream);

    ActionConfig cfg_in{}, cfg_out{};

    FillActionRegister(experiment, cfg_in);
    HW_WriteActionRegister(&cfg_in);
    HW_ReadActionRegister(&cfg_out);

    if (cfg_out.mode != cfg_in.mode)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (mode)");
    if (cfg_out.fpga_ipv4_addr != cfg_in.fpga_ipv4_addr)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (FPGA IPv4 Addr)");
    if (cfg_out.frames_per_trigger != cfg_in.frames_per_trigger)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (Frames per trigger)");
    if (cfg_out.nmodules != cfg_in.nmodules)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (#modules)");

    HW_StartAction();

    start_time = std::chrono::system_clock::now();
    filter = AcquisitionDeviceFilter(experiment);
}

int64_t AcquisitionDevice::CalculateDelay(size_t curr_frame, uint16_t module) const {
    return counters.CalculateDelay(curr_frame, module);
}

void AcquisitionDevice::WaitForFrame(size_t curr_frame, uint16_t module) const {
    counters.WaitForFrame(curr_frame, module);
}

void AcquisitionDevice::SetDefaultInternalGeneratorFrame(size_t buf_h2c_number) {
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        buffer_h2c[buf_h2c_number][i] = i % 65536;
}

void AcquisitionDevice::SetCustomInternalGeneratorFrame(const void *input, size_t input_size) {
    for (int m = 0; m < std::min<uint32_t>(input_size / FPGA_BUFFER_LOCATION_SIZE,
                                           HW_GetInternalPacketGeneratorModuleNum()) ; m++) {
            memcpy(buffer_h2c[m + HW_GetMaxModuleNum() * 6],
                   (uint8_t *) input + m * FPGA_BUFFER_LOCATION_SIZE,
                   FPGA_BUFFER_LOCATION_SIZE);
    }
}

void AcquisitionDevice::ReadWorkCompletionThread() {
    Completion c;
    do {
        c = ReadCompletion();
        work_completion_queue.PutBlocking(c);
    } while (c.frame_number != Completion::MeasurementDone);
}

void AcquisitionDevice::ReadMailbox(uint32_t *values) {
    while (!HW_ReadMailbox(values)) {
        //  The receive FIFO level is less than or equal to the RIT threshold
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

Completion AcquisitionDevice::ReadCompletion() {
    Completion c{};

    uint32_t tmp[4];

    ReadMailbox(tmp);

    if (tmp[0] == UINT32_MAX) {
        packets_ok = tmp[1];
        c.frame_number = Completion::MeasurementDone;
        return c;
    }

    c.handle       = tmp[0];
    c.module       = tmp[1] & 0xFF;
    bool all_packets_ok = tmp[1] & (1 << 8);

    uint64_t detector_frame_number = bit_concat(tmp[2], tmp[3]);

    uint32_t parity = (std::bitset<32>(tmp[0]).count() + std::bitset<32>(tmp[1]).count()
            + std::bitset<32>(tmp[2]).count() + std::bitset<32>(tmp[3]).count()) % 2;

    if (parity == 1)
        throw JFJochException(JFJochExceptionCategory::HardwareParityError, "Wrong parity in work completion");

    // Read debug, timestamp, and bunch ID
    ReadMailbox(tmp);

    c.trigger      = tmp[0] & (1 << 31u);
    c.frame_number = detector_frame_number;

    if (all_packets_ok) {
        // All packets arrived, no more messages in completion coming
        c.packet_mask[0] = UINT64_MAX;
        c.packet_mask[1] = UINT64_MAX;
    } else {
        ReadMailbox(tmp);
        c.packet_mask[0] = bit_concat(tmp[2], tmp[3]);
        c.packet_mask[1] = bit_concat(tmp[0], tmp[1]);
    }

    return c;
}

void AcquisitionDevice::WaitForActionComplete() {
    auto c = work_completion_queue.GetBlocking();

    while (c.frame_number != Completion::MeasurementDone) {
        c.frame_number = filter.ProcessCompletion(c.frame_number, c.trigger);

        if (c.frame_number == Completion::FrameAfterFilterEnd) {
            HW_SetCancelDataCollectionBit();
        }

        if ((c.frame_number != Completion::FrameAfterFilterEnd) && (c.frame_number != Completion::FrameIgnore))
            counters.UpdateCounters(&c);
        else {
            // this frame is not of any interest, therefore its location can be immediately released
            SendWorkRequest(c.handle);
            c.handle = UINT32_MAX;
        }

        if (logger != nullptr)
            logger->Debug("Data stream " + std::to_string(data_stream)
                          + " completion frame number " + std::to_string(c.frame_number)
                          + " module " + std::to_string(c.module)
                          + " handle " + std::to_string(c.handle)
                          + " trigger value " + std::to_string(c.trigger));

        c = work_completion_queue.GetBlocking();
    }

    read_work_completion_future.get();
    counters.SetAcquisitionFinished();

    end_time = std::chrono::system_clock::now();

    EndWorkRequestAndSignalQueues();

    while (!HW_IsIdle())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void AcquisitionDevice::EndWorkRequestAndSignalQueues() {
    HW_SetCancelDataCollectionBit();
    work_request_queue.Put(UINT32_MAX);
    send_work_request_future.get();
}

void AcquisitionDevice::SendWorkRequest(uint32_t handle) {
    work_request_queue.Put(handle);
}

uint32_t AcquisitionDevice::GetPacketsOK() const {
    return packets_ok;
}

void AcquisitionDevice::SaveStatistics(const DiffractionExperiment &experiment,
                                       JFJochProtoBuf::AcquisitionDeviceStatistics &statistics) const {
    size_t expected_images = experiment.GetImageNum();
    if ((experiment.GetDetectorMode() == DetectorMode::PedestalG0) ||
        (experiment.GetDetectorMode() == DetectorMode::PedestalG1) ||
        (experiment.GetDetectorMode() == DetectorMode::PedestalG2))
        expected_images = experiment.GetFrameNum();

    uint32_t expected_packets = experiment.GetModulesNum(data_stream) * expected_images * experiment.GetSummation() * 128;

    auto summation = experiment.GetSummation();
    statistics.set_packets_expected_per_image(experiment.GetModulesNum(data_stream) * summation * 128L);

    uint32_t total_packets_received = 0;

    for (int image = 0; image < expected_images; image++) {
        uint32_t packets_received = 0;
        for (int frame = 0; frame < summation; frame++) {
            for (int module = 0; module < experiment.GetModulesNum(data_stream); module++)
                packets_received += GetPacketCount(image * summation + frame, module);
        }
        statistics.add_packets_received_per_image(packets_received);
        total_packets_received += packets_received;
    }

    statistics.set_good_packets(total_packets_received);
    statistics.set_packets_expected(expected_packets);

    if (total_packets_received == expected_packets)
        statistics.set_efficiency(1.0);
    else
        statistics.set_efficiency(total_packets_received / static_cast<double>(expected_packets));

    statistics.set_ok_eth_packets(packets_ok);

    statistics.set_start_timestamp(start_time.time_since_epoch().count());
    statistics.set_end_timestamp(end_time.time_since_epoch().count());

    auto tmp_v = filter.TriggerSequenceFrameNumbers();
    if (!tmp_v.empty())
        *statistics.mutable_trigger_sequence_frame_numbers() = {tmp_v.begin(), tmp_v.end()};

    auto tmp_v2 = counters.PacketMaskHalfModule();
    if (!tmp_v2.empty())
        *statistics.mutable_packet_mask_half_module() = {tmp_v2.begin(), tmp_v2.end()};

    *statistics.mutable_fpga_status() = GetStatus();
}

uint64_t AcquisitionDevice::GetHead(uint8_t module) const {
    return counters.GetHead(module);
}

uint64_t AcquisitionDevice::GetSlowestHead() const {
    return counters.GetSlowestHead();
}

bool AcquisitionDevice::IsDone() const {
    return counters.GetAcqusitionFinished();
}

void AcquisitionDevice::ActionAbort() {
    HW_SetCancelDataCollectionBit();
}

uint64_t AcquisitionDevice::GetPacketMaskHalfModule(size_t frame, uint8_t module) const {
    return counters.GetPacketMaskHalfModule(frame, module);
}

const int16_t *AcquisitionDevice::GetPacketBuffer(size_t frame_number, uint16_t module, uint16_t packet) {
    if (packet > 127)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Wrong packet number");

    auto handle = GetBufferHandle(frame_number, module);

    if (counters.IsPacketCollected(frame_number, module, packet) &&
        (handle != HandleNotValid))
        return buffer_c2h.at(handle) + packet * RAW_MODULE_COLS * 4;
    else
        return buffer_err.data();
}

const int16_t *AcquisitionDevice::GetFrameBuffer(size_t frame_number, uint16_t module) const {
    auto handle = GetBufferHandle(frame_number, module);
    if (handle != HandleNotValid)
        return buffer_c2h.at(handle);
    else
        return buffer_err.data();
}

void AcquisitionDevice::InitializeCalibration(const DiffractionExperiment &experiment, const JungfrauCalibration &calib) {
    auto offset = RAW_MODULE_SIZE * experiment.GetFirstModuleOfDataStream(data_stream);
    auto nm = HW_GetMaxModuleNum();

    for (int m = 0; m < experiment.GetModulesNum(data_stream); m++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++) {
            buffer_h2c[m + 5 * nm][i] = calib.Pedestal(0)[offset + m * RAW_MODULE_SIZE + i];
            buffer_h2c[m + 3 * nm][i] = calib.Pedestal(1)[offset + m * RAW_MODULE_SIZE + i];
            buffer_h2c[m + 4 * nm][i] = calib.Pedestal(2)[offset + m * RAW_MODULE_SIZE + i];
        }
    }
}

void AcquisitionDevice::MapBuffersStandard(size_t c2h_buffer_count, size_t h2c_buffer_count, int16_t numa_node) {
    try {
        for (int i = 0; i < c2h_buffer_count; i++)
            buffer_c2h.emplace_back((int16_t *) mmap_acquisition_buffer(FPGA_BUFFER_LOCATION_SIZE, numa_node));

        for (int i = 0; i < h2c_buffer_count; i++)
            buffer_h2c.emplace_back((uint16_t *) mmap_acquisition_buffer(FPGA_BUFFER_LOCATION_SIZE, numa_node));
    } catch (const JFJochException &e) {
        UnmapBuffers();
    }
}

void AcquisitionDevice::UnmapBuffers() {
     for (auto &i: buffer_c2h)
        if (i != nullptr) munmap(i, FPGA_BUFFER_LOCATION_SIZE);
    for (auto &i: buffer_h2c)
        if (i != nullptr) munmap(i, FPGA_BUFFER_LOCATION_SIZE);
}

template <class T>
void AcquisitionDevice::LoadModuleGain(const std::vector<T> &vector, uint16_t module) {
    auto nm = HW_GetMaxModuleNum();

    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        buffer_h2c[module + 0 * nm][i] = to_fixed(GAIN_G0_MULTIPLIER, vector[i],14);

    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        buffer_h2c[module + 1 * nm][i] = to_fixed(GAIN_G1_MULTIPLIER, vector[i + RAW_MODULE_SIZE], 12);

    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        buffer_h2c[module + 2 * nm][i] = to_fixed(GAIN_G2_MULTIPLIER, vector[i + 2 * RAW_MODULE_SIZE], 10);
}

void AcquisitionDevice::SendWorkRequestThread() {
    auto handle = work_request_queue.GetBlocking();
    while (handle != UINT32_MAX) {
        // Preferably use the smallest handle (to reduce buffer size for better TLB usage)
        // So if work request cannot be sent, return handle and check again for the smallest one
        if (!HW_SendWorkRequest(handle)) {
            work_request_queue.Put(handle);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        handle = work_request_queue.GetBlocking();
    }
    while (!HW_SendWorkRequest(handle))
        std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void AcquisitionDevice::LoadModuleGain(const std::string &filename, uint16_t module) {
    std::fstream file(filename.c_str(), std::fstream::in | std::fstream::binary);
    if (!file.is_open())
        throw JFJochException(JFJochExceptionCategory::GainFileOpenError, "Gain file cannot be opened");
    std::vector<double> gain_input(RAW_MODULE_SIZE*3);
    file.read((char *) gain_input.data(), gain_input.size() * sizeof(double));
    LoadModuleGain(gain_input, module);
}

void AcquisitionDevice::FrameBufferRelease(size_t frame_number, uint16_t module) {
    auto handle = counters.GetBufferHandleAndClear(frame_number, module);
    if (handle != AcquisitionDeviceCounters::HandleNotFound)
        work_request_queue.Put(handle);
}

void AcquisitionDevice::EnableLogging(Logger *in_logger) {
    logger = in_logger;
}

uint32_t AcquisitionDevice::GetInternalPacketGeneratorModuleNum() {
    return HW_GetInternalPacketGeneratorModuleNum();
}

inline JFJochProtoBuf::FPGAFIFOStatus FIFO_check(uint32_t fifo_register, uint16_t pos_empty, uint16_t pos_full) {
    if (std::bitset<32>(fifo_register).test(pos_empty))
        return JFJochProtoBuf::FPGAFIFOStatus::EMPTY;
    if (std::bitset<32>(fifo_register).test(pos_full))
        return JFJochProtoBuf::FPGAFIFOStatus::FULL;
    return JFJochProtoBuf::FPGAFIFOStatus::PARTIAL;
}

JFJochProtoBuf::FPGAStatus AcquisitionDevice::GetStatus() const {
    ActionStatus status{};
    ActionEnvParams env{};

    HW_GetStatus(&status);
    HW_GetEnvParams(&env);

    JFJochProtoBuf::FPGAStatus ret;
    auto full_status_register = status.ctrl_reg;
    ret.set_full_status_register(full_status_register);

    ret.set_stalls_hbm(status.pipeline_stalls_hbm);
    ret.set_stalls_host(status.pipeline_stalls_host);

    ret.set_qsfp_module_present(full_status_register & CTRL_REGISTER_QSFP_MOD_PRESENT);
    ret.set_ethernet_rx_aligned(full_status_register & CTRL_REGISTER_100G_ALIGNED);
    ret.set_ethernet_bad_fcs(full_status_register & CTRL_REGISTER_100G_BAD_FCS);
    ret.set_max_modules(status.max_modules);
    ret.set_hbm_temp(status.hbm_temperature);
    ret.set_max_hbm_temp(status.hbm_max_temperature);
    ret.set_git_sha1(status.git_sha1);

    (*ret.mutable_fifo_status())["UDP"] = FIFO_check(status.fifo_status, 6, 7);
    (*ret.mutable_fifo_status())["Work Request"] = FIFO_check(status.fifo_status, 12, 13);
    (*ret.mutable_fifo_status())["Work Completion"] = FIFO_check(status.fifo_status, 14, 15);
    (*ret.mutable_fifo_status())["Host mem (data)"] = FIFO_check(status.fifo_status, 8, 9);
    (*ret.mutable_fifo_status())["Host mem (cmd)"] = FIFO_check(status.fifo_status, 10, 11);
    (*ret.mutable_fifo_status())["Data FIFO #8"] = FIFO_check(status.fifo_status, 16, 17);
    (*ret.mutable_fifo_status())["Addr FIFO #3"] = FIFO_check(status.fifo_status, 18, 19);
    (*ret.mutable_fifo_status())["Pedestal->gain (data)"] = FIFO_check(status.fifo_status, 0, 1);
    (*ret.mutable_fifo_status())["Pedestal->gain (cmd)"] = FIFO_check(status.fifo_status, 2, 3);

    ret.set_fpga_idle(HW_IsIdle());

    ret.set_packets_ether(status.packets_eth);
    ret.set_packets_udp(status.packets_udp);
    ret.set_packets_icmp(status.packets_icmp);
    ret.set_packets_jfjoch(status.packets_processed);

    ret.set_datamover_mm2s_error(full_status_register & (1 << 10));
    ret.set_datamover_s2mm_error(full_status_register & (1 << 11));

    ret.set_frame_statistics_alignment_err(full_status_register & (1 << 24));
    ret.set_frame_statistics_tlast_err(full_status_register & (1 << 25));
    ret.set_frame_statistics_work_req_err(full_status_register & (1 << 26));

    ret.set_mailbox_status_reg(env.mailbox_status_reg);
    ret.set_mailbox_err_reg(env.mailbox_err_reg);

    ret.set_fpga_temp_degc(static_cast<double>(env.fpga_temp >> 16)/16.0 - 273.15);

    ret.set_current_edge_12v_a(static_cast<double>(env.fpga_status_rail_12V >> 16) / 4096.0);
    ret.set_voltage_edge_12v_v(static_cast<double>(env.fpga_status_rail_12V & 0xFFFF) / 4096.0);

    ret.set_current_edge_3p3v_a(static_cast<double>(env.fpga_status_rail_3p3V >> 16) / 4096.0);
    ret.set_voltage_edge_3p3v_v(static_cast<double>(env.fpga_status_rail_3p3V & 0xFFFF) / 4096.0);

    ret.set_slowest_head(GetSlowestHead());
    return ret;
}

std::string AcquisitionDevice::GetMACAddress() const {
    uint64_t mac_address_network_order = HW_GetMACAddress();
    std::ostringstream ss;
    for (int i = 0; i < 6; i++) {
        ss << std::setfill('0') << std::setw(2) << std::hex << ((mac_address_network_order >> (i * 8)) & 0xFF);
        if (i != 5) ss << ":";
    }
    return ss.str();
}

ActionConfig AcquisitionDevice::ReadActionRegister() {
    ActionConfig cfg{};
    HW_ReadActionRegister(&cfg);
    return cfg;
}
