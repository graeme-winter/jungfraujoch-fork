// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <iostream>
#include "../common/Logger.h"
#include "../writer/HDF5Writer.h"
#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"

int main(int argc, char **argv) {
    Logger logger("HDF5DatasetWriteTest");

    RegisterHDF5Filter();

    if ((argc < 2) || (argc > 4)) {
        std::cout << "Usage: ./CompressionBenchmark <JF4M hdf5 file> {{<#images>} <rate in Hz>}" << std::endl;
        std::cout << std::endl;
        std::cout << "Env. variables:" << std::endl;
        std::cout << "HDF5DATASET_WRITE_TEST_IMAGES_PER_FILE and HDF5DATASET_WRITE_TEST_PREFIX" << std::endl;
        exit(EXIT_FAILURE);
    }

    int64_t nimages_out = 100;
    double rate = 2200;

    if (argc >= 3)
        nimages_out = atoi(argv[2]);
    if (argc >= 4)
        rate = atof(argv[3]);

    std::chrono::microseconds period_us((rate == 0) ? 0 : (int64_t) (1.0e6 / rate));

    HDF5File data(argv[1], false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        std::cout << "/entry/data/data must be 3D" << std::endl;
        exit(EXIT_FAILURE);
    }

    DiffractionExperiment x;
    x.ImageTime(std::chrono::milliseconds(1));

    // Set metadata for the compression_benchmark.h5 dataset
    x.BeamX_pxl(1090).BeamY_pxl(1136).DetectorDistance_mm(75).OmegaStart(0)
    .OmegaIncrement(0.088).Wavelength_A(1.0);
    x.MaskModuleEdges(true);
    x.MaskChipEdges(true);

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        std::cout << "JF4M with gaps detected (2068 x 2164)" << std::endl;
        x.DataStreamModuleSize(2, {8}, 8, 36);
    } else if ((file_space.GetDimensions()[1] == 4*514) && (file_space.GetDimensions()[2] == 2*1030)) {
        std::cout << "JF4M with no gaps detected (2060 x 2056)" << std::endl;
        x.DataStreamModuleSize(2, {8}, 0, 0);
    } else {
        std::cout << "Unknown geometry - exiting" << std::endl;
        exit(EXIT_FAILURE);
    }
    uint64_t nimages = file_space.GetDimensions()[0];
    logger.Info("Number of images in the original dataset: " + std::to_string(nimages));

    // Set file name
    if (std::getenv("HDF5DATASET_WRITE_TEST_PREFIX") == nullptr)
        x.FilePrefix("writing_test");
    else
        x.FilePrefix(std::getenv("HDF5DATASET_WRITE_TEST_PREFIX"));

    // Set images per single data file
    if (std::getenv("HDF5DATASET_WRITE_TEST_IMAGES_PER_FILE") == nullptr)
        x.ImagesPerFile(100);
    else
        x.ImagesPerFile(atoi(std::getenv("HDF5DATASET_WRITE_TEST_IMAGES_PER_FILE")));


    x.Mode(DetectorMode::Conversion).ImagesPerTrigger(nimages);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    dataset.ReadVector(image_conv, start, file_space.GetDimensions());

    auto image = (int16_t *) malloc( nimages * x.GetModulesNum() * RAW_MODULE_SIZE * sizeof(int16_t));
    for (int i = 0; i < nimages; i++) {
        ConvertedToRawGeometry(x,
                               image + i * RAW_MODULE_SIZE * x.GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);
    }

    // Switch to mode with gaps for transformation
    x.DataStreamModuleSize(2, {8}, 8, 36);

    JungfrauCalibration calibration(x);
    FrameTransformation transformation(x, calibration);

    std::vector<int64_t> output_size(nimages);
    std::vector<std::vector<char> > output(nimages);
    for (auto &i: output) i.resize(x.GetMaxCompressedSize());

    for (int i = 0; i < nimages; i++) {
        transformation.SetOutput(output[i].data());
        for (int j = 0; j < 8; j++)
            transformation.ProcessModule(image + (i * x.GetModulesNum() + j) * RAW_MODULE_SIZE ,
                                         i, j, 0);
        output_size[i] = transformation.PackStandardOutput();
    }

    x.ImagesPerTrigger(nimages_out);
    logger.Info("Number of images to write: " + std::to_string(nimages_out));

    JungfrauCalibration c(8,0);

    /*
    for (int i = 0; i < RAW_MODULE_COLS; i++) {
        c.Mask()[RAW_MODULE_COLS*25+i] = 2;
        c.Mask()[2*RAW_MODULE_SIZE + RAW_MODULE_COLS*256 + i] = 4;
        c.Mask()[4*RAW_MODULE_SIZE + RAW_MODULE_COLS*54+i] = 8;
    } */

    // Master & calibration files are written outside of timing routine
    auto fileset = std::make_unique<HDF5Writer>(x);

    auto start_time = std::chrono::system_clock::now();
    logger.Info("Writing " + std::to_string(nimages_out) + " images");

    int64_t total_image_size = 0;
    for (int i = 0; i < nimages_out; i++) {
        std::this_thread::sleep_until(start_time + i * period_us);
        fileset->Write(output[i % nimages].data(), output_size[i % nimages], i);
        total_image_size += output_size[i % nimages];
    }

    fileset.reset(); // Ensure data file is closed here
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    int64_t bandwidth_MBs = (double) total_image_size / (double)elapsed.count();
    int64_t frequency_Hz = (nimages_out * 1e6) / (double) (elapsed.count());

    logger.Info("Write HDF5 master file");
    JFJochProtoBuf::JFJochReceiverOutput receiver_output;
    *receiver_output.mutable_calibration() = c;
    receiver_output.set_start_time_ms(1640995200000);
    receiver_output.set_end_time_ms(1640995210000);
    receiver_output.set_images_sent(nimages);
    receiver_output.set_max_image_number_sent(nimages - 1);
    *receiver_output.mutable_jungfraujoch_settings() = x;
    WriteHDF5MasterFile(receiver_output);

    logger.Info("Writing done");

    logger.Info("Write speed " + std::to_string(bandwidth_MBs) + " MB/s");
    logger.Info("Frequency " + std::to_string(frequency_Hz) + " Hz");
}