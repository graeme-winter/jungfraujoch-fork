// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiverService.h"

JFJochReceiverService::JFJochReceiverService(std::vector<AcquisitionDevice *> &open_capi_device,
                                             Logger &in_logger, ImagePusher &pusher) :
        logger(in_logger), aq_devices(open_capi_device),
        image_pusher(pusher), data_processing_settings(DiffractionExperiment::DefaultDataProcessingSettings()) {}

grpc::Status JFJochReceiverService::Start(grpc::ServerContext *context, const JFJochProtoBuf::ReceiverInput *request,
                                          JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul_state(state_mutex);
    if (state != ReceiverState::Idle)
        return {grpc::StatusCode::ABORTED, "Receiver not idle, cannot start"};

    try {
        // Thanks to properties of unique_ptr, starting new measurement will call destructor of JFJochReceiver, which will
        // ensure that everything was rolled back
        // But it is important to do it in correct order - first abort old receiver, close it, than start new one
        receiver.reset();
        receiver = std::make_unique<JFJochReceiver>(*request, aq_devices, image_pusher,
                                                    logger, nthreads,
                                                    preview_publisher, spot_publisher);

        receiver->SetDataProcessingSettings(data_processing_settings);
        measurement = std::async(std::launch::async, &JFJochReceiverService::FinalizeMeasurement, this);
        state = ReceiverState::Running;

        return grpc::Status::OK;
    } catch (const JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }

}

void JFJochReceiverService::FinalizeMeasurement() {
    receiver->StopReceiver();
    {
        std::unique_lock<std::mutex> ul(state_mutex);
        state = ReceiverState::Idle;
        measurement_done.notify_all();
    }
}

grpc::Status JFJochReceiverService::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                         JFJochProtoBuf::ReceiverOutput *response) {
    std::unique_lock<std::mutex> ul(state_mutex);

    measurement_done.wait(ul, [this] { return (state != ReceiverState::Running);});

    if (state == ReceiverState::Idle) {
        try {
            if (measurement.valid())
                measurement.get();
        } catch (JFJochException &e) {
            logger.ErrorException(e);
            return {grpc::StatusCode::ABORTED, e.what()};
        }

        if (!receiver) {
            logger.Warning("Request via gRPC, while receiver not running");
            return {grpc::StatusCode::ABORTED, "Receiver not running"};
        } else {
            receiver->GetStatistics(*response);
            return grpc::Status::OK;
        }
    } else
        return {grpc::StatusCode::ABORTED, "Strange state"};
}

grpc::Status JFJochReceiverService::Cancel(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                          JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(state_mutex);
    if (state == ReceiverState::Idle)
        return {grpc::StatusCode::ABORTED, "Receiver not running"};
    else {
        receiver->Cancel();
        return grpc::Status::OK;
    }
}

grpc::Status JFJochReceiverService::Abort(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                          JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(state_mutex);
    if (state == ReceiverState::Idle)
        return {grpc::StatusCode::ABORTED, "Receiver not running"};
    else {
        receiver->Cancel();
        return grpc::Status::OK;
    }
}

JFJochReceiverService& JFJochReceiverService::NumThreads(int64_t input) {
    if (nthreads <= 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Thread number must be above zero");
    nthreads = input;
    return *this;
}

JFJochReceiverService& JFJochReceiverService::PreviewPublisher(ZMQPreviewPublisher *in_preview_writer) {
    preview_publisher = in_preview_writer;
    return *this;
}

JFJochReceiverService &JFJochReceiverService::SpotPublisher(ZMQSpotPublisher *in_spot_writer) {
    spot_publisher = in_spot_writer;
    return *this;
}

grpc::Status JFJochReceiverService::GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                JFJochProtoBuf::ReceiverStatus *response) {
    // FPGA status can be polled outside of the state mutex
    for (auto &i: aq_devices)
        response->mutable_fpga_status()->Add(i->GetStatus());

    {
        // Need to hold mutex, as receiver might not exist here, if state is idle
        std::unique_lock<std::mutex> ul(state_mutex);

        if (state == ReceiverState::Running)
            response->set_progress(receiver->GetProgress());
    }
    return grpc::Status::OK;
}

grpc::Status
JFJochReceiverService::GetDataProcessingPlots(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                              JFJochProtoBuf::ReceiverDataProcessingPlots *response) {
    // Need to hold mutex, as receiver might not exist here, if state is idle
    std::unique_lock<std::mutex> ul(state_mutex);
    if (receiver)
        *response = receiver->GetPlots();
    return grpc::Status::OK;
}

grpc::Status JFJochReceiverService::SetDataProcessingSettings(grpc::ServerContext *context,
                                                              const JFJochProtoBuf::DataProcessingSettings *request,
                                                              JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(state_mutex);
    data_processing_settings = *request;
    if (state != ReceiverState::Idle)
        receiver->SetDataProcessingSettings(*request);

    return grpc::Status::OK;
}

grpc::Status JFJochReceiverService::GetPreviewFrame(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                    JFJochProtoBuf::PreviewFrame *response) {
    if (preview_publisher != nullptr) {
        *response = preview_publisher->GetPreviewImage();
        return grpc::Status::OK;
    }
    return {grpc::StatusCode::NOT_FOUND, "Preview publisher not running"};
}

grpc::Status JFJochReceiverService::GetNetworkConfig(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                     JFJochProtoBuf::ReceiverNetworkConfig *response) {
    for (const auto &aq: aq_devices)
        response->add_fpga_mac_addr(aq->GetMACAddress());
    return grpc::Status::OK;
}