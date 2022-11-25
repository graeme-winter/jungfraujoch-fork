// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>

#include "HDF5DataFile.h"

HDF5DataFile::HDF5DataFile(const std::string &in_filename, int64_t in_image0, int64_t in_nimages, int64_t width, int64_t height,
                           int64_t pixel_depth_byte, bool is_signed, CompressionAlgorithm compression,
                           int64_t compression_block_size, size_t in_max_spots) : data_type(pixel_depth_byte, is_signed),
                           max_spots(in_max_spots) {
    max_image_number = 0;

    if (max_spots > 256)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Max spot count must be less than 256 for performance reasons");

    filename = in_filename;
    nimages = in_nimages;
    image0 = in_image0;

    xpixel = width;
    ypixel = height;

    spot_x.reserve(max_spots * nimages);
    spot_y.reserve(max_spots * nimages);
    spot_intensity.reserve(max_spots * nimages);
    spot_count.reserve(nimages);
    if (compression != CompressionAlgorithm::NO_COMPRESSION)
        dcpl.SetCompression(compression, pixel_depth_byte, compression_block_size);

    dcpl.SetChunking( {1, ypixel, xpixel});
//    image_units = request.image_units();
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
    std::string tmp_filename = filename + ".tmp";
    rename(tmp_filename.c_str(), filename.c_str());
}

void HDF5DataFile::Write(const void *data, size_t data_size, size_t image_number) {
    std::lock_guard<std::mutex> lock(hdf5_mutex);
    if (!data_file) {
        data_file = std::make_unique<HDF5File>(filename + ".tmp", true,false, false);
        HDF5Group(*data_file, "/entry").NXClass("NXentry");
        HDF5Group(*data_file, "/entry/data").NXClass("NXdata");

        HDF5DataSpace data_space({1, ypixel, xpixel},{H5S_UNLIMITED, ypixel, xpixel});
        data_set = std::make_unique<HDF5DataSet>(*data_file, "/entry/data/data", data_type, data_space, dcpl);
        data_set->Attr("image_nr_low", (int32_t) (image0 + 1)).Attr("image_nr_high", (int32_t) (image0 + nimages));
        data_set->Units(image_units);
    }
    if (image_number > max_image_number) {
        max_image_number = image_number;
        data_set->SetExtent({max_image_number+1, ypixel, xpixel});
    }
    data_set->WriteDirectChunk(data, data_size, {image_number, 0, 0});
}

void HDF5DataFile::WriteSpots(size_t image_number, const std::vector<SpotToSave> &spots) {
    std::unique_lock<std::mutex> ul(spot_mutex);

    size_t cnt = std::min(spots.size(), max_spots);
    if (cnt > 0) {
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
}