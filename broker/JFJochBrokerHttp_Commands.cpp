// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerHttp.h"
#include "JFJochBrokerHttp_ProtoBuf_JSON.h"

void JFJochBrokerHttp::Start(const Pistache::Rest::Request &request,
                             Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Start(FromWire<JFJochProtoBuf::DatasetSettings>(request));

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Stop(const Pistache::Rest::Request &request,
                            Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Stop();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Pedestal(const Pistache::Rest::Request &request,
                                Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Pedestal();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Initialize(const Pistache::Rest::Request &request,
                                  Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Initialize();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Cancel(const Pistache::Rest::Request &request,
                              Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Cancel();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Deactivate(const Pistache::Rest::Request &request,
                                  Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Deactivate();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::Trigger(const Pistache::Rest::Request &request,
                                  Pistache::Http::ResponseWriter response) {
    try {
        state_machine.Trigger();

        response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
        response.send(Pistache::Http::Code::Ok);
    } catch (const JFJochException& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}