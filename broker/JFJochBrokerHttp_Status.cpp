// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerHttp.h"
#include "JFJochBrokerHttp_ProtoBuf_JSON.h"

void JFJochBrokerHttp::GetStatus(const Pistache::Rest::Request &request,
                                 Pistache::Http::ResponseWriter response) {
    try {
       const std::string s = grpcToJson(state_machine.GetStatus());
        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok, s, MIME(Application, Json));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetCalibrationStatistics(const Pistache::Rest::Request &request,
                                                Pistache::Http::ResponseWriter response) {
    try {
        const std::string s = grpcToJson(state_machine.GetCalibrationStatistics());
        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok, s, MIME(Application, Json));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetMeasurementStatistics(const Pistache::Rest::Request &request,
                                                    Pistache::Http::ResponseWriter response) {
    try {
        auto statistics = state_machine.GetMeasurementStatistics();
        if (!statistics.has_value())
            response.send(Pistache::Http::Code::Not_Found);
        else {
            const std::string s = grpcToJson(statistics.value());
            response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
            response.send(Pistache::Http::Code::Ok, s, MIME(Application, Json));
        }
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetBrokerStatusProtoBuf(const Pistache::Rest::Request &request,
                             Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = state_machine.GetStatus();
        response.send(Pistache::Http::Code::Ok, tmp.SerializeAsString(),
                      Pistache::Http::Mime::MediaType::fromString("application/x-protobuf"));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}
