// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../common/grpcToJson.h"
#include "../common/jsonToGrpc.h"

#include <google/protobuf/util/json_util.h>
#include <pistache/http.h>

#include "../common/JFJochException.h"

template <class T>
T FromWire(const Pistache::Rest::Request &request) {
    static_assert(std::is_base_of<google::protobuf::Message, T>::value,
            "Template class T not derived from google::protobuf::Message");

    if (request.body().empty())
        throw JFJochException(JFJochExceptionCategory::JSON,
                              "Empty message cannot be parsed");

    // Option to read ProtoBuf structure directly
    auto content_type = request.headers().tryGet<Pistache::Http::Header::ContentType>();
    auto pbuf_mime = Pistache::Http::Mime::MediaType::fromString("application/x-protobuf");
    if ((content_type != nullptr) && (content_type->mime() == pbuf_mime)) {
        T output;
        output.ParseFromString(request.body());
        return output;
    } else
        return jsonToGrpc<T>(request.body());
}

