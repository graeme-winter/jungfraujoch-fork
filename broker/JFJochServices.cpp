// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochServices.h"
#include "../common/JFJochException.h"

uint64_t current_time_ms() {
    auto curr_time = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(curr_time.time_since_epoch()).count();
}

JFJochServices::JFJochServices(Logger &in_logger) : logger(in_logger) {}

void JFJochServices::Start(const DiffractionExperiment& experiment, const JFCalibration &calibration) {
    logger.Info("Measurement start for: {}", experiment.GetFilePrefix());

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        logger.Info("   ... indexer start");
        indexer.Start(experiment);
        indexer_running = true;
    } else
        indexer_running = false;

    if ((experiment.GetImageNum() > 0) && (!experiment.GetFilePrefix().empty())){
        logger.Info("   ... write HDF5 master file");

        JFJochProtoBuf::WriterMetadataInput request;
        experiment.FillWriterMetadata(request);
        calibration.Export(experiment, request);
        request.set_start_time_ms(current_time_ms());
        writer.WriteMasterFile(request);

        logger.Info("   ... writer start");
        writer.Start(experiment, writer_zmq_addr);
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
    }
    logger.Info("   Done!");
}

void JFJochServices::Off() {
    detector.Off();
}

void JFJochServices::On(const DiffractionExperiment &x) {
    logger.Info("Detector on");

    JFJochProtoBuf::DetectorConfig config = x.DetectorConfig(receiver.GetNetworkConfig());

    detector.On(config);
    logger.Info("   ... done");
}

JFJochProtoBuf::BrokerFullStatus JFJochServices::Stop(const JFCalibration &calibration) {
    JFJochProtoBuf::BrokerFullStatus ret;
    try {
        logger.Info("Wait for receiver done");
        *ret.mutable_receiver() = receiver.Stop();

        logger.Info("    ... Receiver efficiency: {} %     Max delay: {}     Compression ratio {}x",
                    static_cast<int>(ret.receiver().efficiency()*100.0),
                    ret.receiver().max_receive_delay(),
                    static_cast<int>(std::round(ret.receiver().compressed_ratio())));

    } catch (const JFJochException &e) {
        logger.Error("Receiver finished with error {}",e.what());
        try {
            // If receiver failed with error, then need to stop writer
            if (writer_running)
                writer.Stop();
            if (indexer_running)
                indexer.Stop();
            detector.Stop();
        } catch (...) {}
        throw;
    }
    logger.Info("Receiver finished with success");

    if (indexer_running) {
        logger.Info("Stopping indexer");
        try {
            *ret.mutable_indexer() = indexer.Stop();
        } catch (JFJochException &e) {

            logger.Error("   ... finished with error {}",e.what());
            try {
                if (writer_running)
                    writer.Stop();
                detector.Stop();
            } catch (...) {}
            throw;
        }
        logger.Info("   ... finished with success.");
        logger.Info("   ... indexed {} out of {}", ret.indexer().indexed_images(),
                    ret.indexer().image_output_size());
    }
    if (writer_running) {
        logger.Info("Stopping writer");
        try {
            auto stats = writer.Stop();
            logger.Info("   ... finished with success");
            for (int i = 0; i < stats.size(); i++) {
                *ret.add_writer() = stats[i];
                logger.Info("Writer {}: Images = {} Throughput = {:.0f} MB/s Frame rate = {:.0f} Hz",
                            i, stats[i].nimages(), stats[i].performance_mbs(), stats[i].performance_hz());
            }
        } catch (JFJochException &e) {
            logger.Error("   ... writer finished with error {}", e.what());
            try {
                detector.Stop();
            } catch (...) {}
            throw;
        }
    }

    try {
        detector.Stop();
    } catch (JFJochException &e) {
        logger.Error("Detector failed with error {} ", e.what());
        throw;
    }

    return ret;
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

JFJochServices &JFJochServices::Receiver(const std::string &addr) {
    receiver.Connect(addr);
    logger.Info("Using receiver service with gRPC " + addr);
    return *this;
}

JFJochServices &JFJochServices::Writer(const std::string &addr, const std::string &zmq_push_addr) {
    writer.AddClient(addr);
    writer_zmq_addr.push_back(zmq_push_addr);
    logger.Info("Using writer   service with gRPC {} listening for images from ZeroMQ {}", addr,  zmq_push_addr);
    return *this;
}

JFJochServices &JFJochServices::Detector(const std::string &addr) {
    detector.Connect(addr);
    logger.Info("Using detector service with gRPC {}", addr);
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
    logger.Info("Using indexer  service with gRPC {}    connected to ZMQ subscriber {}", addr, zmq_sub_addr);
    return *this;
}

JFJochProtoBuf::PreviewFrame JFJochServices::GetPreviewFrame() {
    return receiver.GetPreviewFrame();
}

JFJochServices &JFJochServices::FacilityMetadata(const JFJochProtoBuf::FacilityMetadata &input) {
    facility_metadata = input;
    return *this;
}