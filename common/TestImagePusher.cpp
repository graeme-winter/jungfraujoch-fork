// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TestImagePusher.h"
#include "../tests/FPGAUnitTest.h"
#include "JFJochCompressor.h"
#include "../compression/JFJochDecompress.h"

TestImagePusher::TestImagePusher(int64_t image_number) {
    image_id = image_number;
}

void TestImagePusher::StartDataCollection(int64_t data_file_count) {
    std::unique_lock<std::mutex> ul(m);

    if (is_running)
        correct_sequence = false;
    else
        is_running = true;
}

void TestImagePusher::EndDataCollection() {
    std::unique_lock<std::mutex> ul(m);

    if (!is_running)
        correct_sequence = false;
    else
        is_running = false;
}

void TestImagePusher::SendData(void *image, size_t image_size, const std::vector<DiffractionSpot> &spots,
                               int64_t image_number) {
    std::unique_lock<std::mutex> ul(m);

    frame_counter++;
    if (image_number == image_id) {
        receiver_generated_image.resize(image_size);
        memcpy(receiver_generated_image.data(), image, image_size);
    }
}

bool TestImagePusher::CheckSequence() const {
    std::unique_lock<std::mutex> ul(m);
    return correct_sequence;
}

const std::vector<uint8_t> &TestImagePusher::GetImage() const {
    std::unique_lock<std::mutex> ul(m);
    return receiver_generated_image;
}

size_t TestImagePusher::GetCounter() const {
    std::unique_lock<std::mutex> ul(m);
    return frame_counter;
}

bool TestImagePusher::CheckImage(const DiffractionExperiment &x, const std::vector<uint16_t> &raw_reference_image,
                                 const JFCalibration &calibration, const JFModuleGainCalibration &gain_calibration,
                                 Logger &logger) {
    bool no_errors = true;

    if (receiver_generated_image.empty()) {
        logger.Error("Image empty");
        no_errors = false;
    } else {
        std::vector<int16_t> decompressed_image;
        // Image decompression
        try {
            JFJochDecompress(decompressed_image, x.GetCompressionAlgorithmEnum(),
                             receiver_generated_image, x.GetPixelsNum());
        } catch (const JFJochException &e) {
            logger.Error(e.what());
            no_errors = false;
        }
        if (no_errors) {
            // Check output

            if (x.GetDetectorMode() == DetectorMode::Conversion) {
                size_t storage_cell = 0;
                if (x.GetStorageCellNumber() > 1)
                    storage_cell = image_id % x.GetStorageCellNumber();

                double result = CheckConversionWithGeomTransform(x, calibration, gain_calibration,
                                                                 raw_reference_image.data(),
                                                                 decompressed_image.data(),
                                                                 storage_cell);
                if (result > 0.5) {
                    logger.Error("Mean conversion error ({}) larger than threshold", result);
                    no_errors = false;
                } else
                    logger.Info("Mean conversion error: {}", result);
            } else if (x.GetDetectorMode() == DetectorMode::Raw) {
                if (memcmp(raw_reference_image.data(), decompressed_image.data(), sizeof(uint16_t) * x.GetPixelsNum()) !=
                    0) {
                    logger.Error("Raw data mismatch");
                    no_errors = false;
                }
            }
        }
    }
    return no_errors;
}