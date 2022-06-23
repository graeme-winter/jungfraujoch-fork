// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FrameTransformationPreview.h"

#include <cstring>

#include "RawToConvertedGeometry.h"
#include "JFJochException.h"

FrameTransformationPreview::FrameTransformationPreview(const DiffractionExperiment &in_experiment,
                                                       const std::vector<uint8_t> &in_mask,
                                                       int16_t *output) :
        experiment(in_experiment), mask(in_mask),
        summation(in_experiment.GetSummationForPreview()),
        standard_output(output) {

    if (standard_output == nullptr)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Default stream output not initialized");

    if ((experiment.GetDetectorMode() == DetectorMode::Conversion) && (summation > 1)) {
        for (int i = 0; i < experiment.GetModulesNum(); i++)
            summation_buffer.emplace_back(RAW_MODULE_SIZE);
    }
}

void FrameTransformationPreview::Pack() {
    if (summation > 1) {
        for (int m = 0; m < experiment.GetModulesNum(); m++) {
            TransferModuleAdjustMultipixels<int16_t, int16_t>(standard_output + experiment.GetPixel0OfModule(m),
                                                              summation_buffer[m].data(),
                                                              (experiment.IsUpsideDown() ? -1 : 1) *
                                                              experiment.GetXPixelsNum(),
                                                              INT16_MIN + 4,
                                                              INT16_MAX - 4);
            for (auto &i: summation_buffer[m])
                i = 0;
        }
    }
    // Apply pixel mask
    for (int i = 0; i < experiment.GetPixelsNum(); i++) {
        if (mask[i] == 0)
            standard_output[i] = INT16_MIN;
    }
}

void FrameTransformationPreview::ProcessPacketNoSummation(const int16_t *input, size_t frame_number,
                                                   uint16_t module_number, uint16_t packet_number, int data_stream) {
    auto output = standard_output
                  + experiment.GetPixel0OfModule(experiment.GetFirstModuleOfDataStream(data_stream) + module_number);

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        TransferPacketNoMultipixel<int16_t, int16_t>(output, input,
                                                     (experiment.IsUpsideDown() ? -1 : 1) *
                                                     experiment.GetXPixelsNum(),
                                                     packet_number);
    }
}

void AddPacketToFramesSum(int16_t *destination, const int16_t *source) {
    for (int i = 0; i < 4 * RAW_MODULE_COLS; i++) {
        int32_t tmp = (int32_t) destination[i] + (int32_t) source[i];
        if ((source[i] < INT16_MIN + 4) || (destination[i] == INT16_MIN) || (tmp < INT16_MIN + 4))
            destination[i] = INT16_MIN;
        else if ((source[i] > INT16_MAX - 4) || (destination[i] == INT16_MAX) || (tmp > INT16_MAX + 4))
            destination[i] = INT16_MAX;
        else
            destination[i] = static_cast<int16_t>(tmp);
    }
}

void FrameTransformationPreview::ProcessPacketSummation(const int16_t *input,
                                                        size_t frame_number,
                                                        uint16_t module_number,
                                                        uint16_t packet_number,
                                                        int data_stream) {
    size_t module_number_abs = experiment.GetFirstModuleOfDataStream(data_stream) + module_number;

    AddPacketToFramesSum(summation_buffer[module_number_abs].data() + RAW_MODULE_COLS * 4 * packet_number,
                   input);
}


void FrameTransformationPreview::ProcessModule(const int16_t *input, size_t frame_number, uint16_t module_number,
                                               int data_stream) {
    if (summation == 1) {
        for (int i = 0; i < 128; i++)
            ProcessPacketNoSummation(input + 4 * RAW_MODULE_COLS * i, frame_number, module_number, i, data_stream);
    } else {
        for (int i = 0; i < 128; i++)
            ProcessPacketSummation(input + 4 * RAW_MODULE_COLS * i, frame_number, module_number, i, data_stream);
    }
}

void FrameTransformationPreview::ProcessPacket(const int16_t *input, size_t frame_number, uint16_t module_number,
                                        uint16_t packet_number, int data_stream) {
    if (summation == 1)
        ProcessPacketNoSummation(input, frame_number, module_number, packet_number, data_stream);
    else
        ProcessPacketSummation(input, frame_number, module_number, packet_number, data_stream);
}