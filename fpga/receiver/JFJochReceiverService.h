// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVERSERVICE_H
#define JUNGFRAUJOCH_JFJOCHRECEIVERSERVICE_H

#include "JFJochReceiver.h"
#include "jfjoch.grpc.pb.h"
#include <mutex>

class JFJochReceiverService final : public JFJochProtoBuf::gRPC_JFJochReceiver::Service {
    std::unique_ptr<JFJochReceiver> receiver;
    std::vector<AcquisitionDevice *> &aq_devices;

    Logger &logger;

    ZMQImagePusher image_pusher;
    ZMQPreviewPublisher *preview_publisher = nullptr;
    ZMQSpotPublisher *spot_publisher = nullptr;

    int64_t nthreads = 8;

    enum class ReceiverState {Idle, Running};
    ReceiverState state = ReceiverState::Idle;
    std::mutex state_mutex;
    std::condition_variable measurement_done;
    std::future<void> measurement;
    void FinalizeMeasurement();
    JFJochProtoBuf::DataProcessingSettings data_processing_settings;
public:
    JFJochReceiverService(std::vector<AcquisitionDevice *> &open_capi_device,
                          ZMQContext &context, Logger &logger);
    JFJochReceiverService& PreviewPublisher(ZMQPreviewPublisher *in_preview_writer);
    JFJochReceiverService& SpotPublisher(ZMQSpotPublisher *in_spot_writer);
    JFJochReceiverService& NumThreads(int64_t input);
    JFJochReceiverService& SendBufferSize(int32_t bytes);
    JFJochReceiverService& SendBufferHighWatermark(int32_t msgs);

    grpc::Status Start(grpc::ServerContext* context, const JFJochProtoBuf::JFJochReceiverInput* request,
                       JFJochProtoBuf::Empty* response) override;
    grpc::Status Abort(grpc::ServerContext* context, const JFJochProtoBuf::Empty* request,
                       JFJochProtoBuf::Empty* response) override;
    grpc::Status Cancel(grpc::ServerContext* context, const JFJochProtoBuf::Empty* request,
                       JFJochProtoBuf::Empty* response) override;
    grpc::Status Stop(grpc::ServerContext* context, const JFJochProtoBuf::Empty* request,
                      JFJochProtoBuf::JFJochReceiverOutput* response) override;
    grpc::Status GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                             JFJochProtoBuf::ReceiverStatus *response) override;
    grpc::Status SetDataProcessingSettings(grpc::ServerContext *context, const JFJochProtoBuf::DataProcessingSettings *request,
                                           JFJochProtoBuf::Empty *response) override;
    grpc::Status GetPreviewFrame(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                   JFJochProtoBuf::PreviewFrame *response) override;
    grpc::Status GetNetworkConfig(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                    JFJochProtoBuf::JFJochReceiverNetworkConfig *response) override;
};


#endif //JUNGFRAUJOCH_JFJOCHRECEIVERSERVICE_H
