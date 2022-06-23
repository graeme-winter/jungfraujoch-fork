// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HDF5DATAFILE_H
#define HDF5DATAFILE_H

#include <queue>
#include <string>
#include <chrono>
#include "HDF5Objects.h"
#include "../common/DiffractionExperiment.h"
#include "../common/ZMQWrappers.h"

class HDF5DataFile {
    std::string filename;
    std::unique_ptr<HDF5File> data_file = nullptr;
    std::unique_ptr<HDF5DataSet> data_set = nullptr;

    const DiffractionExperiment &experiment;
    size_t image0;
    size_t ypixel;
    size_t max_image_number;
public:
    HDF5DataFile(const DiffractionExperiment &in_experiment, uint32_t file_id);
    ~HDF5DataFile();
    void Write(const void *data, size_t data_size, size_t image_number);
};

#endif //HDF5DATAFILE_H
