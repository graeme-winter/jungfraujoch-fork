// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <sys/types.h>
#include <fstream>

#include "HDF5Writer.h"
#include "HDF5MasterFile.h"
#include "HDF5NXmx.h"

HDF5Writer::HDF5Writer(const JFJochProtoBuf::WriterInput &request)
: files(request.file_count()) {

    CompressionAlgorithm compression;
    switch (request.compression()) {
        case JFJochProtoBuf::BSHUF_LZ4:
            compression = CompressionAlgorithm::BSHUF_LZ4;
            break;
        case JFJochProtoBuf::BSHUF_ZSTD:
            compression = CompressionAlgorithm::BSHUF_ZSTD;
            break;
        case JFJochProtoBuf::BSHUF_ZSTD_RLE:
            compression = CompressionAlgorithm::BSHUF_ZSTD_RLE;
            break;
        default:
            compression = CompressionAlgorithm::NO_COMPRESSION;
            break;
    }

    for (int i = 0; i < request.file_count(); i++)
        files[i] = std::make_unique<HDF5DataFile>(HDF5Metadata::DataFileName(request.file_prefix(), i),
                                                  request.width(), request.height(),
                                                  request.pixel_depth_bytes(),
                                                  request.signed_pxl(),
                                                  compression,
                                                  request.max_spot_count());
}

void HDF5Writer::Write(const void *data, size_t data_size, const std::vector<SpotToSave>& spots, int64_t image_number) {
    if (image_number < 0)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "No support for negative images");
    if (files.empty())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "No file to write");

    size_t file_number = image_number % files.size();
    // Ignore zero size images
    if (data_size > 0)
        files[file_number]->Write(data, data_size, spots, image_number / files.size());
}
