// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HDF5WRITER_H
#define JUNGFRAUJOCH_HDF5WRITER_H

#include <numeric>

#include "../common/DiffractionExperiment.h"
#include "../common/JungfrauCalibration.h"
#include "HDF5DataFile.h"
#include "HDF5MasterFile.h"

class HDF5Writer {
    std::vector<std::unique_ptr<HDF5DataFile> > files;
    DiffractionExperiment experiment;

    std::vector<size_t> images_remaining;
    size_t images_per_file;
    size_t total_images;
    static void ErrorIfFileExists(const std::string &path);
    static void MakeDirectory(const std::string &path);
public:
    explicit HDF5Writer(const DiffractionExperiment &in_experiment);
    void WriteFromStream(const uint8_t *data, int64_t data_size);
    void Write(const void *data, size_t data_size, size_t image_number);
    size_t GetRemainingImageCount() const;
};

#endif //JUNGFRAUJOCH_HDF5WRITER_H
