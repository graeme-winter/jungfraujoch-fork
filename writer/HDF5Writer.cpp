// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <sys/types.h>
#include <fstream>

#include "HDF5Writer.h"
#include "HDF5MasterFile.h"

HDF5Writer::HDF5Writer(const JFJochProtoBuf::WriterInput &request) :
images_remaining(request.data_files_size()), files(request.data_files_size()) {

    CompressionAlgorithm compression;
    switch (request.compression_algorithm()) {
        case JFJochProtoBuf::BSHUF_LZ4:
            compression = CompressionAlgorithm::BSHUF_LZ4;
            break;
        case JFJochProtoBuf::BSHUF_ZSTD:
            compression = CompressionAlgorithm::BSHUF_ZSTD;
            break;
        default:
            compression = CompressionAlgorithm::NO_COMPRESSION;
            break;
    }

    for (int i = 0; i < request.data_files_size(); i++) {
        images_remaining[i] = request.data_files(i).image_count();
        files[i] = std::make_unique<HDF5DataFile>(request.data_files(i).filename(), request.data_files(i).image0(), request.data_files(i).image_count(),
                                                  request.width(), request.height(), request.pixel_depth_bytes(), request.signed_pxl(), compression,
                                                  request.compression_block_size(), request.max_spot_count());
    }
}

void HDF5Writer::Write(const void *data, size_t data_size, const std::vector<SpotToSave>& spots,
                       int64_t file_number, int64_t image_number_in_file) {
    if ((file_number < 0) || (file_number >= files.size()) || (image_number_in_file < 0))
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Mismatch in image metadata");
    if (images_remaining[file_number] == 0)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "All images written for the data file");
    if (!files[file_number])
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Error with image numbers");

    // Zero size images won't be written, but will be included in remaining images count
    if (data_size > 0)
        files[file_number]->Write(data, data_size, image_number_in_file);
    if (!spots.empty())
        files[file_number]->WriteSpots(image_number_in_file, spots);

    // Close file if all images written
    if ((--images_remaining[file_number]) == 0)
        files[file_number].reset();
}

size_t HDF5Writer::GetRemainingImageCount() const {
    return std::accumulate(images_remaining.begin(), images_remaining.end(), 0);
}

void HDF5Writer::ErrorIfFileExists(const std::string &path) {
    if (std::filesystem::exists(path))
        throw JFJochException(JFJochExceptionCategory::FileWriteError, "File already exists");
}