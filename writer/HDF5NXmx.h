// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HDF5NXMX_H
#define JUNGFRAUJOCH_HDF5NXMX_H

#include <jfjoch.pb.h>

#include "HDF5Objects.h"

namespace HDF5Metadata {
    void NXmx(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void Time(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void LinkToData(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);

    void Detector(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void DetectorModule(HDF5File *hdf5_file, const std::string &name, const std::vector<int32_t> &origin,
                        const std::vector<int32_t> &size, const std::vector<double> &fast_axis,
                        const std::vector<double> &slow_axis, const std::string &nx_axis, double pixel_size_mm);

    void Facility(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void Beam(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void Metrology(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);
    void Sample(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &input);

    void Mask(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &output);
    void Calibration(HDF5File *hdf5_file, const JFJochProtoBuf::WriterMetadataInput &output);

    std::string DataFileName(const std::string& prefix, int64_t file_number);
}

#endif //JUNGFRAUJOCH_HDF5NXMX_H
