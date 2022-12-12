// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerHttp.h"
#include "JFJochBrokerHttp_ProtoBuf_JSON.h"

void JFJochBrokerHttp::GetDataProcessingSettings(const Pistache::Rest::Request &request,
                                                 Pistache::Http::ResponseWriter response) {

    try {
        auto tmp = grpcToJson(state_machine.GetDataProcessingSettings());

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok, tmp, MIME(Application, Json));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::PutDataProcessingSettings(const Pistache::Rest::Request &request,
                                                 Pistache::Http::ResponseWriter response) {
    try {
        state_machine.SetDataProcessingSettings(FromWire<JFJochProtoBuf::DataProcessingSettings>(request));

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const std::exception& e) {
        logger.Error(e.what());
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetDataProcessingPlots(const Pistache::Rest::Request &request,
                                              Pistache::Http::ResponseWriter response) {
    try {
        auto tmp = grpcToJson(state_machine.GetPlots());

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok, tmp, MIME(Application, Json));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}
