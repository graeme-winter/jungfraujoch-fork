// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include "../compression/JFJochCompressor.h"
#include "HDF5Objects.h"
#include "HDF5DataFile.h"

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    if (argc != 4) {
        std::cerr << "Usage: ./HDF5Sum <HDF5 input file> <HDF5 output file> <summation factor>" << std::endl;
        exit(EXIT_FAILURE);
    }
    char *pEnd = nullptr;
    size_t summation_factor = std::strtol(argv[3], &pEnd, 10);

    if (summation_factor <= 1) {
        std::cerr << "Summation factor must be at least 2" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        HDF5ReadOnlyFile data_in(argv[1]);
        HDF5DataSet dataset_in(data_in, "/entry/data/data");
        HDF5DataSpace file_space_in(dataset_in);

        if (file_space_in.GetNumOfDimensions() != 3) {
            std::cerr << "Doesn't look like JF data file" << std::endl;
            exit(EXIT_FAILURE);
        }

        hsize_t nimages_in = file_space_in.GetDimensions()[0];
        hsize_t height = file_space_in.GetDimensions()[1];
        hsize_t width = file_space_in.GetDimensions()[2];

        if (nimages_in < summation_factor) {
            std::cerr << "Summation factor doesn't make sense (must be less or equal than images in the input file)" << std::endl;
            exit(EXIT_FAILURE);
        }
        hsize_t nimages_out = nimages_in / summation_factor;

        std::cout << "Summing dataset of " << nimages_in << " to " << nimages_out << " images" << std::endl;

        HDF5DataFile dataFile(argv[2], 0, nimages_out, width, height, 4, true);

        std::vector<hsize_t> dim = {summation_factor, height, width};

        constexpr const int32_t underload_sum = INT32_MIN;
        constexpr const int32_t overload_sum = INT32_MAX;

        JFJochBitShuffleCompressor compressor(CompressionAlgorithm::BSHUF_LZ4, 4096, 0);
        std::vector<uint8_t> output(bshuf_compress_lz4_bound(height*width, 4, 4096) + 12);

        for (int i = 0; i < nimages_out; i++) {
            std::vector<hsize_t> start = {summation_factor * i,0,0};
            std::vector<int16_t> image_in(summation_factor * height*width);
            dataset_in.ReadVector(image_in, start, dim);

            std::vector<int32_t> image_out(height*width, 0);
            for (int j = 0; j < summation_factor; j++) {
                for (int pxl = 0; pxl < width * height; pxl++) {
                    int16_t tmp = image_in[j * width * height + pxl];
                    if ((tmp < INT16_MIN + 4) || (image_out[pxl] == underload_sum))
                        image_out[pxl] = underload_sum;
                    else if ((tmp > INT16_MAX - 4) || (image_out[pxl] == overload_sum))
                        image_out[pxl] = overload_sum;
                    else image_out[pxl] += tmp;
                }
            }

            auto image_size = compressor.Compress(output.data(), image_out);
            dataFile.Write(output.data(), image_size, i);
        }
    } catch (const JFJochException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}