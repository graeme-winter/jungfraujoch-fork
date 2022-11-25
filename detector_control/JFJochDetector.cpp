// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochDetector.h"
#include "../common/JFJochException.h"

grpc::Status JFJochDetector::Start(grpc::ServerContext *context, const JFJochProtoBuf::DetectorInput *request,
                                   JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);

    try {
        detector.Start(*request);
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                  JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);

    try {
        detector.Stop();
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::On(grpc::ServerContext *context, const JFJochProtoBuf::DetectorConfig *request,
                                JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);
    try {
        detector.Configure(*request);
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}


grpc::Status JFJochDetector::Off(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                 JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);
    try {
        detector.Deactivate();
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::Status(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                    JFJochProtoBuf::DetectorStatus *response) {
    try {
        switch(detector.GetState()) {
            case DetectorWrapper::DetectorState::IDLE:
                response->set_status(JFJochProtoBuf::IDLE);
                break;
            case DetectorWrapper::DetectorState::ERROR:
                response->set_status(JFJochProtoBuf::ERROR);
                break;
            case DetectorWrapper::DetectorState::BUSY:
                response->set_status(JFJochProtoBuf::BUSY);
                break;
        }
        response->set_fw_version(detector.GetFirmwareVersion());
        response->set_server_version(detector.GetDetectorServerVersion());

        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::Trigger(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                     JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);
    try {
        detector.Trigger();
        return grpc::Status::OK;
    } catch (JFJochException &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}