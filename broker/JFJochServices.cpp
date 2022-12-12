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

    if ((experiment.GetImageNum() > 0) && (!experiment.GetFilePrefix().empty())) {

        experiment.FillWriterMetadata(request);
        calibration.Export(experiment, request);
        request.set_start_time_ms(current_time_ms());

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

    std::unique_ptr<JFJochException> exception;

    try {
        logger.Info("Wait for receiver done");
        *ret.mutable_receiver() = receiver.Stop();

        logger.Info("    ... Receiver efficiency: {} %     Max delay: {}     Compression ratio {}x",
                    static_cast<int>(ret.receiver().efficiency()*100.0),
                    ret.receiver().max_receive_delay(),
                    static_cast<int>(std::round(ret.receiver().compressed_ratio())));

    } catch (const JFJochException &e) {
        logger.Error("   ... finished with error {}",e.what());
        exception = std::make_unique<JFJochException>(e);
    }
    logger.Info("Receiver finished with success");

    if (indexer_running) {
        logger.Info("Stopping indexer");
        try {
            *ret.mutable_indexer() = indexer.Stop();
        } catch (JFJochException &e) {
            logger.Error("   ... finished with error {}",e.what());
            exception = std::make_unique<JFJochException>(e);
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
            logger.Info("   ... write HDF5 master file");

            request.set_end_time_ms(current_time_ms());

            if (ret.has_receiver()) {
                request.set_image_number(static_cast<int64_t>(ret.receiver().max_image_number_sent()) + 1);
                request.mutable_detector_metadata()->set_efficiency(ret.receiver().efficiency());
                request.mutable_detector_metadata()->set_collection_cancelled(ret.receiver().cancelled());
                request.mutable_detector_metadata()->set_max_receiver_delay(ret.receiver().max_receive_delay());
            }

            if (ret.has_indexer()) {
                *request.mutable_processing_results()->mutable_image_output() = ret.indexer().image_output();
            }

            writer.WriteMasterFile(request);
            logger.Info("   ... done");
        } catch (JFJochException &e) {
            logger.Error("   ... finished with error {}",e.what());
            exception = std::make_unique<JFJochException>(e);
        }
    }

    logger.Info("Stopping detector");
    try {
        detector.Stop();
        logger.Info("   ... done");
    } catch (JFJochException &e) {
        logger.Error("   ... finished with error {}",e.what());
        exception = std::make_unique<JFJochException>(e);
    }

    if (exception)
        throw JFJochException(*exception);

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

JFJochProtoBuf::BrokerPlots JFJochServices::GetPlots() {
    JFJochProtoBuf::BrokerPlots plots;
    // This is a best-effort function
    try {
        auto tmp = indexer.GetPlots();
        *plots.mutable_indexing_rate() = tmp.indexing_rate();
    } catch (...) {}
    try {
        auto tmp = receiver.GetPlots();
        *plots.mutable_bkg_estimate() = tmp.bkg_estimate();
        *plots.mutable_spot_count() = tmp.spot_count();
        *plots.mutable_radial_int_profile() = tmp.radial_int_profile();
    } catch (...) {}
    return plots;
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
    *request.mutable_facility_metadata() = input;
    return *this;
}

void JFJochServices::Trigger() {
    detector.Trigger();
}

size_t JFJochServices::WriterZMQCount() const {
    return writer_zmq_addr.size();
}