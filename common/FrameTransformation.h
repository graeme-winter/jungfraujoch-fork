// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_FRAMETRANSFORMATION_H
#define JUNGFRAUJOCH_FRAMETRANSFORMATION_H

#include "DiffractionExperiment.h"
#include "JFJochCompressor.h"

class FrameTransformation {
    const DiffractionExperiment& experiment;
    JFJochBitShuffleCompressor compressor;

    CompressionAlgorithm compression_algorithm;

    std::vector<std::vector<int32_t> > summation_buffer;
    std::vector<char> precompression_buffer;
    std::vector<int16_t> image16bit;

    char *standard_output = nullptr;

    const size_t summation;
    const size_t pixel_depth;

    void ProcessPacketNoSummation(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);
    void ProcessPacketSummation(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);
public:
    FrameTransformation(const DiffractionExperiment &experiment);
    FrameTransformation& SetOutput(void *output);
    void ProcessModule(const int16_t *input, size_t frame_number, uint16_t module_number, int data_stream);
    void ProcessPacket(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);

    size_t PackStandardOutput(); // transfer summed image to converted coordinates, clear summation buffer, compress
    void PackUncompressed(); // transfer summed image to converted coordinates, clear summation buffer, no compression

    int16_t *GetPreview16BitImage();
};

#endif //JUNGFRAUJOCH_FRAMETRANSFORMATION_H
