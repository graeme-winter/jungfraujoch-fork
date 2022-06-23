// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_FRAMETRANSFORMATIONPREVIEW_H
#define JUNGFRAUJOCH_FRAMETRANSFORMATIONPREVIEW_H

#include "DiffractionExperiment.h"
#include "JFJochCompressor.h"
#include "JungfrauCalibration.h"

class FrameTransformationPreview {
    const DiffractionExperiment& experiment;

    std::vector<std::vector<int16_t> > summation_buffer;
    std::vector<char> precompression_buffer;
    const std::vector<uint8_t> &mask;

    int16_t *standard_output;

    const int64_t summation;

    void ProcessPacketNoSummation(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);
    void ProcessPacketSummation(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);
public:
    FrameTransformationPreview(const DiffractionExperiment &experiment, const std::vector<uint8_t> &mask,
                               int16_t *output);
    void ProcessModule(const int16_t *input, size_t frame_number, uint16_t module_number, int data_stream);
    void ProcessPacket(const int16_t *input, size_t frame_number, uint16_t module_number, uint16_t packet_number, int data_stream);

    void Pack(); // transfer summed image to converted coordinates, clear summation buffer, no compression
};


#endif //JUNGFRAUJOCH_FRAMETRANSFORMATIONPREVIEW_H
