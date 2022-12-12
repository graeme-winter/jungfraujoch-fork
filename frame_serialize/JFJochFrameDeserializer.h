// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHFRAMEDESERIALIZER_H
#define JUNGFRAUJOCH_JFJOCHFRAMEDESERIALIZER_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "../common/SpotToSave.h"
#include "tinycbor/src/cbor.h"

class JFJochFrameDeserializer {
public:
    enum class Type {START, END, IMAGE};
private:
    Type msg_type;
    std::vector<SpotToSave> spots;
    int64_t image_number;

    std::vector<uint8_t> image;
    size_t image_size;

    void DecodeType(CborValue &value);
    std::string GetCBORString(CborValue &value);
    int64_t GetCBORInt(CborValue &value);
    float GetCBORFloatAndAdvance(CborValue &value);

    void GetCBORBinaryImage(CborValue &value);
    void GetCBORSpots(CborValue &value);
    bool ProcessImageMapElement(CborValue &value);
public:
    void Process(const std::vector<uint8_t>& buffer);
    Type GetType() const;
    const std::vector<SpotToSave>& GetSpots() const;
    int64_t GetImageNumber() const;
    const uint8_t* GetImageData() const;
    const size_t GetImageSize() const;
};


#endif //JUNGFRAUJOCH_JFJOCHFRAMEDESERIALIZER_H
