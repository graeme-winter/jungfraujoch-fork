// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JSONTOGRPC_H
#define JUNGFRAUJOCH_JSONTOGRPC_H

#include <google/protobuf/util/json_util.h>

#include "../common/JFJochException.h"

template<class T>
T jsonToGrpc(const std::string& json) {
    T output;

    google::protobuf::util::JsonParseOptions opts;
    opts.case_insensitive_enum_parsing = false;
    opts.ignore_unknown_fields = false;

    auto status = google::protobuf::util::JsonStringToMessage(json, &output, opts);
    if (!status.ok())
        throw JFJochException(JFJochExceptionCategory::JSON, "Error in generating ProtoBuf from JSON: " + status.message().ToString());
    return output;
}

#endif //JUNGFRAUJOCH_JSONTOGRPC_H
