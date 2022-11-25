// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

// Copyright (2019-2021) Paul Scherrer Institute

#include <cmath>
#include <vector>
#include "../writer/HDF5Objects.h"
#include "../common/RadialIntegration.h"

std::vector<uint8_t> GetOneByteMask(DiffractionExperiment &x, HDF5Object &master_file) {
    std::vector<uint8_t> ret(x.GetPixelsNum(), 1);

    HDF5DataSet pixelmask_dataset(master_file, "/entry/instrument/detector/pixel_mask");

    std::vector<hsize_t> pm_start = {0, 0};
    std::vector<hsize_t> pm_dim = {(hsize_t) x.GetYPixelsNum(), (hsize_t) x.GetXPixelsNum()};

    std::vector<uint32_t> pixel_mask(x.GetPixelsNum());

    pixelmask_dataset.ReadVector(pixel_mask, pm_start, pm_dim);

    for (int i = 0; i < x.GetPixelsNum(); i++) {
        if (pixel_mask[i] != 0)
            ret[i] = 0;
    }
    return ret;
}

void CalcRadialIntegration(DiffractionExperiment &x, RadialIntegration &rad, const char *file_name,
                           hsize_t first, hsize_t last) {
    HDF5ReadOnlyFile data_file(file_name);
    HDF5DataSet dataset(data_file, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        std::cout << "/entry/data/data must be 3D" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<int16_t> source(x.GetPixelsNum());

    first = std::min(first, file_space.GetDimensions()[0] - 1);
    last = std::min(last, file_space.GetDimensions()[0] - 1);
    std::cout << "# " << first << " " << last << std::endl;
    for (hsize_t i = first; i < last; i++) {
        std::vector<hsize_t> start = {i, 0, 0};
        std::vector<hsize_t> dim = {1, (hsize_t) x.GetYPixelsNum(), (hsize_t) x.GetXPixelsNum()};
        dataset.ReadVector(source, start, dim);
        rad.Process(source.data(), x.GetPixelsNum());
    }
}

void GetGeometry(DiffractionExperiment &x, HDF5Object &master_file) {
    x.BeamX_pxl(
            HDF5DataSet(master_file, "/entry/instrument/detector/beam_center_x").ReadScalar<double>());
    x.BeamY_pxl(
            HDF5DataSet(master_file, "/entry/instrument/detector/beam_center_y").ReadScalar<double>());
    x.DetectorDistance_mm(
            HDF5DataSet(master_file, "/entry/instrument/detector/detector_distance").ReadScalar<double>() *1e3);
}

int main(int argc, char **argv) {

    hsize_t first, last;
    DiffractionExperiment x(2, {8}, 8, 36);

    RegisterHDF5Filter();

    if (argc == 3) {
        first = 0;
        last = INT32_MAX;
    } else if (argc == 5) {
        first = std::strtol(argv[3], nullptr, 10);
        last = std::strtol(argv[4], nullptr, 10);
    } else {
        std::cout << "Usage ./ImageAverage <name of master dataset> <name of data dataset> {<first image> <last image>}" << std::endl;
        exit(EXIT_FAILURE);
    }

    HDF5ReadOnlyFile master_file(argv[1]);
    auto one_byte_mask = GetOneByteMask(x, master_file);

    GetGeometry(x, master_file);

    RadialIntegrationMapping rad_int_map(x, one_byte_mask.data());
    RadialIntegration rad_int(rad_int_map);

    CalcRadialIntegration(x, rad_int, argv[2], first, last);

    auto result_x = rad_int_map.GetBinToQ();
    std::vector<float> result_y;
    rad_int.GetResult(result_y);

    for (int i = 0; i < result_y.size(); i++)
        std::cout << result_x[i] << " " << result_y[i] << std::endl;
}
