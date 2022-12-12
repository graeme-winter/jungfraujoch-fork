// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>

#include "HDF5DataFile.h"
#include "../compression/JFJochCompressor.h"

HDF5DataFile::HDF5DataFile(const std::string &in_filename, int64_t width, int64_t height,
                           int64_t pixel_depth_byte, bool is_signed, CompressionAlgorithm compression,
                           size_t in_max_spots) : data_type(pixel_depth_byte, is_signed),
                           max_spots(in_max_spots) {
    max_image_number = 0;

    if (max_spots > 256)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Max spot count must be less than 256 for performance reasons");

    filename = in_filename;

    xpixel = width;
    ypixel = height;

    dcpl.SetCompression(compression, pixel_depth_byte, JFJochBitShuffleCompressor::DefaultBlockSize);
    dcpl.SetChunking( {1, ypixel, xpixel});
}

HDF5DataFile::~HDF5DataFile() {
    data_set.reset();

    if (!spot_count.empty()) {
        HDF5Group group(*data_file, "/entry/result");
        group.NXClass("NXcollection");
        std::vector<hsize_t> dims = {spot_count.size(), max_spots};
        group.SaveVector("nPeaks", spot_count);
        group.SaveVector("peakXPosRaw", spot_x, dims, CompressionAlgorithm::BSHUF_LZ4);
        group.SaveVector("peakYPosRaw", spot_y, dims, CompressionAlgorithm::BSHUF_LZ4);
        group.SaveVector("peakTotalIntensity", spot_intensity, dims, CompressionAlgorithm::BSHUF_LZ4);
    }

    data_file.reset();
}

void HDF5DataFile::CreateFile() {
    data_file = std::make_unique<HDF5File>(filename, true,false, false);
    HDF5Group(*data_file, "/entry").NXClass("NXentry");
    HDF5Group(*data_file, "/entry/data").NXClass("NXdata");

    HDF5DataSpace data_space({1, ypixel, xpixel},{H5S_UNLIMITED, ypixel, xpixel});
    data_set = std::make_unique<HDF5DataSet>(*data_file, "/entry/data/data", data_type, data_space, dcpl);
}

void HDF5DataFile::WriteImage(const void *data, size_t data_size, uint64_t image_number) {
    if (image_number > max_image_number) {
        max_image_number = image_number;
        data_set->SetExtent({max_image_number+1, ypixel, xpixel});
    }
    data_set->WriteDirectChunk(data, data_size, {image_number, 0, 0});
}

void HDF5DataFile::WriteSpots(const std::vector<SpotToSave> &spots, uint64_t image_number) {
    std::unique_lock<std::mutex> ul(spot_mutex);

    size_t cnt = std::min(spots.size(), max_spots);
    if (spot_count.size() < image_number + 1) {
        spot_count.resize(image_number + 1);
        spot_x.resize(max_spots * (image_number + 1));
        spot_y.resize(max_spots * (image_number + 1));
        spot_intensity.resize(max_spots * (image_number + 1));
    }

    spot_count[image_number] = cnt;

    for (int i = 0; i < cnt; i++) {
        spot_x[max_spots * image_number + i] = spots[i].x;
        spot_y[max_spots * image_number + i] = spots[i].y;
        spot_intensity[max_spots * image_number + i] = spots[i].intensity;
    }
}

void HDF5DataFile::Write(const void *data, size_t data_size, const std::vector<SpotToSave> &spots,
                         uint64_t image_number) {
    std::lock_guard<std::mutex> lock(hdf5_mutex);
    if (!data_file)
        CreateFile();

    WriteImage(data, data_size, image_number);
    if (!spots.empty())
        WriteSpots(spots, image_number);
}
