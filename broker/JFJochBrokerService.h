// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHBROKERSERVICE_H
#define JUNGFRAUJOCH_JFJOCHBROKERSERVICE_H

#include "jfjoch.grpc.pb.h"
#include "JFJochStateMachine.h"
#include "JFJochServices.h"

class JFJochBrokerService : public JFJochProtoBuf::gRPC_JFJochBroker::Service {
    DiffractionExperiment experiment;
    std::mutex experiment_mutex;
    DiffractionExperiment GetExperiment();

    JFJochServices services;
    JFJochStateMachine state_machine {services, experiment};
    JFJochProtoBuf::DataProcessingSettings data_processing_settings;
    std::mutex data_processing_settings_mutex;
    JFJochProtoBuf::DataProcessingSettings GetDataProcessingSettings();
    Logger &logger;
    static void ParseBrokerSetup(DiffractionExperiment &experiment, const JFJochProtoBuf::BrokerSetup *request);
public:
    JFJochBrokerService(Logger &logger);

    DiffractionExperiment& Experiment(); // Not protected via mutex, don't execute in parallel context !
    JFJochServices& Services();

    grpc::Status Start(grpc::ServerContext *context, const JFJochProtoBuf::BrokerSetup *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                      JFJochProtoBuf::Empty *response) override;
    grpc::Status Abort(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Cancel(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                       JFJochProtoBuf::Empty *response) override;
    grpc::Status Initialize(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                            JFJochProtoBuf::Empty *response) override;
    grpc::Status Deactivate(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                            JFJochProtoBuf::Empty *response) override;
    grpc::Status Pedestal(grpc::ServerContext *context, const JFJochProtoBuf::BrokerSetup *request,
                          JFJochProtoBuf::Empty *response) override;
    grpc::Status Setup(grpc::ServerContext *context, const JFJochProtoBuf::BrokerPersistentSettings *request,
                         JFJochProtoBuf::Empty *response) override;

    grpc::Status GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                           JFJochProtoBuf::BrokerStatus *response) override;
    grpc::Status GetCalibration(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                  JFJochProtoBuf::JFCalibration *response) override;
    grpc::Status GetDetailedReceiverOutput(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                           JFJochProtoBuf::JFJochReceiverOutput *response) override;
    grpc::Status GetPreviewFrame(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                               JFJochProtoBuf::PreviewFrame *response) override;
    grpc::Status SetDataProcessingSettings(grpc::ServerContext *context, const JFJochProtoBuf::DataProcessingSettings *request,
                                           JFJochProtoBuf::Empty *response) override;
    grpc::Status GetDataProcessingSettings(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                             JFJochProtoBuf::DataProcessingSettings *response) override;

    grpc::Status LoadMask(grpc::ServerContext *context, const JFJochProtoBuf::MaskToLoad *request,
                            JFJochProtoBuf::Empty *response) override;
    grpc::Status GetMask(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                           JFJochProtoBuf::Image *response) override;
    grpc::Status GetMaskRawCoord(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                         JFJochProtoBuf::Image *response) override;

    grpc::Status GetPedestalG0(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                 JFJochProtoBuf::Image *response) override;
    grpc::Status GetPedestalG1(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                               JFJochProtoBuf::Image *response) override;
    grpc::Status GetPedestalG2(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                               JFJochProtoBuf::Image *response) override;
};


#endif //JUNGFRAUJOCH_JFJOCHBROKERSERVICE_H
