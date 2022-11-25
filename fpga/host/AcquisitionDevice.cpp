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
buffer_err(RAW_MODULE_SIZE), internal_pkt_gen_frame(RAW_MODULE_SIZE) {
    logger = nullptr;
    data_stream = in_data_stream;

    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        internal_pkt_gen_frame[i] = i % 65536;
}

bool AcquisitionDevice::IsFullModuleCollected(size_t frame, uint8_t module) const {
    return counters.IsFullModuleCollected(frame, module);
}

uint64_t AcquisitionDevice::GetBufferHandle(size_t frame, uint8_t module) const {
    return counters.GetBufferHandle(frame, module);
}

void AcquisitionDevice::FillActionRegister(const DiffractionExperiment& x, ActionConfig &job) {

    job.fpga_ipv4_addr             = x.GetDestIPv4Address(data_stream);
    job.nmodules                   = x.GetModulesNum(data_stream);
    job.frames_internal_packet_gen = x.GetFrameNum();
    job.one_over_energy            = std::lround((1<<20)/ x.GetPhotonEnergy_keV());
    job.nstorage_cells             = x.GetStorageCellNumber() - 1;
    job.mode                       = 0;

    if ((x.GetDetectorMode() == DetectorMode::Conversion) && (x.GetDetectorType() == JFJochProtoBuf::JUNGFRAU))
        job.mode |= MODE_CONV;

    if (x.IsUsingInternalPacketGen()) {
        job.mode |= MODE_INTERNAL_PACKET_GEN;
        if (x.GetDetectorType() == JFJochProtoBuf::EIGER)
            job.mode |= MODE_INTERNAL_PACKET_GEN_4KB;
    }
}

