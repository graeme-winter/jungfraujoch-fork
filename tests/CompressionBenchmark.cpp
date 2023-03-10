// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include <bitshuffle/bitshuffle_core.h>
#include <zstd.h>
#include "../writer/HDF5Objects.h"
#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"
#include "../compression/JFJochDecompress.h"

std::string CheckCompression(const DiffractionExperiment &x, size_t nimages, const std::vector<int16_t> &image) {

    double original_size = nimages * x.GetModulesNum() * RAW_MODULE_SIZE * x.GetPixelDepth();

    FrameTransformation transformation(x);
    std::vector<uint16_t> output(nimages * x.GetMaxCompressedSize());

    double compressed_size = 0;
    auto start_time = std::chrono::system_clock::now();

    for (int i = 0; i < nimages; i++) {
        transformation.SetOutput(output.data() + i * x.GetMaxCompressedSize());
        for (int j = 0; j < x.GetModulesNum(); j++ ) {
            transformation.ProcessModule(image.data() + (j + i * x.GetModulesNum()) * RAW_MODULE_SIZE,
                                         i, j, 0);
        }
        compressed_size += transformation.PackStandardOutput();
    }

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    char buf[256];

    snprintf(buf, 255, "%.2fx compression (%5.2f bits/pxl)   Throughput: %5.2f GB/s\n", original_size / compressed_size, compressed_size * 8 / double(nimages * x.GetModulesNum() * RAW_MODULE_SIZE), (original_size / elapsed.count()) / 1000.0);
    return std::string(buf);
}

std::string CheckDecompression(const DiffractionExperiment &x, size_t nimages, const std::vector<int16_t> &image) {
    double original_size = nimages * x.GetModulesNum() * RAW_MODULE_SIZE * x.GetPixelDepth();

    FrameTransformation transformation(x);
    std::vector<char> output[nimages];
    for (auto &v: output)
        v.resize(x.GetMaxCompressedSize());

    std::vector<size_t> compressed_size(nimages);

    for (int i = 0; i < nimages; i++) {
        transformation.SetOutput(output[i].data());
        for (int j = 0; j < x.GetModulesNum(); j++ ) {
            transformation.ProcessModule(image.data() + (j + i * x.GetModulesNum()) * RAW_MODULE_SIZE,
                                         i, j, 0);
        }
        compressed_size[i] = transformation.PackStandardOutput();
        output[i].resize(compressed_size[i]);
    }

    std::vector<uint16_t> decompress_v;
    auto start_time = std::chrono::system_clock::now();
    for (int i = 0; i < nimages; i++) {
        JFJochDecompress(decompress_v, x.GetCompressionAlgorithmEnum(), output[i], x.GetPixelsNum());
    }

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    char buf[256];

    snprintf(buf, 255, "    decompression                    Throughput: %5.2f GB/s\n", (original_size / elapsed.count()) / 1000.0);
    return {buf};
}

int main(int argc, char **argv) {

    RegisterHDF5Filter();

    if (argc != 2) {
        std::cout << "Usage: ./CompressionBenchmark <JF4M hdf5 file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    HDF5ReadOnlyFile data(argv[1]);

    HDF5DataSet dataset(data, "/entry/data/data");

    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        std::cout << "/entry/data/data must be 3D" << std::endl;
        exit(EXIT_FAILURE);
    }

    DiffractionExperiment x(2, {8}, 8, 36);

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        std::cout << "JF4M with gaps detected (2068 x 2164)" << std::endl;
    } else {
        std::cout << "Unknown geometry - exiting" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint64_t nimages = file_space.GetDimensions()[0];
    std::cout << "Number of images in the dataset: " << nimages << std::endl;
    if (nimages > 200) {
        nimages = 200;
        std::cout << "Using only " << nimages << " images" << std::endl;

    }
    x.Mode(DetectorMode::Conversion);

    std::vector<int16_t> image_conv ( nimages * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);

    std::vector<hsize_t> start = {0,0,0};
    std::vector<hsize_t> dim = {nimages, file_space.GetDimensions()[1], file_space.GetDimensions()[2]};

    auto start_time = std::chrono::system_clock::now();
    dataset.ReadVector(image_conv, start, dim);
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Images loaded      " << elapsed.count() / nimages << " ms/image      "
              << (image_conv.size() * sizeof(uint16_t)) / (1000.0 * elapsed.count())  <<  " MB/s" << std::endl;

    std::vector<int16_t> image( nimages * x.GetModulesNum() * RAW_MODULE_SIZE);

    for (int i = 0; i < nimages; i++) {
        ConvertedToRawGeometry(x,
                               image.data() + i * RAW_MODULE_SIZE * x.GetModulesNum(),
                               image_conv.data() + i * file_space.GetDimensions()[1] * file_space.GetDimensions()[2]);
    }

    auto image_shuffled = bitshuffle(image, 4096);

    x.MaskChipEdges(false);

    x.Compression(JFJochProtoBuf::NO_COMPRESSION);
    std::cout << "None (geom transform)       " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_LZ4);
    std::cout << "BSHUF/LZ4                   " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD);
    std::cout << "BSHUF/ZSTD (0)              " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD_RLE);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::NO_COMPRESSION);
    std::cout << "None (geom transform)       " << CheckCompression(x, nimages, image);

    std::cout << std::endl << std::endl << "Decompression" << std::endl << std::endl;

    x.Compression(JFJochProtoBuf::BSHUF_LZ4);
    std::cout << "BSHUF/LZ4                   " << CheckDecompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD);
    std::cout << "BSHUF/ZSTD                  " << CheckDecompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD_RLE);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckDecompression(x, nimages, image);

    std::cout << std::endl << std::endl << "EIGER-type transformation (16-bit)" << std::endl << std::endl;
    x.DetectorType(JFJochProtoBuf::EIGER);

    x.Compression(JFJochProtoBuf::NO_COMPRESSION);
    std::cout << "None (geom transform)       " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_LZ4);
    std::cout << "BSHUF/LZ4                   " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD);
    std::cout << "BSHUF/ZSTD (0)              " << CheckCompression(x, nimages, image);

    x.Compression(JFJochProtoBuf::BSHUF_ZSTD_RLE);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckCompression(x, nimages, image);
}