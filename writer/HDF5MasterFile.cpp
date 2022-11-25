// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HDF5MasterFile.h"
#include "HDF5NXmx.h"
#include "MakeDirectory.h"

void WriteHDF5MasterFile(const JFJochProtoBuf::WriterMetadataInput &input) {
    const std::string& filename = input.metadata_file_name();

    MakeDirectory(filename);

    HDF5File hdf5_file(filename, true, true, false);
    hdf5_file.Attr("file_name", filename);
    HDF5Metadata::NXmx(&hdf5_file, input);
}