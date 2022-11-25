// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>

#include <bitshuffle/bitshuffle.h>

#include "FrameTransformation.h"
#include "RawToConvertedGeometry.h"
#include "JFJochException.h"

FrameTransformation::FrameTransformation(const DiffractionExperiment &in_experiment) :
        experiment(in_experiment), summation(experiment.GetSummation()),
        pixel_depth(experiment.GetPixelDepth()), compressor(in_experiment.GetCompressionAlgorithmEnum(),
                                                            in_experiment.GetCompressionBlockSize(),
                                                            in_experiment.GetCompressionLevel()),
        compression_algorithm(in_experiment.GetCompressionAlgorithmEnum()),
        line_shift((experiment.IsUpsideDown() ? -1 : 1) * experiment.GetXPixelsNum()) {

    if ((experiment.GetDetectorMode() == DetectorMode::Conversion) && (summation > 1)) {
        for (int i = 0; i < experiment.GetModulesNum(); i++)
            summation_buffer.emplace_back(RAW_MODULE_SIZE);
    }

    precompression_buffer.resize(experiment.GetPixelsNum() * pixel_depth);
    if (pixel_depth == 4)
        image16bit.resize(experiment.GetPixelsNum(), 0);
}

FrameTransformation& FrameTransformation::SetOutput(void *output) {
    standard_output = (char *) output;
    return *this;
}

template <class Td> void AddToFramesSum(Td *destination, const int16_t *source, int underload_sum, int overload_sum) {
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if ((source[i] < INT16_MIN + 4) || (destination[i] == underload_sum))
            destination[i] = underload_sum;
        else if ((source[i] > INT16_MAX - 4) || (destination[i] == overload_sum))
            destination[i] = overload_sum;
        else destination[i] += source[i];
    }
}


template <class Td> void AddToFramesSumUnsigned(Td *destination, const int16_t *source, int overload_sum) {
    auto source_unsigned = reinterpret_cast<const uint16_t *>(source);
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if ((source_unsigned[i] > INT16_MAX - 4) || (destination[i] == overload_sum))
            destination[i] = overload_sum;
        else destination[i] += source_unsigned[i];
    }
}

void FrameTransformation::PackSummation() {
    for (int m = 0; m < experiment.GetModulesNum(); m++) {
        void *output = precompression_buffer.data() + sizeof(int32_t) * experiment.GetPixel0OfModule(m);

        if (experiment.GetDetectorType() == JFJochProtoBuf::EIGER)
            TransferModuleEIGER((uint32_t *) output,
                                (uint32_t *) summation_buffer[m].data(),
                                line_shift);
        else
            TransferModuleAdjustMultipixels((int32_t *) output,
                                            (int32_t *) summation_buffer[m].data(),
                                            line_shift,
                                            static_cast<int32_t>(experiment.GetUnderflow()),
                                            static_cast<int32_t>(experiment.GetOverflow()));
        for (auto &i: summation_buffer[m])
            i = 0;
    }

    if (pixel_depth == 4) {
        // Generate 16-bit preview image
        auto arr = (int32_t *) precompression_buffer.data();

        for (int pxl = 0; pxl < experiment.GetPixelsNum(); pxl++) {
            if (arr[pxl] >= INT16_MAX)
                image16bit[pxl] = INT16_MAX;
            else if (arr[pxl] <= INT16_MIN)
                image16bit[pxl] = INT16_MIN;
            else
                image16bit[pxl] = static_cast<int16_t>(arr[pxl]);
        }
    }
}

size_t FrameTransformation::PackStandardOutput() {
    if (summation > 1)
        PackSummation();
    switch (compression_algorithm) {
        case CompressionAlgorithm::BSHUF_LZ4:
        case CompressionAlgorithm::BSHUF_ZSTD:
            return compressor.Compress(standard_output, precompression_buffer.data(),
                                       experiment.GetPixelsNum(), pixel_depth);

        case CompressionAlgorithm::NO_COMPRESSION:
            memcpy(standard_output, precompression_buffer.data(), experiment.GetPixelsNum() * pixel_depth);
            return experiment.GetPixelsNum() * pixel_depth;
        default:
            throw JFJochException(JFJochExceptionCategory::Compression, "Not implemented compression algorithm");
    }
}

void FrameTransformation::ProcessModule(const int16_t *input, size_t frame_number, uint16_t module_number,
                                        int data_stream) {
    if (standard_output == nullptr)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Default stream output not initialized");

    size_t module_number_abs = experiment.GetFirstModuleOfDataStream(data_stream) + module_number;

    if (summation == 1) {
        auto output = ((int16_t *) precompression_buffer.data())
                + experiment.GetPixel0OfModule(module_number_abs);

        if (experiment.GetDetectorMode() != DetectorMode::Conversion)
            memcpy(output, input, RAW_MODULE_SIZE * experiment.GetPixelDepth());
        else {
            switch (experiment.GetDetectorType()) {
                case JFJochProtoBuf::EIGER:
                    TransferModuleEIGER((uint16_t *) output, (uint16_t *) input, line_shift);
                    break;
                default:
                case JFJochProtoBuf::JUNGFRAU:
                    TransferModuleAdjustMultipixels((int16_t *) output, (int16_t *) input, line_shift,
                                                        static_cast<int16_t>(experiment.GetUnderflow()),
                                                        static_cast<int16_t>(experiment.GetOverflow()));
                    break;
            }
        }
    } else {
        if (experiment.GetDetectorType() == JFJochProtoBuf::EIGER)
            AddToFramesSumUnsigned(summation_buffer[module_number_abs].data(),
                                   input,experiment.GetOverflow());
        else
            AddToFramesSum(summation_buffer[module_number_abs].data(),
                           input,experiment.GetUnderflow(), experiment.GetOverflow());
    }
}

int16_t *FrameTransformation::GetPreview16BitImage() {
    if (pixel_depth == 2)
        return (int16_t *) precompression_buffer.data();
    else
        return image16bit.data();
}

