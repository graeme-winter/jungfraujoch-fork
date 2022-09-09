// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_HDF5NXMX_H
#define JUNGFRAUJOCH_HDF5NXMX_H

#include "HDF5Objects.h"
#include "../common/DiffractionExperiment.h"
#include "../common/JFCalibration.h"

namespace HDF5Metadata {
    void NXmx(HDF5File *hdf5_file, const DiffractionExperiment& experiment,
              const JFJochProtoBuf::JFJochWriterMetadataInput &output);
    void Time(HDF5File *hdf5_file, const DiffractionExperiment& experiment, const JFJochProtoBuf::JFJochReceiverOutput &output);
    void Detector(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void DetectorGroup(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void DataVDS(HDF5File *hdf5_file, const DiffractionExperiment &experiment, int64_t actual_image_number = -1);
    void DataVDS_TimeResolved(HDF5File *hdf5_file, const DiffractionExperiment &experiment, int64_t actual_image_number = -1);
    void LinkToData(HDF5File *hdf5_file, const DiffractionExperiment& experiment, int64_t actual_image_number = -1);
    void Beam(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void Attenuator(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void Metrology(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void Sample(HDF5File *hdf5_file, const DiffractionExperiment& experiment);
    void Mask(HDF5File *hdf5_file, const DiffractionExperiment& experiment, const JFCalibration& calib);
    void Calibration(HDF5File *hdf5_file, const JFCalibration& calib);
    void FinalSettings(HDF5File *hdf5_file, const JFJochProtoBuf::JFJochReceiverOutput &output);
    void Processing(HDF5File *hdf5_file, const DiffractionExperiment& experiment, const JFJochProtoBuf::JFJochIndexerOutput &output);
}

namespace HDF5Data {
    std::unique_ptr<HDF5DataSet> Data(HDF5File *hdf5_file, const DiffractionExperiment& experiment, size_t image0, size_t nimages, size_t ypixel);
}

#endif //JUNGFRAUJOCH_HDF5NXMX_H
