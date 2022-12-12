// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include "MakeAcquisitionDevice.h"
#include "JFJochReceiverTest.h"
#include "../tests/FPGAUnitTest.h"

int main(int argc, char **argv) {
    uint16_t nstreams = 1;
    uint16_t nmodules = 1;
    size_t nimages = 2;
    uint64_t processing_period = 20;

    Logger logger("ActionTest");
    logger.Verbose(true);

    bool abort_test = false;

    if ((argc == 1) || (argc > 6)) {
        logger.Error("Usage ./jfjoch_action_test <path to JFjoch source> {<# of images> {<# of modules> {<# of streams> {<processing period>}}}}");
        exit(EXIT_FAILURE);
    }

    if (argc >= 3) nimages = atol(argv[2]);
    if (argc >= 4) nmodules = atol(argv[3]);
    if (argc >= 5) nstreams = atoi(argv[4]);
    if (argc >= 6) processing_period = atoi(argv[5]);



    std::vector<int64_t> detector_geom;
    for (int i = 0; i < nstreams; i++)
        detector_geom.push_back(nmodules);

    DiffractionExperiment x(1, detector_geom);
    x.Mode(DetectorMode::Conversion);
    x.ImagesPerTrigger(nimages).PedestalG0Frames(0).UseInternalPacketGenerator(true).PhotonEnergy_keV(12.4).NumTriggers(1);
    x.SpotFindingPeriod(std::chrono::milliseconds(processing_period))
            .BackgroundEstimationPeriod(std::chrono::milliseconds(processing_period));
    x.Compression(JFJochProtoBuf::BSHUF_ZSTD);

#ifdef __PPC__
    std::vector<uint16_t> pci_slot_number = {4, 5};
    std::vector<uint16_t> numa_node = {0, 8};
    bool verbose = false;
    bool print_status_updates = true;
    size_t frame_buffer_size = 4096;
    uint16_t nthreads = nstreams * 40;
#else
    std::vector<int16_t> pci_slot_number = {4};
    std::vector<int16_t> numa_node = {-1};
    bool verbose = true;
    bool print_status_updates = false;
    size_t frame_buffer_size = 32;
    uint16_t nthreads = nstreams * 2;
#endif

    logger.Verbose(verbose);

    if (nstreams > pci_slot_number.size()) {
        logger.Error("Only {} data streams allowed on this platform", pci_slot_number.size());
        exit(EXIT_FAILURE);
    }

    std::vector<std::unique_ptr<AcquisitionDevice>> oc_devices;
    std::vector<AcquisitionDevice *> aq_devices;

    std::string image_path = std::string(argv[1]) + "/tests/test_data/mod5_raw0.bin";
    std::vector<uint16_t> input(RAW_MODULE_SIZE, 0);
    LoadBinaryFile(image_path, input.data(), RAW_MODULE_SIZE);

    for (int i = 0; i < nstreams; i++) {
        oc_devices.push_back(MakeAcquisitionDevice(AcquisitionDeviceType::OpenCAPI, i, frame_buffer_size,
                                                   pci_slot_number[i], numa_node[i]));
        oc_devices[i]->SetCustomInternalGeneratorFrame(input);
        oc_devices[i]->EnableLogging(&logger);
        aq_devices.push_back(oc_devices[i].get());
    }

    volatile bool done = false;
    JFJochProtoBuf::ReceiverOutput output;
    bool ret;
    std::thread run_thread([&] {
        try {
            ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads,abort_test, verbose);
        } catch (std::exception &e) {
            logger.Error(e.what());
            ret = false;
        }
        done = true;
    });

    if (print_status_updates) {
        while (!done) {
            for (int i = 0; i < nstreams; i++) {
                auto status = oc_devices[i]->GetStatus();
                logger.Info("Device {}:  Head packet: {}  Power: {} W  FPGA Temp: {:d} degC  HBM Temp: {:d} degC  Stalls: {}",
                            i, status.slowest_head(), status.current_edge_12v_a() * status.voltage_edge_12v_v() +
                                                      status.current_edge_3p3v_a() * status.voltage_edge_3p3v_v(),
                            status.fpga_temp_degc(), status.hbm_temp(), status.stalls_hbm());
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    run_thread.join();

    double receiving_time = static_cast<double>(output.end_time_ms() - output.start_time_ms())/1000.0;

    logger.Info("Max delay: {}",output.max_receive_delay());
    logger.Info("Compression ratio: {}", output.compressed_ratio());
    logger.Info("Receiving time: {} s", receiving_time);
    logger.Info("Frame rate: {} Hz", static_cast<double>(nimages)/receiving_time);
    logger.Info("Total throughput: {:.2f} GB/s",
            static_cast<double>(nimages*nstreams*nmodules*RAW_MODULE_SIZE*sizeof(uint16_t)) / (receiving_time * 1e9));
    logger.Info("");
    for (int i = 0; i < nstreams; i++) {
        auto stalls = output.device_statistics(i).fpga_status().stalls_hbm();

        uint64_t clock_MHz = 250;
        uint64_t throughput_MBs = nimages * nmodules * RAW_MODULE_SIZE*sizeof(uint16_t) * clock_MHz / (nimages * nmodules * 128 * 128 + stalls);
        double performance = static_cast<double>(throughput_MBs) / 1000;
        // Assuming 250 MHz clock
        logger.Info("Device {}:  stalls: {}   est. performance: {:.2f} GB/s", i, stalls, performance);

        if (output.device_statistics(i).fpga_status().frame_statistics_alignment_err())
            logger.Error("Device {}: memory alignment error", i);
        if (output.device_statistics(i).fpga_status().frame_statistics_tlast_err())
            logger.Error("Device {}: error in AXI-Stream sequence", i);
        if (output.device_statistics(i).fpga_status().frame_statistics_work_req_err())
            logger.Error("Device {}: parity error in work request", i);
        if (output.device_statistics(i).fpga_status().datamover_s2mm_error())
            logger.Error("Device {}: AXI-Stream to AXImm error", i);
        if (output.device_statistics(i).fpga_status().mailbox_err_reg() != 0)
            logger.Error("Device {}: Mailbox error {:x}", i,
                         output.device_statistics(i).fpga_status().mailbox_err_reg());
    }

    if (ret) {
        logger.Info("Test properly executed! (check stall values manually)");
        exit(EXIT_SUCCESS);
    } else
        exit(EXIT_FAILURE);
}
