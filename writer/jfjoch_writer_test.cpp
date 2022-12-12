// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <iostream>
#include "HDF5Objects.h"
#include "../common/Logger.h"
#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"
#include "../common/ZMQImagePusher.h"
#include "../grpc/JFJochWriterGroupClient.h"

#define BASE_TCP_PORT 8000

int main(int argc, char **argv) {
    Logger logger("jfjoch_writer_test");

    RegisterHDF5Filter();

    if (argc < 4) {
        std::cout << "Usage: ./jfjoch_writer_test <JF4M hdf5 file> <#images> <writer gRPC address(es)>" << std::endl;
        std::cout << std::endl;
        exit(EXIT_FAILURE);
    }

    int64_t nimages_out = atoi(argv[2]);

    DiffractionExperiment x(2, {8}, 8, 36, true);
    x.Summation(1).ImagesPerTrigger(nimages_out).Mode(DetectorMode::Conversion);

    HDF5File data(argv[1], false, false, false);
    HDF5DataSet dataset(data, "/entry/data/data");
    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        logger.Error("/entry/data/data must be 3D");
        exit(EXIT_FAILURE);
    }

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        logger.Info("JF4M with gaps detected (2068 x 2164)");
    } else {
        logger.Error( "Unknown geometry - exiting");
        exit(EXIT_FAILURE);
    }

    uint64_t nimages_in_file = file_space.GetDimensions()[0];
    logger.Info("Number of images in the original dataset: " + std::to_string(nimages_in_file));

    ZMQContext context;
    context.NumThreads(4);

    JFJochWriterGroupClient client;

    std::vector<std::string> zmq_addr;
    for (int i = 3; i < argc; i++) {
        zmq_addr.emplace_back("tcp://0.0.0.0:" + std::to_string(BASE_TCP_PORT + i));
        client.AddClient(argv[i]);
    }
    x.DataFileCount(zmq_addr.size());

    ZMQImagePusher pusher(context, zmq_addr);

    FrameTransformation transformation(x);

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

    logger.Info("Sending {} images", nimages_out);

    client.Start(x, zmq_addr);

    std::vector<DiffractionSpot> empty_spot_vector;

    pusher.StartDataCollection(zmq_addr.size());
    for (int i = 0; i < nimages_out; i++)
        pusher.SendData(output[i % nimages_in_file].data(),output_size[i % nimages_in_file],
                        empty_spot_vector, i);
    pusher.EndDataCollection();
    logger.Info("Sending done");
    auto stats = client.Stop();
    logger.Info("Writing done");
    for (int i = 0; i < stats.size(); i++)
        logger.Info("Writer {}: Images = {} Throughput = {:.0f} MB/s Frame rate = {:.0f} Hz",
                    i, stats[i].nimages(), stats[i].performance_mbs(), stats[i].performance_hz());
}
