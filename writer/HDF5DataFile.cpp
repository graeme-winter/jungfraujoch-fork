// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include "HDF5DataFile.h"
#include "HDF5NXmx.h"

HDF5DataFile::HDF5DataFile(const DiffractionExperiment &in_experiment, uint32_t file_id) :
experiment(in_experiment) {
    image0 = file_id * experiment.GetImagesPerFile();
    ypixel = in_experiment.GetYPixelsNum();
    filename = in_experiment.GenerateDataFilename(file_id);
    max_image_number = 0;
}

void HDF5DataFile::Write(const void *data, size_t data_size, size_t image_number) {
    std::lock_guard<std::mutex> lock(hdf5_mutex);
    if (!data_file) {
        data_file = std::make_unique<HDF5File>(filename + ".tmp", true,
                                               false, false);
        HDF5Group(*data_file, "/entry").NXClass("NXentry");
        HDF5Group(*data_file, "/entry/data").NXClass("NXdata");

        data_set = HDF5Data::Data(data_file.get(), experiment, image0, experiment.GetImagesPerFile(), ypixel);
    }
    if (image_number > max_image_number) {
        max_image_number = image_number;
        data_set->SetExtent({max_image_number+1, ypixel, (hsize_t) experiment.GetXPixelsNum()});
    }
    data_set->WriteDirectChunk(data, data_size, {image_number, 0, 0});
}

HDF5DataFile::~HDF5DataFile() {
    data_set.reset();
    data_file.reset();

    std::string tmp_filename = filename + ".tmp";
    rename(tmp_filename.c_str(), filename.c_str());
}