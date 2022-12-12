// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHINDEXERCLIENT_H
#define JUNGFRAUJOCH_JFJOCHINDEXERCLIENT_H

#include "jfjoch.grpc.pb.h"
#include "../common/DiffractionExperiment.h"

class JFJochIndexerClient {
    std::unique_ptr<JFJochProtoBuf::gRPC_JFJochIndexer::Stub> _stub;
    std::string zmq_addr;
public:
    void Connect(const std::string& addr);
    JFJochIndexerClient &ZMQReceiverAddr(const std::string& receiver_addr);
    void Start(const DiffractionExperiment &x);
    JFJochProtoBuf::IndexerOutput Stop();
    JFJochProtoBuf::IndexerStatus GetStatus();
    JFJochProtoBuf::IndexerDataProcessingPlots GetPlots();
};


#endif //JUNGFRAUJOCH_JFJOCHINDEXERCLIENT_H
