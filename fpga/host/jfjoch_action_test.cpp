// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include "OpenCAPIDevice.h"
#include "JFJochReceiverTest.h"

int main(int argc, char **argv) {
    uint16_t nstreams = 1;
    uint16_t nmodules = 1;
    size_t nimages = 2;
    std::string logfile = "";

    bool abort_test = false;

    if ((argc == 1) || (argc > 6)) {
        std::cout << "Usage ./jfjoch_action_test <path to JFjoch source> {<# of images> {<# of modules> {<# of streams> {<log filename>}}}}" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (argc >= 3) nimages = atol(argv[2]);
    if (argc >= 4) nmodules = atol(argv[3]);
    if (argc >= 5) nstreams = atoi(argv[4]);
    if (argc >= 6) logfile = std::string(argv[5]);

    Logger logger("ActionTest", logfile);
    logger.Verbose(true);
    DiffractionExperiment x;

    std::vector<int64_t> detector_geom;
    for (int i = 0; i < nstreams; i++)
        detector_geom.push_back(nmodules);

    x.Mode(DetectorMode::Conversion).DataStreamModuleSize(1, detector_geom);
    x.ImagesPerTrigger(nimages).PedestalG0Frames(0).UseInternalPacketGenerator(true).PhotonEnergy_keV(12.4).NumTriggers(0);
    x.SpotFindingPeriod(std::chrono::milliseconds(40)).ImagesPerFile(100);
    x.Compression(CompressionAlgorithm::BSHUF_ZSTD,ZSTD_USE_JFJOCH_RLE);

#ifdef __PPC__
    std::vector<uint16_t> pci_slot_number = {4, 5};
    std::vector<uint16_t> numa_node = {0, 8};
    bool verbose = false;
    size_t frame_buffer_size = 4096;
    uint16_t nthreads = nstreams * 26;
#else
    std::vector<int16_t> pci_slot_number = {4};
    std::vector<int16_t> numa_node = {-1};
    bool verbose = true;
    size_t frame_buffer_size = 32;
    uint16_t nthreads = nstreams * 2;
#endif

    logger.Verbose(verbose);

    if (nstreams > pci_slot_number.size()) {
        logger.Error("Only " + std::to_string(pci_slot_number.size()) + " data streams allowed on this platform");
        exit(EXIT_FAILURE);
    }

    std::vector<std::unique_ptr<OpenCAPIDevice>> oc_devices;
    std::vector<AcquisitionDevice *> aq_devices;

    for (int i = 0; i < nstreams; i++) {
        oc_devices.push_back(std::make_unique<OpenCAPIDevice>(pci_slot_number[i], i, frame_buffer_size, numa_node[i]));
        oc_devices[i]->EnableLogging(&logger);
        aq_devices.push_back(oc_devices[i].get());
    }

    volatile bool done = false;
    JFJochProtoBuf::JFJochReceiverOutput output;
    bool ret;
    std::thread run_thread([&] {
        try {
            ret = JFJochReceiverTest(output, logger, aq_devices, x, nthreads, std::string(argv[1]) + "/",
                                     abort_test, verbose);
        } catch (std::exception &e) {
            logger.Error(e.what());
            ret = false;
        }
        done = true;
    });

#ifdef __PPC__
    while (!done) {
       for (int i = 0; i < nstreams; i++) {
           auto status = oc_devices[i]->GetStatus();
           logger.Info("Dev: " + std::to_string(i)
                       + " Power: " + std::to_string(status.current_edge_12v_a()*status.voltage_edge_12v_v()+status.current_edge_3p3v_a()*status.voltage_edge_3p3v_v())
                       + " FPGA Temp: " + std::to_string(status.fpga_temp_degc())
                       + " HBM Temp: " + std::to_string(status.hbm_temp())
                       + " Stalls: " + std::to_string(status.stalls_hbm()));
       }
       std::this_thread::sleep_for(std::chrono::seconds(1));
    }
#endif

    run_thread.join();

    logger.Info("Max delay: " + std::to_string(output.max_receive_delay()));
    logger.Info("Compression ratio: " + std::to_string(output.compressed_ratio()));
    for (int i = 0; i < nstreams; i++) {
        auto stalls = output.device_statistics(i).fpga_status().stalls_hbm();

        uint64_t clock_MHz = 250;
        uint64_t throughput_MBs = nimages * nmodules * RAW_MODULE_SIZE*sizeof(uint16_t) * clock_MHz / (nimages * nmodules * 128 * 128 + stalls);
        double performance = static_cast<double>(throughput_MBs) / 1000;
        // Assuming 250 MHz clock
        logger.Info("Device " + std::to_string(i) + "   stalls: "  + std::to_string(stalls) +
                    "   est. performance: " + std::to_string(performance).substr(0, 5) + " GB/s");

        if (output.device_statistics(i).fpga_status().frame_statistics_alignment_err())
            logger.Error("Device " + std::to_string(i) + ": memory alignment error");
        if (output.device_statistics(i).fpga_status().frame_statistics_tlast_err())
            logger.Error("Device " + std::to_string(i) + ": error in AXI-Stream sequence");
        if (output.device_statistics(i).fpga_status().frame_statistics_work_req_err())
            logger.Error("Device " + std::to_string(i) + ": parity error in work request");
        if (output.device_statistics(i).fpga_status().datamover_s2mm_error())
            logger.Error("Device " + std::to_string(i) + ": AXI-Stream to AXImm error");
        if (output.device_statistics(i).fpga_status().mailbox_err_reg() != 0)
            logger.Error("Device " + std::to_string(i) + ": Mailbox error " +
                    std::to_string(output.device_statistics(i).fpga_status().mailbox_err_reg()));
    }

    if (ret) {
        logger.Info("Test properly executed! (check stall values manually)");
        exit(EXIT_SUCCESS);
    } else
        exit(EXIT_FAILURE);
}
