// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochWriterGroupClient.h"

void JFJochWriterGroupClient::AddClient(const std::string &addr) {
    clients.emplace_back();
    clients[clients.size() - 1].Connect(addr);
}

void JFJochWriterGroupClient::Start(const DiffractionExperiment &experiment, const std::vector<std::string> &zmq_push_addr) {
    for (int i = 0; i < std::min(zmq_push_addr.size(), clients.size()); i++)
        clients[i].Start(experiment, zmq_push_addr[i]);
}

std::vector<JFJochProtoBuf::WriterOutput> JFJochWriterGroupClient::Stop() {
    std::vector<JFJochProtoBuf::WriterOutput> ret;
    for (auto &i: clients)
        ret.push_back(i.Stop());
    return ret;
}

void JFJochWriterGroupClient::WriteMasterFile(JFJochProtoBuf::WriterMetadataInput &request) {
    if (!clients.empty())
        clients[0].WriteMasterFile(request);
}

void JFJochWriterGroupClient::Abort() {
    for (auto &i: clients)
        i.Abort();
}
