// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HDF5MASTERFILE_H
#define HDF5MASTERFILE_H

#include <string>

#include "jfjoch.pb.h"
#include "HDF5Objects.h"

void WriteHDF5MasterFile(const JFJochProtoBuf::JFJochWriterMetadataInput &output);

#endif //JUNGFRAUJOCH_HDF5MASTERFILE_H
