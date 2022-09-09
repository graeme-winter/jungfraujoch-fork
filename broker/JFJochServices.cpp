// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochServices.h"
#include "../common/JFJochException.h"

JFJochServices::JFJochServices(Logger &in_logger) : logger(in_logger) {}

void JFJochServices::Start(const DiffractionExperiment& experiment, const JFCalibration &calibration) {
    logger.Info("Measurement start for: " + experiment.GetFilePrefix());

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        logger.Info("   ... indexer start");
        indexer.Start(experiment);
        indexer_running = true;
    } else
        indexer_running = false;

    if (experiment.GetImageNum() > 0) {
        logger.Info("   ... writer start");
        writer.Start(experiment);
        writer_running = true;
    } else
        writer_running = false;

    logger.Info("   ... receiver start");
    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        receiver.Start(experiment, &calibration, writer_zmq_addr);
    else
        receiver.Start(experiment, nullptr, writer_zmq_addr);

    if (!experiment.IsUsingInternalPacketGen()) {
        logger.Info("   ... detector start");
        detector.Start(experiment);

        if (!experiment.GetTimeResolvedMode() &&
            ((experiment.GetDetectorMode() == DetectorMode::Conversion)
            || (experiment.GetDetectorMode() == DetectorMode::Raw))) {
            // Pedestal is using soft trigger
            // Time resolved mode uses external (non RPi) trigger
            logger.Info("   ... trigger");
            trigger.Trigger();
        }
    }
    logger.Info("   Done!");
}

void JFJochServices::Off() {
    detector.Off();
}

void JFJochServices::On(const DiffractionExperiment &x) {
    logger.Info("Detector on");

    auto net_config = receiver.GetNetworkConfig();
    if (net_config.fpga_mac_addr_size() < x.GetDataStreamsNum())
        logger.Warning("Receiver doesn't support enough data streams");
    JFJochProtoBuf::JFJochDetectorConfig config;

    int i = 0;
    for (int d = 0; d < x.GetDataStreamsNum(); d++) {
        for (int module = 0; module < x.GetModulesNum(d); module++) {
            auto mod_cfg = config.add_modules();
            mod_cfg->set_udp_dest_port_1(x.GetDestUDPPort(d,module));
            mod_cfg->set_udp_dest_port_2(x.GetDestUDPPort(d,module) + 1);
            mod_cfg->set_ipv4_src_addr_1(x.GetSrcIPv4Address(i * 2));
            mod_cfg->set_ipv4_src_addr_2(x.GetSrcIPv4Address(i * 2 + 1));
            mod_cfg->set_ipv4_dest_addr_1(x.GetDestIPv4Address(d));
            mod_cfg->set_ipv4_dest_addr_2(x.GetDestIPv4Address(d));
            if (d < net_config.fpga_mac_addr_size()) {
                mod_cfg->set_mac_addr_dest_1(net_config.fpga_mac_addr(d));
                mod_cfg->set_mac_addr_dest_2(net_config.fpga_mac_addr(d));
            }
            i++;
        }
    }

    detector.On(config);
    logger.Info("   ... done");
}

JFJochProtoBuf::JFJochReceiverOutput JFJochServices::Stop(const JFCalibration &calibration) {
    JFJochProtoBuf::JFJochReceiverOutput last_receiver_output;
    try {
        logger.Info("Wait for receiver done");
        last_receiver_output = receiver.Stop();

        logger.Info("    ... Receiver efficiency: " + std::to_string(static_cast<int>(last_receiver_output.efficiency()*100.0)) + " %"
                    + "     Max delay: " + std::to_string(last_receiver_output.max_receive_delay())
                    + "     Compression ratio " + std::to_string(static_cast<int>(std::round(last_receiver_output.compressed_ratio()))) + "x"
                    );

    } catch (const JFJochException &e) {
        logger.Info("Receiver finished with error " + std::string(e.what()));
        // If receiver failed with error, then need to stop writer
        if (writer_running)
            writer.Stop();
        if (indexer_running)
            indexer.Stop();
        throw;
    }
    logger.Info("Receiver finished with success");

    if (indexer_running) {
        logger.Info("Stopping indexer");
        try {
            *last_receiver_output.mutable_indexer_output() = indexer.Stop();
        } catch (JFJochException &e) {
            logger.Info("   ... finished with error " + std::string(e.what()));
            if (writer_running)
                writer.Stop();
            throw;
        }
        logger.Info("   ... finished with success.");
        logger.Info("   ... indexed "
                    + std::to_string(last_receiver_output.indexer_output().indexed_images())
                    + " out of "
                    + std::to_string(last_receiver_output.indexer_output().image_output_size()));
    }
    if (writer_running) {
        logger.Info("Stopping writer");
        try {
            writer.Stop();
            logger.Info("   ... finished with success");
            writer.WriteMasterFile(last_receiver_output, calibration);
            logger.Info("   ... master HDF5 file written");
        } catch (JFJochException &e) {
            logger.Info("   ... writer finished with error " + std::string(e.what()));
            throw;
        }
    }

    return last_receiver_output;
}

void JFJochServices::Abort() {
    // Abort should try to achieve the best outcome possible
    // but it OK if things fail (for example lost connection)
    try {
        receiver.Abort();
    } catch (const std::exception &e) {
        logger.Error(e.what());
    }
    try {
        writer.Abort();
    } catch (const std::exception &e) {
        logger.Error(e.what());
    };
}

void JFJochServices::Cancel() {
    detector.Stop();
    receiver.Cancel();
}

bool JFJochServices::IsDetectorIdle() {
    return detector.IsIdle();
}

JFJochServices &JFJochServices::Receiver(const std::string &addr) {
    receiver.Connect(addr);
    logger.Info("Using receiver service with gRPC " + addr);
    return *this;
}

JFJochServices &JFJochServices::Writer(const std::string &addr, const std::string &zmq_pull_addr) {
    writer.AddClient(addr);
    writer_zmq_addr.push_back(zmq_pull_addr);
    logger.Info("Using writer   service with gRPC " + addr + " listening for images on ZeroMQ " + zmq_pull_addr);
    return *this;
}

JFJochServices &JFJochServices::Detector(const std::string &addr) {
    detector.Connect(addr);
    logger.Info("Using detector service with gRPC " + addr);
    return *this;
}

JFJochServices &JFJochServices::Trigger(const std::string &addr) {
    trigger.Connect(addr);
    logger.Info("Using trigger  service with gRPC " + addr);
    return *this;
}

JFJochProtoBuf::ReceiverStatus JFJochServices::GetReceiverStatus() {
    return receiver.GetStatus();
}

JFJochProtoBuf::IndexerStatus JFJochServices::GetIndexerStatus() {
    return indexer.GetStatus();
}

void JFJochServices::SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings) {
    receiver.SetDataProcessingSettings(settings);
}

JFJochServices &JFJochServices::Indexer(const std::string &addr, const std::string &zmq_sub_addr) {
    indexer.ZMQReceiverAddr(zmq_sub_addr).Connect(addr);
    logger.Info("Using indexer  service with gRPC " + addr + "    connected to ZMQ subscriber " + zmq_sub_addr);
    return *this;
}

JFJochProtoBuf::PreviewFrame JFJochServices::GetPreviewFrame() {
    return receiver.GetPreviewFrame();
}