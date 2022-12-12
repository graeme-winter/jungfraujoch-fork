// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_MAXCOMPRESSEDSIZE_H
#define JUNGFRAUJOCH_MAXCOMPRESSEDSIZE_H

#include <cstdint>
#include "CompressionAlgorithmEnum.h"

int64_t MaxCompressedSize(CompressionAlgorithm algorithm, int64_t pixels_number, uint16_t pixel_depth);

#endif //JUNGFRAUJOCH_MAXCOMPRESSEDSIZE_H
