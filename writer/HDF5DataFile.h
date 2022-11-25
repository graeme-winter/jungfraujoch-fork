// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HDF5DATAFILE_H
#define HDF5DATAFILE_H

#include <queue>
#include <string>
#include <chrono>

#include "HDF5Objects.h"
#include "../common/SpotToSave.h"

class HDF5DataFile {
    std::string filename;
    std::string master_filename;

    std::unique_ptr<HDF5File> data_file = nullptr;
    std::unique_ptr<HDF5DataSet> data_set = nullptr;

    size_t nimages;
    size_t image0;
    size_t xpixel;
    size_t ypixel;
    size_t max_image_number;
    HDF5Dcpl dcpl;
    HDF5DataType data_type;
    std::string image_units;

    std::mutex spot_mutex;
    std::vector<float> spot_x;
    std::vector<float> spot_y;
    std::vector<float> spot_intensity;
    std::vector<uint32_t> spot_count;
    const size_t max_spots;
public:
    HDF5DataFile(const std::string& name, int64_t image0, int64_t nimages, int64_t width, int64_t height, int64_t pixel_depth_byte,
                 bool is_signed, CompressionAlgorithm compression = CompressionAlgorithm::BSHUF_LZ4, int64_t compression_block_size = 0, size_t max_spots = 0);
    ~HDF5DataFile();
    void Write(const void *data, size_t data_size, size_t image_number);
    void WriteSpots(size_t image_number, const std::vector<SpotToSave> & spots);
};

#endif //HDF5DATAFILE_H
