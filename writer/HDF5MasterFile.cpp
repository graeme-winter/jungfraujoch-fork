// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fstream>

#include "../common/Coord.h"
#include "../common/JFJochException.h"
#include "HDF5MasterFile.h"
#include "../common/RawToConvertedGeometry.h"
#include "HDF5NXmx.h"

void WriteHDF5MasterFile(const JFJochProtoBuf::JFJochWriterMetadataInput &output) {
    DiffractionExperiment experiment(output.receiver_output().jungfraujoch_settings());
    std::string filename = experiment.GenerateMasterFilename();

    HDF5File hdf5_file(filename, true, true, false);
    hdf5_file.Attr("file_name", filename);
    HDF5Metadata::NXmx(&hdf5_file, experiment, output);
}