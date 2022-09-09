// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <nlohmann/json.hpp>
#include <fstream>

#include "../../grpc/gRPCServer_Template.h"
#include "../../common/ZMQImagePusher.h"
#include "JFJochReceiverService.h"
#ifdef JFJOCH_USE_OPENCAPI
#include "OpenCAPIDevice.h"
#endif
#include "HLSSimulatedDevice.h"
#include "GenerateOpenCAPIDeviceName.h"

AcquisitionDevice *SetupAcquisitionDevice(const nlohmann::json &input, uint16_t data_stream) {
    AcquisitionDevice *ret;

    int16_t numa_node = -1;
    if (input.contains("numa_node"))
        numa_node = input["numa_node"];

    int64_t frame_buffer_size = UINT16_MAX;
    if (input.contains("frame_buffer_size"))
        frame_buffer_size = input["frame_buffer_size"];

    if (input.contains("type") && (input["type"] == "software"))
        ret = new HLSSimulatedDevice(data_stream, frame_buffer_size, numa_node);
#ifdef JFJOCH_USE_OPENCAPI
    else {
        auto oc = new OpenCAPIDevice(input["pci_slot"].get<uint16_t>(), data_stream,
                                 frame_buffer_size, numa_node);
        if (input.contains("fpga_mac_addr"))
            oc->SetMACAddress(DiffractionExperiment::MacAddressFromStr(input["fpga_mac_addr"].get<std::string>()));
        ret = oc;
    }
#else
    else throw JFJochException(JFJochExceptionCategory::OpenCAPIError, "OpenCAPI not available");
#endif

    if (input.contains("gain_file") && (input["gain_file"].is_array())) {
        for (int i = 0; i < input["gain_file"].size(); i++)
            ret->LoadModuleGain(input["gain_file"][i].get<std::string>(), i);
    }

    if (input.contains("custom_test_frame")) {
        std::vector<uint16_t> tmp(ret->GetInternalPacketGeneratorModuleNum() * RAW_MODULE_SIZE);
        for (int i = 0; i < ret->GetInternalPacketGeneratorModuleNum(); i++) {
            auto filename = input["custom_test_frame"].get<std::string>();
            std::fstream file(filename.c_str(), std::fstream::in | std::fstream::binary);
            file.read(((char *) tmp.data()) + i * RAW_MODULE_SIZE * sizeof(uint16_t),
                      RAW_MODULE_SIZE * sizeof(uint16_t));
        }
        ret->SetCustomInternalGeneratorFrame(tmp);
    }

    return ret;
}

int main(int argc, char **argv) {
    if ((argc < 2) || (argc > 3)) {
        std::cout << "Usage ./jfjoch_receiver <JSON config> {<log file>}" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string log_file;
    if (argc >= 3)
        log_file = argv[2];

    Logger logger("jfjoch_receiver", log_file);

    nlohmann::json input;
    std::ifstream file(argv[1]);
    try {
        input = nlohmann::json::parse(file);
    } catch (const nlohmann::json::exception &e) {
        logger.Error("JSON Parsing exception: " + std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    std::string grpc_addr;
    try {
        grpc_addr = input["grpc_addr"];
    } catch (...) {
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "grpc_addr is compulsory parameter");
    }

    bool verbose = false;
    if (input.contains("verbose") && (input["verbose"]))
        verbose = true;

    int32_t zmq_threads = 4;
    if (input.contains("zmq_threads"))
        zmq_threads = input["zmq_threads"];

    ZMQContext context;
    context.NumThreads(zmq_threads);

    logger.Verbose(verbose);

    std::vector<std::unique_ptr<AcquisitionDevice>> aq_devices;
    for (int i = 0; i < input["opencapi"].size(); i++) {
        auto ptr = SetupAcquisitionDevice(input["opencapi"][i], i);
        aq_devices.emplace_back(ptr);
    }

    std::vector<AcquisitionDevice *> aq_devices_ptr;
    for (const auto &i: aq_devices) {
        if (verbose)
            i->EnableLogging(&logger);
        aq_devices_ptr.push_back(i.get());
    }

    logger.Info("Enabled acquisition device count: " + std::to_string(aq_devices.size()));

    JFJochReceiverService service(aq_devices_ptr, context, logger);

    std::unique_ptr<ZMQPreviewPublisher> preview;
    if (input.contains("preview_zmq_addr")) {
        preview = std::make_unique<ZMQPreviewPublisher>(context, input["preview_zmq_addr"]);
        service.PreviewPublisher(preview.get());
        logger.Info("Preview available on ZMQ addr " + input["preview_zmq_addr"].get<std::string>());
    }

    std::unique_ptr<ZMQSpotPublisher> spot_publisher;
    if (input.contains("spot_zmq_addr")) {
        spot_publisher = std::make_unique<ZMQSpotPublisher>(context, input["spot_zmq_addr"]);
        service.SpotPublisher(spot_publisher.get());
        logger.Info("Spot finder available on ZMQ addr " + input["spot_zmq_addr"].get<std::string>());
    }

    if (input.contains("tcp_send_buffer_size"))
        service.SendBufferSize(input["tcp_send_buffer_size"]);

    if (input.contains("zmq_send_high_watermark"))
        service.SendBufferHighWatermark(input["zmq_send_high_watermark"]);

    if (input.contains("compression_threads"))
        service.NumThreads(input["compression_threads"]);

    auto server = gRPCServer(grpc_addr, service);
    logger.Info("gRPC configuration listening on address " + grpc_addr);
    logger.Info("Started");
    server->Wait();
}