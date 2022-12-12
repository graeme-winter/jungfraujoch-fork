// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochFrameSerializer.h"
#include "tinycbor/src/cbor.h"
#include "CborErr.h"

JFJochFrameSerializer::JFJochFrameSerializer(size_t buffer_size) : buffer(buffer_size) {
}

const std::vector<uint8_t> &JFJochFrameSerializer::GetBuffer() const {
    return buffer;
}

size_t JFJochFrameSerializer::SerializeSequenceStart() {
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);
    cborErr(cbor_encoder_create_map(&encoder, &mapEncoder, 1));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "type"));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "start"));
    cborErr(cbor_encoder_close_container(&encoder, &mapEncoder));
    return cbor_encoder_get_buffer_size(&encoder, buffer.data());
}

size_t JFJochFrameSerializer::SerializeSequenceEnd() {
    CborEncoder encoder, mapEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);
    cborErr(cbor_encoder_create_map(&encoder, &mapEncoder, 1));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "type"));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "end"));
    cborErr(cbor_encoder_close_container(&encoder, &mapEncoder));
    return cbor_encoder_get_buffer_size(&encoder, buffer.data());
}

size_t JFJochFrameSerializer::SerializeImage(void *image, size_t image_size,
                                             const int64_t image_number,
                    const std::vector<SpotToSave>& spots) {
    CborEncoder encoder, mapEncoder, arrayEncoder;
    cbor_encoder_init(&encoder, buffer.data(), buffer.size(), 0);
    cborErr(cbor_encoder_create_map(&encoder, &mapEncoder, 4));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "type"));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "image"));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "data"));
    cborErr(cbor_encode_byte_string(&mapEncoder, (uint8_t *) image, image_size));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "image_number"));
    cborErr(cbor_encode_int(&mapEncoder, image_number));
    cborErr(cbor_encode_text_stringz(&mapEncoder, "spots"));
    cborErr(cbor_encoder_create_array(&mapEncoder, &arrayEncoder, spots.size() * 3));
    for (int i = 0; i < spots.size(); i++) {
        cborErr(cbor_encode_float(&arrayEncoder, spots[i].x));
        cborErr(cbor_encode_float(&arrayEncoder, spots[i].y));
        cborErr(cbor_encode_float(&arrayEncoder, spots[i].intensity));
    }
    cborErr(cbor_encoder_close_container(&mapEncoder, &arrayEncoder));
    cborErr(cbor_encoder_close_container(&encoder, &mapEncoder));
    return cbor_encoder_get_buffer_size(&encoder, buffer.data());
}