void AcquisitionDevice::PrepareAction(const DiffractionExperiment &experiment) {
    if (!HW_IsIdle())
        throw(JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Hardware action running prior to start of data acquisition"));

    if (experiment.GetModulesNum(data_stream) > max_modules)
        throw(JFJochException(JFJochExceptionCategory::InputParameterAboveMax,
                              "Number of modules exceeds max possible for FPGA"));

    counters.Reset(experiment, data_stream);
}

void AcquisitionDevice::StartAction(const DiffractionExperiment &experiment) {
    if (!HW_IsIdle())
        throw(JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Hardware action running prior to start of data acquisition"));

    if (experiment.GetModulesNum(data_stream) > max_modules)
        throw(JFJochException(JFJochExceptionCategory::InputParameterAboveMax,
                              "Number of modules exceeds max possible for FPGA"));

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (experiment.GetDetectorMode() == DetectorMode::Conversion)
            buffer_err[i] = PIXEL_OUT_LOST;
        else
            buffer_err[i] = -1;
    }

    counters.Reset(experiment, data_stream);
    completion_vector.Reset(experiment, data_stream);
    expected_frames = experiment.GetFrameNum();

    ActionConfig cfg_in{}, cfg_out{};

    FillActionRegister(experiment, cfg_in);
    HW_WriteActionRegister(&cfg_in);
    HW_ReadActionRegister(&cfg_out);

    if (experiment.IsUsingInternalPacketGen())
        CopyInternalPacketGenFrameToDeviceBuffer();

    if (cfg_out.mode != cfg_in.mode)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (mode)");
    if (cfg_out.fpga_ipv4_addr != cfg_in.fpga_ipv4_addr)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (FPGA IPv4 Addr)");
    if (cfg_out.frames_internal_packet_gen != cfg_in.frames_internal_packet_gen)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (Frames per trigger)");
    if (cfg_out.nmodules != cfg_in.nmodules)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Mismatch between expected and actual values of configuration registers (#modules)");

    // Ensure internal queues are empty
    work_request_queue.Clear();
    work_completion_queue.Clear();

    HW_StartAction();

    send_work_request_future = std::async(std::launch::async, &AcquisitionDevice::SendWorkRequestThread, this);
    read_work_completion_future = std::async(std::launch::async, &AcquisitionDevice::ReadWorkCompletionThread, this);

    for (uint32_t i = 0; i < buffer_device.size(); i++)
        SendWorkRequest(i);

    start_time = std::chrono::system_clock::now();
}

void AcquisitionDevice::CopyInternalPacketGenFrameToDeviceBuffer() {
    memcpy(buffer_device[0], internal_pkt_gen_frame.data(),
           RAW_MODULE_SIZE * sizeof(uint16_t));
}

int64_t AcquisitionDevice::CalculateDelay(size_t curr_frame, uint16_t module) const {
    return counters.CalculateDelay(curr_frame, module);
}

void AcquisitionDevice::WaitForFrame(size_t curr_frame, uint16_t module) const {
    counters.WaitForFrame(curr_frame, module);
}

void AcquisitionDevice::SetCustomInternalGeneratorFrame(const std::vector<uint16_t> &v) {
    if (v.size() != RAW_MODULE_SIZE)
        throw JFJochException(JFJochExceptionCategory::OpenCAPIError,
                              "Error in size of custom internal generator frame");
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        internal_pkt_gen_frame[i] = v[i];
}

const std::vector<uint16_t> &AcquisitionDevice::GetInternalGeneratorFrame() const {
    return internal_pkt_gen_frame;
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
        bytes_received = tmp[1] * 4096LU;
        c.frame_number = Completion::MeasurementDone;
        return c;
    }

    c.handle       = tmp[0];
    c.module       = tmp[1] & 0xFF;
    c.packet_count = (tmp[1] & (0xFFFF0000)) >> 16;
    bool all_packets_ok = tmp[1] & (1 << 8);

    uint64_t detector_frame_number = bit_concat(tmp[2], tmp[3]);

    uint32_t parity = (std::bitset<32>(tmp[0]).count() + std::bitset<32>(tmp[1]).count()
            + std::bitset<32>(tmp[2]).count() + std::bitset<32>(tmp[3]).count()) % 2;

    if (parity == 1)
        throw JFJochException(JFJochExceptionCategory::HardwareParityError, "Wrong parity in work completion");

    // Read debug, timestamp, and bunch ID
    ReadMailbox(tmp);

    if (detector_frame_number == 0)
        throw JFJochException(JFJochExceptionCategory::HardwareParityError, "Detector frame number cannot be zero");
    else
        c.frame_number = detector_frame_number - 1;
    c.debug          = tmp[0];
    c.timestamp      = tmp[1];
    c.bunchid        = bit_concat(tmp[2], tmp[3]);

    if (all_packets_ok) {
        // All packets arrived, no more messages in completion coming
        c.packet_mask[0] = UINT64_MAX;
        c.packet_mask[1] = UINT64_MAX;
        c.packet_mask[2] = UINT64_MAX;
        c.packet_mask[3] = UINT64_MAX;
    } else {
        ReadMailbox(tmp);
        c.packet_mask[0] = bit_concat(tmp[2], tmp[3]);
        c.packet_mask[1] = bit_concat(tmp[0], tmp[1]);
        ReadMailbox(tmp);
        c.packet_mask[2] = bit_concat(tmp[2], tmp[3]);
        c.packet_mask[3] = bit_concat(tmp[0], tmp[1]);
    }

    return c;
}

void AcquisitionDevice::WaitForActionComplete() {
    auto c = work_completion_queue.GetBlocking();

    while (c.frame_number != Completion::MeasurementDone) {

        if (c.frame_number >= expected_frames) {
            HW_SetCancelDataCollectionBit();
            // this frame is not of any interest, therefore its location can be immediately released
            SendWorkRequest(c.handle);
            c.handle = UINT32_MAX;
        } else {
            counters.UpdateCounters(&c);
            completion_vector.Add(c);
        }

        if (logger != nullptr)
            logger->Debug("Data stream " + std::to_string(data_stream)
                          + " completion frame number " + std::to_string(c.frame_number)
                          + " module " + std::to_string(c.module)
                          + " handle " + std::to_string(c.handle)
                          + " timestamp " + std::to_string(c.timestamp));

        c = work_completion_queue.GetBlocking();
    }

    read_work_completion_future.get();
    counters.SetAcquisitionFinished();

    end_time = std::chrono::system_clock::now();

    EndWorkRequestAndSignalQueues();

    HW_EndAction();

    while (!HW_IsIdle())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void AcquisitionDevice::EndWorkRequestAndSignalQueues() {
    HW_SetCancelDataCollectionBit();
    work_request_queue.Put(0); // We use 0 for end of data collection - so if this is put into the queue, it will be
    send_work_request_future.get();
}

void AcquisitionDevice::SendWorkRequest(uint32_t handle) {
    work_request_queue.Put(handle+1);
}

uint64_t AcquisitionDevice::GetBytesReceived() const {
    return bytes_received;
}

void AcquisitionDevice::SaveStatistics(const DiffractionExperiment &experiment,
                                       JFJochProtoBuf::AcquisitionDeviceStatistics &statistics) const {

    statistics.set_bytes_received(GetBytesReceived());
    statistics.set_start_timestamp(start_time.time_since_epoch().count());
    statistics.set_end_timestamp(end_time.time_since_epoch().count());

    completion_vector.FillStatistics(experiment, data_stream, statistics);
    *statistics.mutable_fpga_status() = GetStatus();
}

uint64_t AcquisitionDevice::GetHead(uint8_t module) const {
    return counters.GetHead(module);
}

uint64_t AcquisitionDevice::GetSlowestHead() const {
    return counters.GetSlowestHead();
}

bool AcquisitionDevice::IsDone() const {
    return counters.IsAcquisitionFinished();
}

void AcquisitionDevice::ActionAbort() {
    HW_SetCancelDataCollectionBit();
}

const int16_t *AcquisitionDevice::GetFrameBuffer(size_t frame_number, uint16_t module) const {
    auto handle = GetBufferHandle(frame_number, module);
    if (handle != HandleNotValid)
        return (int16_t *) buffer_device.at(handle);
    else
        return GetErrorFrameBuffer();
}

const int16_t *AcquisitionDevice::GetErrorFrameBuffer() const {
    return buffer_err.data();
}

int16_t *AcquisitionDevice::GetDeviceBuffer(size_t handle) {
    if (handle >= buffer_device.size())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Handle outside of range");
    else
        return (int16_t *) buffer_device.at(handle);
}

void AcquisitionDevice::InitializeCalibration(const DiffractionExperiment &experiment, const JFCalibration &calib) {
    auto offset = experiment.GetFirstModuleOfDataStream(data_stream);

    if (calib.GetModulesNum() != experiment.GetModulesNum())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Mismatch regarding module count in calibration and experiment description");

    if (calib.GetStorageCellNum() != experiment.GetStorageCellNumber())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Mismatch regarding storage cell count in calibration and experiment description");

    if (gain0.size() != max_modules)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Gain factors not loaded");

    if (1 + max_modules * (3 + 3 * experiment.GetStorageCellNumber()) > buffer_device.size())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Not enough host/FPGA buffers to load all calibration constants");

    for (int m = 0; m < experiment.GetModulesNum(data_stream); m++) {
        memcpy(buffer_device[1 + m],          gain0[m].data(), RAW_MODULE_SIZE * sizeof(uint16_t));
        memcpy(buffer_device[1 + m + max_modules],     gain1[m].data(), RAW_MODULE_SIZE * sizeof(uint16_t));
        memcpy(buffer_device[1 + m + max_modules * 2], gain2[m].data(), RAW_MODULE_SIZE * sizeof(uint16_t));
    }

    for (int s = 0; s < experiment.GetStorageCellNumber(); s++) {
        for (int m = 0; m < experiment.GetModulesNum(data_stream); m++) {
            for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                buffer_device[1 + m + (3 + 0 * 16 + s) * max_modules][i] = calib.Pedestal(offset + m, 0,s)[i];
                buffer_device[1 + m + (3 + 1 * 16 + s) * max_modules][i] = calib.Pedestal(offset + m, 1,s)[i];
                buffer_device[1 + m + (3 + 2 * 16 + s) * max_modules][i] = calib.Pedestal(offset + m, 2,s)[i];
            }
        }
    }
}

void AcquisitionDevice::MapBuffersStandard(size_t c2h_buffer_count, size_t h2c_buffer_count, int16_t numa_node) {
    try {
        for (int i = 0; i < std::max(c2h_buffer_count, h2c_buffer_count); i++)
            buffer_device.emplace_back((uint16_t *) mmap_acquisition_buffer(FPGA_BUFFER_LOCATION_SIZE, numa_node));
    } catch (const JFJochException &e) {
        UnmapBuffers();
        throw;
    }
}

void AcquisitionDevice::UnmapBuffers() {
     for (auto &i: buffer_device)
        if (i != nullptr) munmap(i, FPGA_BUFFER_LOCATION_SIZE);
}

void AcquisitionDevice::LoadModuleGain(const JFModuleGainCalibration &gain_calibration, uint16_t module) {
    if (module >= max_modules)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Module number out of bounds");

    auto &vector = gain_calibration.GetGainCalibration();
    if (gain0.empty())
        gain0.resize(max_modules);
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        gain0[module][i] = to_fixed(GAIN_G0_MULTIPLIER, vector.at(i),14);

    if (gain1.empty())
        gain1.resize(max_modules);
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        gain1[module][i] = to_fixed(GAIN_G1_MULTIPLIER, vector.at(i + RAW_MODULE_SIZE), 12);

    if (gain2.empty())
        gain2.resize(max_modules);
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        gain2[module][i] = to_fixed(GAIN_G2_MULTIPLIER, vector.at(i + 2 * RAW_MODULE_SIZE), 10);
}

void AcquisitionDevice::SendWorkRequestThread() {
    auto handle = work_request_queue.GetBlocking();
    while (handle != 0) {
        // Preferably use the smallest handle (to reduce buffer size for better TLB usage)
        // So if work request cannot be sent, return handle and check again for the smallest one
        if (!HW_SendWorkRequest(handle - 1)) {
            work_request_queue.Put(handle);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        handle = work_request_queue.GetBlocking();
    }
    while (!HW_SendWorkRequest(UINT32_MAX))
        std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void AcquisitionDevice::FrameBufferRelease(size_t frame_number, uint16_t module) {
    auto handle = counters.GetBufferHandleAndClear(frame_number, module);
    if (handle != AcquisitionOnlineCounters::HandleNotFound)
        SendWorkRequest(handle);
}

void AcquisitionDevice::EnableLogging(Logger *in_logger) {
    logger = in_logger;
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

    ret.set_fpga_temp_degc(env.fpga_temp_C);

    ret.set_current_edge_12v_a(static_cast<double>(env.fpga_pcie_12V_I_mA) / 1000.0);
    ret.set_voltage_edge_12v_v(static_cast<double>(env.fpga_pcie_12V_V_mV) / 1000.0);

    ret.set_current_edge_3p3v_a(static_cast<double>(env.fpga_pcie_3p3V_I_mA) / 1000.0);
    ret.set_voltage_edge_3p3v_v(static_cast<double>(env.fpga_pcie_3p3V_V_mV) / 1000.0);

    ret.set_pcie_c2h_beats(env.pcie_c2h_beats);
    ret.set_pcie_h2c_beats(env.pcie_h2c_beats);
    ret.set_pcie_c2h_descriptors(env.pcie_c2h_descriptors);
    ret.set_pcie_h2c_descriptors(env.pcie_h2c_descriptors);
    ret.set_pcie_c2h_status(env.pcie_c2h_status);
    ret.set_pcie_h2c_status(env.pcie_h2c_status);

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
