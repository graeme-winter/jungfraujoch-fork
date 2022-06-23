// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <iostream>
#include "../common/Logger.h"
#include "../writer/HDF5Writer.h"
#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"
#include "../common/ZMQImagePusher.h"

int main(int argc, char **argv) {
    Logger logger("ZMQTestPusher");

    RegisterHDF5Filter();

    if (argc < 4) {
        std::cout << "Usage: ./ZMQImagePusher <JF4M hdf5 file> <#images> <zmq address>" << std::endl;
        std::cout << std::endl;
        exit(EXIT_FAILURE);
    }

    int64_t nimages_out = 100;
    if (argc >= 3)
        nimages_out = atoi(argv[2]);

    DiffractionExperiment x;
    x.ImageTime(std::chrono::milliseconds(1));
    x.DataStreamModuleSize(2, {8}, 8, 36);
    x.ImagesPerTrigger(nimages_out);
    x.ImagesPerFile(1000);
    x.Mode(DetectorMode::Conversion);

    HDF5File data(argv[1], false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        std::cout << "/entry/data/data must be 3D" << std::endl;
        exit(EXIT_FAILURE);
    }

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        std::cout << "JF4M with gaps detected (2068 x 2164)" << std::endl;
        x.DataStreamModuleSize(2, {8}, 8, 36);
    } else {
        std::cout << "Unknown geometry - exiting" << std::endl;
        exit(EXIT_FAILURE);
    }
    uint64_t nimages_in_file = file_space.GetDimensions()[0];
    logger.Info("Number of images in the original dataset: " + std::to_string(nimages_in_file));

    ZMQContext context;
    context.NumThreads(4);

    std::vector<std::string> zmq_addr;
    for (int i = 3; i < argc; i++)
        zmq_addr.emplace_back(argv[i]);

    ZMQImagePusher pusher(context);
    pusher.Connect(zmq_addr, x);

    JungfrauCalibration calibration(x);
    FrameTransformation transformation(x, calibration);

    std::vector<std::vector<char> > output(nimages_in_file);
    std::vector<int64_t> output_size(nimages_in_file);
    for (auto &i: output) i.resize(x.GetMaxCompressedSize());

    std::vector<int16_t> image_tmp_conv( file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);
    std::vector<int16_t> image_tmp_raw(x.GetModulesNum() * RAW_MODULE_SIZE);

    for (int i = 0; i < nimages_in_file; i++) {
        std::vector<hsize_t> start = {(hsize_t)i,0,0};
        std::vector<hsize_t> dim = {1,file_space.GetDimensions()[1], file_space.GetDimensions()[2]};
        dataset.ReadVector(image_tmp_conv, start, dim);

        ConvertedToRawGeometry(x,
                               image_tmp_raw.data(),
                               image_tmp_conv.data());

        transformation.SetOutput(output[i].data());
        for (int j = 0; j < x.GetModulesNum(); j++)
            transformation.ProcessModule(image_tmp_raw.data() + j * RAW_MODULE_SIZE,
                                         i, j, 0);
        output_size[i] = transformation.PackStandardOutput();
    }

    logger.Info("Sending " + std::to_string(nimages_out) + " images");

    std::vector<char> send_buffer(x.GetMaxCompressedSize() + sizeof(ImageMetadata));

    for (int i = 0; i < nimages_out; i++) {
        memcpy(send_buffer.data() + sizeof(ImageMetadata),
               output[i % nimages_in_file].data(),
               output_size[i % nimages_in_file]);
        pusher.SendData(send_buffer.data(), i, output_size[i % nimages_in_file]);
    }
    pusher.EndDataCollection();
    logger.Info("Sending done");
}
