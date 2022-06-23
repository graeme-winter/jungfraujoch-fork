// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochWriterGroupClient.h"

void JFJochWriterGroupClient::AddClient(const std::string &addr) {
    clients.emplace_back();
    clients[clients.size() - 1].Connect(addr);
}

void JFJochWriterGroupClient::Start(const DiffractionExperiment &experiment) {
    for (auto &i: clients)
        i.Start(experiment);
}

void JFJochWriterGroupClient::Stop() {
    for (auto &i: clients)
        i.Stop();
}

void JFJochWriterGroupClient::WriteMasterFile(const JFJochProtoBuf::JFJochReceiverOutput &request) {
    if (!clients.empty())
        clients[0].WriteMasterFile(request);
}

void JFJochWriterGroupClient::Abort() {
    for (auto &i: clients)
        i.Abort();
}
