// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_GRPCTOJSON_H
#define JUNGFRAUJOCH_GRPCTOJSON_H

#include <google/protobuf/util/json_util.h>

#include "../common/JFJochException.h"

inline std::string grpcToJson(const google::protobuf::Message &message) {
    google::protobuf::util::JsonPrintOptions opts;
    opts.always_print_primitive_fields = true;
    opts.add_whitespace = true;
    opts.always_print_enums_as_ints = false;
    opts.preserve_proto_field_names = true;

    std::string s;
    auto status = google::protobuf::util::MessageToJsonString(message, &s, opts);
    if (!status.ok())
        throw JFJochException(JFJochExceptionCategory::JSON, "Error in generating JSON from ProtoBuf: " + status.message().ToString());

    return s;
}
#endif //JUNGFRAUJOCH_GRPCTOJSON_H
