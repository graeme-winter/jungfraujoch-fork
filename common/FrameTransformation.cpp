// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>

#include <bitshuffle/bitshuffle.h>

#include "FrameTransformation.h"
#include "RawToConvertedGeometry.h"
#include "JFJochException.h"

FrameTransformation::FrameTransformation(const DiffractionExperiment &in_experiment, const JungfrauCalibration &calibration,
                                         size_t in_summation, CompressionAlgorithm algorithm) :
        experiment(in_experiment), summation(in_summation),
        pixel_depth(in_summation > 1 ? sizeof(int32_t) : sizeof(int16_t)), compressor(in_experiment),
        compression_algorithm(algorithm) {

    if ((experiment.GetDetectorMode() == DetectorMode::Conversion) && (summation > 1)) {
        for (int i = 0; i < experiment.GetModulesNum(); i++)
            summation_buffer.emplace_back(RAW_MODULE_SIZE);
    }

    if (compression_algorithm != CompressionAlgorithm::None) {
        if (compression_algorithm != experiment.GetCompressionAlgorithm())
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                                  "Combination of compression algorithms not supported by FrameTransformation");
        precompression_buffer.resize(experiment.GetPixelsNum() * pixel_depth);
    }
}

FrameTransformation::FrameTransformation(const DiffractionExperiment &in_experiment,
                                         const JungfrauCalibration &calibration, size_t in_summation) :
        FrameTransformation(in_experiment, calibration, in_summation, in_experiment.GetCompressionAlgorithm()) {}

FrameTransformation::FrameTransformation(const DiffractionExperiment &in_experiment,
                                         const JungfrauCalibration &calibration) :
        FrameTransformation(in_experiment, calibration, in_experiment.GetSummation(),
                            in_experiment.GetCompressionAlgorithm()) {}

FrameTransformation& FrameTransformation::SetOutput(void *output) {
    standard_output = (char *) output;
    return *this;
}

template <class T> void AddToFramesSum(T *destination, const int16_t *source, size_t nlines,
                                       int overload_sum, int underload_sum) {
    for (int i = 0; i < nlines * RAW_MODULE_COLS; i++) {
        if ((source[i] < INT16_MIN + 4) || (destination[i] == underload_sum))
            destination[i] = underload_sum;
        else if ((source[i] > INT16_MAX - 4) || (destination[i] == overload_sum))
            destination[i] = overload_sum;
        else destination[i] += source[i];
    }
}

void FrameTransformation::PackUncompressed() {
    if (summation > 1) {
        for (int m = 0; m < experiment.GetModulesNum(); m++) {
            TransferModuleAdjustMultipixels<int32_t, int32_t>(((int32_t *) GetUncompressedImage()) + experiment.GetPixel0OfModule(m),
                                                              summation_buffer[m].data(),
                                                              (experiment.IsUpsideDown() ? -1 : 1) *
                                                              experiment.GetXPixelsNum(),
                                                              DiffractionExperiment::GetUnderflow(summation) + 1,
                                                              DiffractionExperiment::GetOverflow(summation) - 1);
            for (auto &i: summation_buffer[m])
                i = 0;
        }
    }
}

size_t FrameTransformation::PackStandardOutput() {
    PackUncompressed();
    switch (compression_algorithm) {
        case CompressionAlgorithm::BSHUF_LZ4:
        case CompressionAlgorithm::BSHUF_ZSTD:
            BitShuffleCompressionHeader(standard_output, experiment.GetPixelsNum(),
                                        experiment.GetCompressionBlockSize(),
                                        pixel_depth);
            return compressor.Compress(standard_output+12, precompression_buffer.data(),
                                       experiment.GetPixelsNum(), pixel_depth) + 12;

        case CompressionAlgorithm::None:
            return experiment.GetPixelsNum() * pixel_depth;
        default:
            throw JFJochException(JFJochExceptionCategory::Compression, "Not implemented compression algorithm");
    }
}


void FrameTransformation::ProcessPacketNoSummation(const int16_t *input, size_t frame_number,
                                                   uint16_t module_number, uint16_t packet_number, int data_stream) {
    char *in_output = (compression_algorithm == CompressionAlgorithm::None)
                      ? standard_output : precompression_buffer.data();

    size_t offset = experiment.GetPixel0OfModule(experiment.GetFirstModuleOfDataStream(data_stream)+module_number);
    auto output = ((int16_t *) in_output) + offset;

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        if (experiment.GetMaskChipEdges())
            TransferPacketNoMultipixel<int16_t, int16_t>(output, input,
                                             (experiment.IsUpsideDown() ? -1 : 1) *
                                             experiment.GetXPixelsNum(),
                                             packet_number);
        else
            TransferPacketAdjustMultipixels<int16_t, int16_t>(output, input,
                                                          (experiment.IsUpsideDown() ? -1 : 1) *
                                                          experiment.GetXPixelsNum(),
                                                          packet_number,
                                                          DiffractionExperiment::GetUnderflow(summation) + 1,
                                                          DiffractionExperiment::GetOverflow(summation) - 1);
    }
    else
        memcpy(output + packet_number * 4 * RAW_MODULE_COLS,
               input,
               4 * RAW_MODULE_COLS * sizeof(int16_t));
}

void FrameTransformation::ProcessPacketSummation(const int16_t *input, size_t frame_number, uint16_t module_number,
                                                 uint16_t packet_number, int data_stream) {
    size_t module_number_abs = experiment.GetFirstModuleOfDataStream(data_stream) + module_number;

    AddToFramesSum(summation_buffer[module_number_abs].data() + RAW_MODULE_COLS * 4 * packet_number,
                   input,
                   4,
                   experiment.GetOverflow(),
                   experiment.GetUnderflow());
}

void FrameTransformation::ProcessModule(const int16_t *input, size_t frame_number, uint16_t module_number,
                                        int data_stream) {
    if (standard_output == nullptr)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Default stream output not initialized");

    if (summation == 1) {
        for (int i = 0; i < 128; i++)
            ProcessPacketNoSummation(input + 4 * RAW_MODULE_COLS * i, frame_number, module_number, i, data_stream);
    } else {
        for (int i = 0; i < 128; i++)
            ProcessPacketSummation(input + 4 * RAW_MODULE_COLS * i, frame_number, module_number, i, data_stream);
    }
}

void FrameTransformation::ProcessPacket(const int16_t *input, size_t frame_number, uint16_t module_number,
                                        uint16_t packet_number, int data_stream) {
    if (standard_output == nullptr)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Default stream output not initialized");

    if (summation == 1)
        ProcessPacketNoSummation(input, frame_number, module_number, packet_number, data_stream);
    else
        ProcessPacketSummation(input, frame_number, module_number, packet_number, data_stream);
}

const char *FrameTransformation::GetUncompressedImage() const {
    return (compression_algorithm == CompressionAlgorithm::None) ? standard_output : precompression_buffer.data();
}