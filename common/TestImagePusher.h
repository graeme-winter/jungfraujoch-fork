// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_TESTIMAGEPUSHER_H
#define JUNGFRAUJOCH_TESTIMAGEPUSHER_H

#include <mutex>

#include "ImagePusher.h"
#include "Logger.h"
#include "DiffractionExperiment.h"
#include "JFCalibration.h"
#include "JFModuleGainCalibration.h"

class TestImagePusher : public ImagePusher {
    mutable std::mutex m;
    std::vector<uint8_t> receiver_generated_image;
    int64_t image_id;
    bool correct_sequence = true;
    bool is_running = false;
    size_t frame_counter = 0;
public:
    explicit TestImagePusher(int64_t image_number);
    void StartDataCollection(int64_t file_count) override;
    void EndDataCollection() override;
    void SendData(void *image, size_t image_size, const std::vector<DiffractionSpot> &spots,
                  int64_t image_number) override;
    bool CheckImage(const DiffractionExperiment &x,
                         const std::vector<uint16_t> &raw_reference_image,
                         const JFCalibration &calibration,
                         const JFModuleGainCalibration &gain_calibration,
                         Logger &logger);
    [[nodiscard]] bool CheckSequence() const;
    [[nodiscard]] const std::vector<uint8_t> &GetImage() const;
    [[nodiscard]] size_t GetCounter() const;
};


#endif //JUNGFRAUJOCH_TESTIMAGEPUSHER_H
