// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include "JFJochFrameDeserializer.h"
#include "tinycbor/src/cbor.h"
#include "CborErr.h"

std::string JFJochFrameDeserializer::GetCBORString(CborValue &value) {
    if (!cbor_value_is_text_string(&value))
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "CBOR key must be text");

    char buf[256];
    size_t len = 255;
    cborErr(cbor_value_copy_text_string(&value, buf, &len, nullptr));
    buf[len+1] = 0;
    cborErr(cbor_value_advance(&value));
    return std::string(buf);
}

int64_t JFJochFrameDeserializer::GetCBORInt(CborValue &value) {
    if (!cbor_value_is_integer(&value) )
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Integer expected");
    int64_t tmp;
    cborErr(cbor_value_get_int64(&value, &tmp));
    cborErr(cbor_value_advance(&value));
    return tmp;
}

void JFJochFrameDeserializer::GetCBORBinaryImage(CborValue &value) {
    if (!cbor_value_is_byte_string(&value))
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Byte string expected");
    cborErr(cbor_value_get_string_length(&value, &image_size));

    if (image.size() < image_size + 1)
        image.resize(image_size + 1);

    size_t tmp = image_size + 1;
    cborErr(cbor_value_copy_byte_string(&value, image.data(), &tmp, nullptr));
    if (tmp != image_size)
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Size mismatch");
    cborErr(cbor_value_advance(&value));
}

float JFJochFrameDeserializer::GetCBORFloatAndAdvance(CborValue &value) {
    if (!cbor_value_is_float(&value) )
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Float expected");
    float tmp;
    cborErr(cbor_value_get_float(&value, &tmp));
    cborErr(cbor_value_advance(&value));
    return tmp;
}

void JFJochFrameDeserializer::GetCBORSpots(CborValue &value) {
    if (!cbor_value_is_array(&value))
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "CBOR array expected");
    size_t array_len;
    cborErr(cbor_value_get_array_length(&value, &array_len));
    if (array_len % 3 != 0)
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Spot array has to have elements multiple of 3");

    if (array_len > 0) {
        CborValue valueArr;
        cborErr(cbor_value_enter_container(&value, &valueArr));
        for (int i = 0; i < array_len / 3; i++) {
            SpotToSave s;
            s.x = GetCBORFloatAndAdvance(valueArr);
            s.y = GetCBORFloatAndAdvance(valueArr);
            s.intensity = GetCBORFloatAndAdvance(valueArr);
            spots.push_back(s);
        }
        cborErr(cbor_value_leave_container(&value, &valueArr));
    } else {
        cborErr(cbor_value_advance(&value));
    }
}

void JFJochFrameDeserializer::DecodeType(CborValue &value) {
    auto type_str = GetCBORString(value);
    if (type_str == "start")
        msg_type = Type::START;
    else if (type_str == "end")
        msg_type = Type::END;
    else if (type_str == "image")
        msg_type = Type::IMAGE;
    else
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Unknown message type");
}

bool JFJochFrameDeserializer::ProcessImageMapElement(CborValue &value) {
    if (cbor_value_at_end(&value))
        return false;
    else {
        auto key = GetCBORString(value);

        if (key == "type")
            DecodeType(value);
        else if (key == "image_number")
            image_number = GetCBORInt(value);
        else if (key == "file_number")
            file_number = GetCBORInt(value);
        else if (key == "data")
            GetCBORBinaryImage(value);
        else if (key == "spots")
            GetCBORSpots(value);
        return true;
    }
}

void JFJochFrameDeserializer::Process(const std::vector<uint8_t> &buffer) {
    spots.clear();
    image_size = 0;

    CborParser parser;
    CborValue value;
    cborErr(cbor_parser_init(buffer.data(), buffer.size(), 0, &parser, &value));
    if (cbor_value_is_map(&value)) {
        CborValue map_value;
        cborErr(cbor_value_enter_container(&value, &map_value));
        while (ProcessImageMapElement(map_value));
        cborErr(cbor_value_leave_container(&value, &map_value));
    } else
        throw JFJochException(JFJochExceptionCategory::CBORError,
                              "Serialized frame must be map in top level");
}

const uint8_t *JFJochFrameDeserializer::GetImageData() const {
    return image.data();
}

const size_t JFJochFrameDeserializer::GetImageSize() const {
    return image_size;
}

size_t JFJochFrameDeserializer::GetImageNumber() const {
    return image_number;
}

size_t JFJochFrameDeserializer::GetFileNumber() const {
    return file_number;
}

JFJochFrameDeserializer::Type JFJochFrameDeserializer::GetType() const {
    return msg_type;
}

const std::vector<SpotToSave> &JFJochFrameDeserializer::GetSpots() const {
    return spots;
}