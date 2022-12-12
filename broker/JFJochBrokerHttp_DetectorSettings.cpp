// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerHttp.h"
#include "JFJochBrokerHttp_ProtoBuf_JSON.h"

void JFJochBrokerHttp::GetDetectorSettings(const Pistache::Rest::Request &request,
                                           Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = grpcToJson(state_machine.GetDetectorSettings());
        response.send(Pistache::Http::Code::Ok, tmp, MIME(Application, Json));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::PutDetectorSettings(const Pistache::Rest::Request &request,
                                           Pistache::Http::ResponseWriter response) {
    try {
        state_machine.SetDetectorSettings(FromWire<JFJochProtoBuf::DetectorSettings>(request));

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}