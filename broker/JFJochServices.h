// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHSERVICES_H
#define JUNGFRAUJOCH_JFJOCHSERVICES_H

#include "../common/DiffractionExperiment.h"
#include "../common/JungfrauCalibration.h"
#include "../common/Logger.h"
#include "../grpc/JFJochReceiverClient.h"
#include "../grpc/JFJochWriterGroupClient.h"
#include "../grpc/JFJochTriggerClient.h"
#include "../grpc/JFJochDetectorClient.h"
#include "../grpc/JFJochIndexerClient.h"

class JFJochServices {
    JFJochReceiverClient receiver;
    JFJochWriterGroupClient writer;
    JFJochTriggerClient trigger;
    JFJochDetectorClient detector;
    JFJochIndexerClient indexer;
    Logger &logger;
    bool indexer_running = false;
    bool writer_running = false;
    std::vector<std::string> writer_zmq_addr;
public:
    JFJochServices(Logger &in_logger);
    void On(const DiffractionExperiment& experiment);
    void Off();
    void Start(const DiffractionExperiment& experiment, const JungfrauCalibration &calibration);
    JFJochProtoBuf::JFJochReceiverOutput Stop();
    void Abort();
    void Cancel();
    bool IsDetectorIdle();

    JFJochProtoBuf::ReceiverStatus GetReceiverStatus();
    JFJochProtoBuf::IndexerStatus GetIndexerStatus();
    JFJochProtoBuf::PreviewFrame GetPreviewFrame();

    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings);
    JFJochServices& Receiver(const std::string &addr);
    JFJochServices& Writer(const std::string &addr, const std::string &zmq_pull_addr);
    JFJochServices& Trigger(const std::string &addr);
    JFJochServices& Detector(const std::string &addr);
    JFJochServices& Indexer(const std::string &addr, const std::string &zmq_sub_addr);
};


#endif //JUNGFRAUJOCH_JFJOCHSERVICES_H
