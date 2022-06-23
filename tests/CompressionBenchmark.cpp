// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>

#include <bitshuffle/bitshuffle_core.h>
#include <zstd.h>
#include "../writer/HDF5Objects.h"
#include "../common/FrameTransformation.h"
#include "../common/RawToConvertedGeometry.h"

std::string CheckCompression(const DiffractionExperiment &x, size_t nimages, const std::vector<int16_t> &image) {

    double original_size = nimages * x.GetModulesNum() * RAW_MODULE_SIZE * x.GetPixelDepth();
    JungfrauCalibration calibration(x);

    FrameTransformation transformation(x, calibration);
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

    JungfrauCalibration calibration(x);
    FrameTransformation transformation(x, calibration);
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
    }

    JFJochDecompressor decompressor(x);
    auto start_time = std::chrono::system_clock::now();
    for (int i = 0; i < nimages; i++) {
        decompressor.Decompress(output[i].data(),
                                compressed_size[i], x.GetPixelsNum(),
                                sizeof(uint16_t));
    }

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    char buf[256];

    snprintf(buf, 255, "    decompression                    Throughput: %5.2f GB/s\n", (original_size / elapsed.count()) / 1000.0);
    return std::string(buf);
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

    DiffractionExperiment x;

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

    // Switch to mode with gaps
    x.DataStreamModuleSize(2, {8}, 8, 36);

    x.CompressionBlockSize(4096);

    x.Compression(CompressionAlgorithm::None);
    std::cout << "None (geom transform)       " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_LZ4);
    std::cout << "BSHUF/LZ4                   " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_LZ4, 10);
    std::cout << "BSHUF/LZ4  (10)             " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD);
    std::cout << "BSHUF/ZSTD (0)              " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, -10);
    std::cout << "BSHUF/ZSTD (-10)            " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, -5);
    std::cout << "BSHUF/ZSTD (-5)             " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, 5);
    std::cout << "BSHUF/ZSTD (5)              " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, 10);
    std::cout << "BSHUF/ZSTD (10)             " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckCompression(x, nimages, image);

    x.CompressionBlockSize(2*1024);
    x.Compression(CompressionAlgorithm::BSHUF_LZ4, 0);
    std::cout << "BSHUF/LZ4 (block 4kB)       " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, 0);
    std::cout << "BSHUF/ZSTD (block 4kB)      " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    std::cout << "BSHUF/ZSTD (block 4kB;RLE)  " << CheckCompression(x, nimages, image);

    x.CompressionBlockSize(32*1024);
    x.Compression(CompressionAlgorithm::BSHUF_LZ4, 0);
    std::cout << "BSHUF/LZ4 (block 64kB)      " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, 0);
    std::cout << "BSHUF/ZSTD (block 64kB)     " << CheckCompression(x, nimages, image);


    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    std::cout << "BSHUF/ZSTD (block 64kB;RLE) " << CheckCompression(x, nimages, image);

    x.CompressionBlockSize(512*1024);
    x.Compression(CompressionAlgorithm::BSHUF_LZ4, 0);
    std::cout << "BSHUF/LZ4 (block 1MB)       " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, 0);
    std::cout << "BSHUF/ZSTD (block 1MB)      " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    std::cout << "BSHUF/ZSTD (block 1MB;RLE)  " << CheckCompression(x, nimages, image);

    std::cout << std::endl << std::endl << "Decompression" << std::endl << std::endl;

    x.Compression(CompressionAlgorithm::BSHUF_LZ4).CompressionBlockSize(4096);
    std::cout << "BSHUF/LZ4                   " << CheckDecompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD).CompressionBlockSize(4096);
    std::cout << "BSHUF/ZSTD                  " << CheckDecompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE).CompressionBlockSize(4096);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckDecompression(x, nimages, image);

    x.CompressionBlockSize(4096);
    std::cout << std::endl << std::endl << "No multi-pixel calculation" << std::endl << std::endl;
    x.MaskChipEdges(true);

    x.Compression(CompressionAlgorithm::None);
    std::cout << "None (geom transform)       " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_LZ4);
    std::cout << "BSHUF/LZ4                   " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD);
    std::cout << "BSHUF/ZSTD (0)              " << CheckCompression(x, nimages, image);

    x.Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    std::cout << "BSHUF/ZSTD (RLE)            " << CheckCompression(x, nimages, image);
}