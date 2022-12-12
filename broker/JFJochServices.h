// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHSERVICES_H
#define JUNGFRAUJOCH_JFJOCHSERVICES_H

#include "../common/DiffractionExperiment.h"
#include "../common/JFCalibration.h"
#include "../common/Logger.h"
#include "../grpc/JFJochReceiverClient.h"
#include "../grpc/JFJochWriterGroupClient.h"
#include "../grpc/JFJochDetectorClient.h"
#include "../grpc/JFJochIndexerClient.h"

class JFJochServices {
    JFJochReceiverClient receiver;
    JFJochWriterGroupClient writer;
    JFJochDetectorClient detector;
    JFJochIndexerClient indexer;

    JFJochProtoBuf::WriterMetadataInput request;

    Logger &logger;
    bool indexer_running = false;
    bool writer_running = false;
    std::vector<std::string> writer_zmq_addr;
public:
    JFJochServices(Logger &in_logger);
    void On(const DiffractionExperiment& experiment);
    void Off();
    void Start(const DiffractionExperiment& experiment, const JFCalibration &calibration);
    JFJochProtoBuf::BrokerFullStatus Stop(const JFCalibration &calibration);
    void Abort();
    void Cancel();
    void Trigger();

    JFJochProtoBuf::ReceiverStatus GetReceiverStatus();
    JFJochProtoBuf::IndexerStatus GetIndexerStatus();
    JFJochProtoBuf::PreviewFrame GetPreviewFrame();
    JFJochProtoBuf::BrokerPlots GetPlots();

    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings);
    JFJochServices& Receiver(const std::string &addr);
    JFJochServices& Writer(const std::string &addr, const std::string &zmq_push_addr);
    JFJochServices& Detector(const std::string &addr);
    JFJochServices& Indexer(const std::string &addr, const std::string &zmq_sub_addr);
    JFJochServices& FacilityMetadata(const JFJochProtoBuf::FacilityMetadata &input);

    size_t WriterZMQCount() const;
};


#endif //JUNGFRAUJOCH_JFJOCHSERVICES_H
