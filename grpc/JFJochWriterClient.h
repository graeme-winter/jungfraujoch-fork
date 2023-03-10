// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHWRITERCLIENT_H
#define JUNGFRAUJOCH_JFJOCHWRITERCLIENT_H

#include <string>
#include <jfjoch.grpc.pb.h>

#include "../common/DiffractionExperiment.h"

class JFJochWriterClient {
    std::unique_ptr<JFJochProtoBuf::gRPC_JFJochWriter::Stub> _stub;
public:
    void Connect(const std::string &addr);
    void Start(const DiffractionExperiment &experiment, const std::string zmq_push_addr);
    JFJochProtoBuf::WriterOutput Stop();
    void WriteMasterFile(JFJochProtoBuf::WriterMetadataInput &request);
    void Abort();
};


#endif //JUNGFRAUJOCH_JFJOCHWRITERCLIENT_H
