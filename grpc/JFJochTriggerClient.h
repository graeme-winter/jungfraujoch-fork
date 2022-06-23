// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHTRIGGERCLIENT_H
#define JUNGFRAUJOCH_JFJOCHTRIGGERCLIENT_H

#include <string>
#include <jfjoch.grpc.pb.h>

class JFJochTriggerClient {
    std::unique_ptr<JFJochProtoBuf::gRPC_JFJochTrigger::Stub> _stub;
public:
    void Connect(const std::string &addr);
    void Trigger();
};


#endif //JUNGFRAUJOCH_JFJOCHTRIGGERCLIENT_H
