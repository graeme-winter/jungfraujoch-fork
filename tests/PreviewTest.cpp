// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../common/ZMQPreviewPublisher.h"
#include "../writer/HDF5Objects.h"
#include "../common/Logger.h"

int main(int argc, char **argv) {

    Logger logger("PreviewTest");


    RegisterHDF5Filter();

    if (argc != 2) {
        logger.Error("Usage: ./CompressionBenchmark <JF4M hdf5 file>");
        exit(EXIT_FAILURE);
    }

    ZMQContext context;
    ZMQPreviewPublisher publisher(context, "tcp://0.0.0.0:5400");

    HDF5ReadOnlyFile data(argv[1]);

    HDF5DataSet dataset(data, "/entry/data/data");

    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        logger.Error("/entry/data/data must be 3D");
        exit(EXIT_FAILURE);
    }

    DiffractionExperiment x(2, {8}, 8, 36);

    x.BeamX_pxl(1090).BeamY_pxl(1136).DetectorDistance_mm(75).PhotonEnergy_keV(WVL_1A_IN_KEV);

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        logger.Info("JF4M with gaps detected (2068 x 2164)");
    } else {
        logger.Error("Unknown geometry - exiting");
        exit(EXIT_FAILURE);
    }
    uint64_t nimages = file_space.GetDimensions()[0];

    logger.Info("Number of images in the dataset: " + std::to_string(nimages));

    x.Mode(DetectorMode::Conversion);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    dataset.ReadVector(image_conv, start, file_space.GetDimensions());

    logger.Info("Images loaded");

    JFCalibration calibration(x);
    calibration.Mask(0) = 0x2;
    calibration.Mask(1) = 0x2;
    calibration.Mask(2) = 0x2;

    publisher.Start(x, calibration);

    for (int i = 0; i < nimages; i++) {
        publisher.Publish(x,
                          image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2],
                          i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logger.Info("Done");
}
