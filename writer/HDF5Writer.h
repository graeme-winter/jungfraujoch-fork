// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HDF5WRITER_H
#define JUNGFRAUJOCH_HDF5WRITER_H

#include <numeric>

#include "HDF5DataFile.h"
#include "HDF5MasterFile.h"

class HDF5Writer {
    std::vector<std::unique_ptr<HDF5DataFile> > files;
    std::vector<size_t> images_remaining;
    size_t total_images;
    static void ErrorIfFileExists(const std::string &path);
public:
    explicit HDF5Writer(const JFJochProtoBuf::WriterInput &request);
    void Write(const void *data, size_t data_size, const std::vector<SpotToSave>& spots,
               int64_t file_number, int64_t image_number_in_file);
    size_t GetRemainingImageCount() const;
};

#endif //JUNGFRAUJOCH_HDF5WRITER_H
