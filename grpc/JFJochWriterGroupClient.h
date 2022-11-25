// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHWRITERGROUPCLIENT_H
#define JUNGFRAUJOCH_JFJOCHWRITERGROUPCLIENT_H

#include <vector>
#include "JFJochWriterClient.h"

class JFJochWriterGroupClient {
    std::vector<JFJochWriterClient> clients;
public:
    void AddClient(const std::string &addr);
    void Start(const DiffractionExperiment &experiment, const std::vector<std::string> &zmq_push_addr);
    std::vector<JFJochProtoBuf::WriterOutput> Stop();
    void WriteMasterFile(JFJochProtoBuf::WriterMetadataInput &request);
    void Abort();
};


#endif //JUNGFRAUJOCH_JFJOCHWRITERGROUPCLIENT_H
