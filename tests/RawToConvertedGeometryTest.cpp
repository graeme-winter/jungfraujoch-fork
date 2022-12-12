// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>

#include "../common/RawToConvertedGeometry.h"

TEST_CASE("RawToConvertedGeometry_Half_and_quarter","[RawToConvertedGeometry]") {
    REQUIRE(half<int32_t>(7, -10, 20) == 3);
    REQUIRE(half<int32_t>(7, 9, 20) == 7);
    REQUIRE(half<int32_t>(27, 9, 20) == 27);

    REQUIRE(half<double>(7, -10, 20) == Approx(3.5));
    REQUIRE(half<double>(7, 9, 20) == 7);
    REQUIRE(half<double>(27, 9, 20) == 27);

    REQUIRE(quarter<int16_t>(6, -10, 20) == 1);
    REQUIRE(quarter<int16_t>(26, -10, 20) == 26);
    REQUIRE(quarter<int16_t>(-16, -10, 20) == -16);
}

TEST_CASE("RawToConvertedGeometry_LineCopyAndAddMultipixel","[RawToConvertedGeometry]") {
    int16_t in[1024];
    int16_t out[1030];
    for (int16_t i = 0; i < 1024; i++)
        in[i] = i;

    LineCopyAndAddMultipixel<int16_t>(out, in);
    REQUIRE(out[0] == 0);
    REQUIRE(out[100] == 100);
    REQUIRE(out[255] == 255);
    REQUIRE(out[255+1] == 255);
    REQUIRE(out[256+1] == 256);
    REQUIRE(out[256+2] == 256);
    REQUIRE(out[259+2] == 259);
    REQUIRE(out[400+2] == 400);
    REQUIRE(out[600+4] == 600);
    REQUIRE(out[768+5] == 768);
    REQUIRE(out[768+6] == 768);
    REQUIRE(out[1000+6] == 1000);
    REQUIRE(out[1015+6] == 1015);
    REQUIRE(out[1029] == 1029 - 6);
}

TEST_CASE("RawToConvertedGeometry_LineCopyAndAddMultipixelNormalRow","[RawToConvertedGeometry]") {
    int16_t in[1024];
    int16_t out[1030];
    for (int16_t i = 0; i < 1024; i++)
        in[i] = i;

    LineCopyAndAdjustMultipixel<int16_t, int16_t>(out, in, 0, 500);
    REQUIRE(out[0] == 0);
    REQUIRE(out[100] == 100);
    REQUIRE(out[255] == 255/2);
    REQUIRE(out[255+1] == 255/2);
    REQUIRE(out[256+1] == 256/2);
    REQUIRE(out[256+2] == 256/2);
    REQUIRE(out[259+2] == 259);
    REQUIRE(out[400+2] == 400);
    REQUIRE(out[600+4] == 600);
    REQUIRE(out[768+5] == 768); // > 500 so not modified
    REQUIRE(out[768+6] == 768); // > 500 so not modified
    REQUIRE(out[1000+6] == 1000);
    REQUIRE(out[1015+6] == 1015);
    REQUIRE(out[1029] == 1029 - 6);
}

TEST_CASE("RawToConvertedGeometry_LineCopyAndAddMultipixelMiddleRow","[RawToConvertedGeometry]") {
    int16_t in[1024];
    int16_t out[1030];
    for (int16_t i = 0; i < 1024; i++)
        in[i] = i;

    LineCopyAndAdjustMultipixelMidRow<int16_t, int16_t>(out, in, 0, 500);
    REQUIRE(out[0] == 0);
    REQUIRE(out[100] == 100/2);
    REQUIRE(out[255] == 255/4);
    REQUIRE(out[255+1] == 255/4);
    REQUIRE(out[256+1] == 256/4);
    REQUIRE(out[256+2] == 256/4);
    REQUIRE(out[259+2] == 259/2);
    REQUIRE(out[400+2] == 400/2);
    REQUIRE(out[600+4] == 600); // > 500 so not modified
    REQUIRE(out[768+5] == 768); // > 500 so not modified
    REQUIRE(out[768+6] == 768); // > 500 so not modified
    REQUIRE(out[1000+6] == 1000); // > 500 so not modified
    REQUIRE(out[1015+6] == 1015); // > 500 so not modified
    REQUIRE(out[1029] == 1029 - 6); // > 500 so not modified
}

TEST_CASE("RawToConvertedGeometry_Transform","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2, {4,4,6,6}, 0,0, false);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(2) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    std::vector<uint32_t> input(x.GetModulesNum(3) * RAW_MODULE_SIZE);
    std::vector<uint32_t> output(x.GetPixelsNum());

    for (uint32_t i = 0; i < x.GetModulesNum(3) * RAW_MODULE_SIZE; i++) input[i] = i;

    for (int i = 0; i < x.GetModulesNum(3); i++)
        TransferModule<uint32_t, uint32_t>(output.data() + x.GetPixel0OfModule(i),
                                           input.data() + i * RAW_MODULE_SIZE,
                                           (x.IsUpsideDown() ? -1 : 1) * x.GetXPixelsNum());

    REQUIRE(output[0] == input[0]);
    REQUIRE(output[253] == input[253]);
    REQUIRE(output[256] == input[255]);
    REQUIRE(output[1029] == input[1023]);

    // Row just below middle row
    REQUIRE(output[254*(2*1030)+1006] == input[254*1024+1000]);

    // Middle row
    REQUIRE(output[255*(2*1030)+518] == input[255*1024+514]);
    REQUIRE(output[256*(2*1030)+518] == input[255*1024+514]);

    // Split corner (module 0)
    REQUIRE(output[255*(2*1030)+255] == input[255*1024+255]);
    REQUIRE(output[256*(2*1030)+255] == input[255*1024+255]);
    REQUIRE(output[255*(2*1030)+256] == input[255*1024+255]);
    REQUIRE(output[256*(2*1030)+256] == input[255*1024+255]);

    // Split corner (module 0)
    REQUIRE(output[258*(2*1030)+255] == input[256*1024+255]);
    REQUIRE(output[257*(2*1030)+255] == input[256*1024+255]);
    REQUIRE(output[258*(2*1030)+256] == input[256*1024+255]);
    REQUIRE(output[257*(2*1030)+256] == input[256*1024+255]);

    // Row just above middle row
    REQUIRE(output[259*(2*1030)+302] == input[257*1024+300]);

    // Split corner (module 1)
    REQUIRE(output[255*(2*1030)+1030+255] == input[512*1024+255*1024+255]);
    REQUIRE(output[256*(2*1030)+1030+255] == input[512*1024+255*1024+255]);
    REQUIRE(output[255*(2*1030)+1030+256] == input[512*1024+255*1024+255]);
    REQUIRE(output[256*(2*1030)+1030+256] == input[512*1024+255*1024+255]);

    // Module 1
    REQUIRE(output[1030+5] == input[512*1024 + 5]);
    REQUIRE(output[1030+256] == input[512*1024 + 255]);

    // Module 2
    REQUIRE(output[1030*514*2 + 256] == input[512*1024*2 + 255]);
}

TEST_CASE("RawToConvertedGeometry_Transform_AdjustMultipixels","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2, {4,4,6,6}, 0, 0, false);

    x.Mode(DetectorMode::Conversion);
    x.Summation(2); // Ensure the image is 32-bit
    REQUIRE(x.GetPixelDepth() == 4);

    REQUIRE(x.GetModulesNum(3) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    std::vector<int32_t> input(x.GetModulesNum(3) * RAW_MODULE_SIZE);
    std::vector<int32_t> output(x.GetPixelsNum());

    for (int32_t i = 0; i < x.GetModulesNum(3) * RAW_MODULE_SIZE; i++) input[i] = i % x.GetOverflow();

    for (int i = 0; i < x.GetModulesNum(3); i++)
        TransferModuleAdjustMultipixels<int32_t, int32_t>(output.data() + x.GetPixel0OfModule(i),
                                                          input.data() + i * RAW_MODULE_SIZE,
                                                          (x.IsUpsideDown() ? -1 : 1) * x.GetXPixelsNum(),
                                                          x.GetUnderflow() + 1, x.GetOverflow() - 1);

    REQUIRE(output[0] == input[0]);
    REQUIRE(output[253] == input[253]);
    REQUIRE(output[256] == input[255]/2);
    REQUIRE(output[1029] == input[1023]);

    // Row just below middle row
    REQUIRE(output[254*(2*1030)+1006] == input[254*1024+1000]);

    // Middle row
    REQUIRE(output[255*(2*1030)+518] == input[255*1024+514]/2);
    REQUIRE(output[256*(2*1030)+518] == input[255*1024+514]/2);

    // Split corner (module 0)
    REQUIRE(output[255*(2*1030)+255] == input[255*1024+255]/4);
    REQUIRE(output[256*(2*1030)+255] == input[255*1024+255]/4);
    REQUIRE(output[255*(2*1030)+256] == input[255*1024+255]/4);
    REQUIRE(output[256*(2*1030)+256] == input[255*1024+255]/4);

    // Split corner (module 0)
    REQUIRE(output[258*(2*1030)+255] == input[256*1024+255]/4);
    REQUIRE(output[257*(2*1030)+255] == input[256*1024+255]/4);
    REQUIRE(output[258*(2*1030)+256] == input[256*1024+255]/4);
    REQUIRE(output[257*(2*1030)+256] == input[256*1024+255]/4);

    // Row just above middle row
    REQUIRE(output[259*(2*1030)+302] == input[257*1024+300]);

    // Split corner (module 1)
    REQUIRE(output[255*(2*1030)+1030+255] == input[512*1024+255*1024+255]/4);
    REQUIRE(output[256*(2*1030)+1030+255] == input[512*1024+255*1024+255]/4);
    REQUIRE(output[255*(2*1030)+1030+256] == input[512*1024+255*1024+255]/4);
    REQUIRE(output[256*(2*1030)+1030+256] == input[512*1024+255*1024+255]/4);

    // Module 1
    REQUIRE(output[1030+5] == input[512*1024 + 5]);
    REQUIRE(output[1030+256] == input[512*1024 + 255] / 2);

    // Module 2
    REQUIRE(output[1030*514*2 + 256] == input[512*1024*2 + 255] / 2);
}

TEST_CASE("RawToConvertedGeometry_Transform_upside_down","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4,6,6}, 0, 0, true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(3) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    auto input = (uint32_t *) calloc(x.GetModulesNum(2) * RAW_MODULE_SIZE, sizeof(uint32_t));
    auto output = (uint32_t *) calloc(x.GetPixelsNum(), sizeof(uint32_t));
    for (uint32_t i = 0; i < x.GetModulesNum(3) * RAW_MODULE_SIZE; i++) input[i] = i;

    for (int i = 0; i < x.GetModulesNum(3); i++)
        TransferModule<uint32_t, uint32_t>(output + x.GetPixel0OfModule(i + x.GetFirstModuleOfDataStream(3)),
                                           input + i * RAW_MODULE_SIZE,
                                           (x.IsUpsideDown() ? -1 : 1) * x.GetXPixelsNum());

    REQUIRE(output[514*1030*4+0] == input[511*1024+0]);
    REQUIRE(output[514*1030*4+253] == input[511*1024+253]);
    REQUIRE(output[514*1030*4+256] == input[511*1024+255]);
    REQUIRE(output[514*1030*4+1029] == input[511*1024+1023]);

    // Row just below middle row
    REQUIRE(output[514*1030*4+254*(2*1030)+1006] == input[257*1024+1000]);

    // Middle row
    REQUIRE(output[514*1030*4+255*(2*1030)+518] == input[256*1024+514]);
    REQUIRE(output[514*1030*4+256*(2*1030)+518] == input[256*1024+514]);

    // Split corner (module 0)
    REQUIRE(output[514*1030*4+255*(2*1030)+255] == input[256*1024+255]);
    REQUIRE(output[514*1030*4+256*(2*1030)+255] == input[256*1024+255]);
    REQUIRE(output[514*1030*4+255*(2*1030)+256] == input[256*1024+255]);
    REQUIRE(output[514*1030*4+256*(2*1030)+256] == input[256*1024+255]);

    // Split corner (module 0)
    REQUIRE(output[514*1030*4+258*(2*1030)+255] == input[255*1024+255]);
    REQUIRE(output[514*1030*4+257*(2*1030)+255] == input[255*1024+255]);
    REQUIRE(output[514*1030*4+258*(2*1030)+256] == input[255*1024+255]);
    REQUIRE(output[514*1030*4+257*(2*1030)+256] == input[255*1024+255]);

    // Row just above middle row
    REQUIRE(output[514*1030*4+259*(2*1030)+302] == input[254*1024+300]);

    // Split corner (module 1)
    REQUIRE(output[514*1030*4+257*(2*1030)+1030+255] == input[512*1024+255*1024+255]);
    REQUIRE(output[514*1030*4+258*(2*1030)+1030+255] == input[512*1024+255*1024+255]);
    REQUIRE(output[514*1030*4+257*(2*1030)+1030+256] == input[512*1024+255*1024+255]);
    REQUIRE(output[514*1030*4+258*(2*1030)+1030+256] == input[512*1024+255*1024+255]);

    // Module 1
    REQUIRE(output[514*1030*4+1030+5] == input[512*1024 + 511*1024 + 5]);
    REQUIRE(output[514*1030*4+1030+256] == input[512*1024 + 511*1024 + 255]);

    // Module 2
    REQUIRE(output[1030*514*2 + 256] == input[512*1024*2 + 511*1024 + 255]);

    free(input);
    free(output);
}

TEST_CASE("RawToConvertedGeometry","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4,6,6}, 0, 0, true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(2) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    auto input = (uint32_t *) calloc(x.GetModulesNum() * RAW_MODULE_SIZE, sizeof(uint32_t));
    auto output = (uint32_t *) calloc(x.GetPixelsNum(), sizeof(uint32_t));

    auto input2 = (uint32_t *) calloc(x.GetModulesNum() * RAW_MODULE_SIZE, sizeof(uint32_t));

    for (uint32_t i = 0; i < x.GetModulesNum() * RAW_MODULE_SIZE; i++)
        input[i] = 3*i-1;

    RawToConvertedGeometry<uint32_t, uint32_t>(x, output, input);
    ConvertedToRawGeometry<uint32_t>(x, input2, output);

    uint64_t diff = 0;
    for (int i = 0; i < x.GetModulesNum(2) * RAW_MODULE_SIZE; i++) {
        if (input[i] != input2[i]) diff++;
    }
    REQUIRE(diff == 0);

    free(input);
    free(output);
    free(input2);
}

TEST_CASE("RawToConvertedGeometry_int64","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4,6,6}, 0, 0, true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(2) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    std::vector<uint32_t> input(x.GetModulesNum() * RAW_MODULE_SIZE);
    std::vector<uint64_t> output(x.GetPixelsNum());
    std::vector<uint64_t> input2(x.GetModulesNum() * RAW_MODULE_SIZE);

    for (uint32_t i = 0; i < x.GetModulesNum() * RAW_MODULE_SIZE; i++)
        input[i] = 3*i-1;

    RawToConvertedGeometry<uint64_t, uint32_t>(x, output.data(), input.data());
    ConvertedToRawGeometry<uint64_t>(x, input2.data(), output.data());

    uint64_t diff = 0;
    for (int i = 0; i < x.GetModulesNum(2) * RAW_MODULE_SIZE; i++) {
        if (input[i] != input2[i]) diff++;
    }
    REQUIRE(diff == 0);
}

TEST_CASE("RawToConvertedGeometry_FP","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4,6,6}, 0, 0, true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(2) == 6);
    REQUIRE(x.GetPixelsNum() == CONVERTED_MODULE_SIZE * x.GetModulesNum());

    std::vector<uint32_t> input(x.GetModulesNum() * RAW_MODULE_SIZE);
    std::vector<double> output(x.GetPixelsNum());
    std::vector<double> input2(x.GetModulesNum() * RAW_MODULE_SIZE);

    for (uint32_t i = 0; i < x.GetModulesNum() * RAW_MODULE_SIZE; i++)
        input[i] = 3*i-1;

    RawToConvertedGeometry<double, uint32_t>(x, output.data(), input.data());
    ConvertedToRawGeometry<double>(x, input2.data(), output.data());

    uint64_t diff = 0;
    for (int i = 0; i < x.GetModulesNum(2) * RAW_MODULE_SIZE; i++) {
        if (input[i] != Approx(input2[i])) diff++;
    }
    REQUIRE(diff == 0);
}

TEST_CASE("RawToConvertedGeometry_Gaps","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4}, 8, 36, true);

    x.Mode(DetectorMode::Conversion);
    REQUIRE(x.GetModulesNum(1) == 4);
    REQUIRE(x.GetXPixelsNum() == 2 * CONVERTED_MODULE_COLS + 8);
    REQUIRE(x.GetYPixelsNum() == 4 * CONVERTED_MODULE_LINES + 3*36);
    REQUIRE(x.GetPixelsNum() == x.GetXPixelsNum() * x.GetYPixelsNum());

    auto input = (uint32_t *) calloc(x.GetModulesNum() * RAW_MODULE_SIZE, sizeof(uint32_t));
    auto output = (uint32_t *) calloc(x.GetPixelsNum(), sizeof(uint32_t));

    auto input2 = (uint32_t *) calloc(x.GetModulesNum() * RAW_MODULE_SIZE, sizeof(uint32_t));

    for (uint32_t i = 0; i < x.GetModulesNum() * RAW_MODULE_SIZE; i++)
        input[i] = 3*i-1;

    RawToConvertedGeometry<uint32_t, uint32_t>(x, output, input);
    ConvertedToRawGeometry<uint32_t>(x, input2, output);

    uint64_t diff = 0;
    for (int i = 0; i < x.GetModulesNum() * RAW_MODULE_SIZE; i++) {
        if (input[i] != input2[i]) diff++;
    }
    REQUIRE(diff == 0);

    free(input);
    free(output);
    free(input2);
}

TEST_CASE("RawToConvertedGeometry_Coordinates","[RawToConvertedGeometry]") {
    DiffractionExperiment x(2,{4,4}, 8, 36, true);

    x.Mode(DetectorMode::Conversion);

    Coord x1_raw(0,0,0);
    Coord x1_conv = RawToConvertedCoordinate(x, 0, x1_raw);
    REQUIRE(x1_conv.x == 0 );
    REQUIRE(x1_conv.y == 3 * 514 + 513 + 36 * 3 );
    REQUIRE(x1_conv.z == 0);
    x1_conv = RawToConvertedCoordinate(x, 1, x1_raw);
    REQUIRE(x1_conv.x == 0 );
    REQUIRE(x1_conv.y == 514.0 + 513.0 + 36 );
    REQUIRE(x1_conv.z == 0);

    // Module #4
    Coord x2_raw(1020.5,3*512 + 257, 5.0);
    Coord x2_conv = RawToConvertedCoordinate(x, 0, x2_raw);
    REQUIRE(x2_conv.x == 1030 + 8 + 1020.5 + 6.0 );
    REQUIRE(x2_conv.y == 513.0 - (257 + 2) + 36.0 * 2 + 514.0 * 2);
    REQUIRE(x2_conv.z == 5.0);
    x2_conv = RawToConvertedCoordinate(x, 1, x2_raw);
    REQUIRE(x2_conv.x == 1030 + 8 + 1020.5 + 6.0 );
    REQUIRE(x2_conv.y == 513.0 - (257 + 2));
    REQUIRE(x2_conv.z == 5.0);
}

TEST_CASE("TransferPacketEIGER_uint16_TopLeft","[RawToConvertedGeometry]") {
    std::vector<uint16_t> destination(CONVERTED_MODULE_SIZE);
    std::vector<uint16_t> source(2048);

    std::mt19937 g1(345);
    std::uniform_int_distribution<uint16_t> dist;

    for (auto &i: source)
        i = dist(g1);

    TransferPacketEIGER(destination.data(), source.data(), CONVERTED_MODULE_COLS, 45);

    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS] == source[0]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 1] == source[1]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 500] == source[498]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 512] == source[510]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 513] == 0); // Masked

    CHECK(destination[(45 * 4 + 1) * CONVERTED_MODULE_COLS] == source[512]);
    CHECK(destination[(45 * 4 + 1) * CONVERTED_MODULE_COLS + 253] == source[512 + 253]);

    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 254] == source[512*2 + 254]);
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 255] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 256] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 257] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 258] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 259] == source[512*2 + 257]);

    CHECK(destination[(45 * 4 + 3) * CONVERTED_MODULE_COLS] == source[512*3]);
    CHECK(destination[(45 * 4 + 3) * CONVERTED_MODULE_COLS + 455] == source[512*3 + 453]);
}

TEST_CASE("TransferPacketEIGER_uint16_TopRight","[RawToConvertedGeometry]") {
    std::vector<uint16_t> destination(CONVERTED_MODULE_SIZE);
    std::vector<uint16_t> source(2048);

    std::mt19937 g1(346);
    std::uniform_int_distribution<uint16_t> dist;

    for (auto &i: source)
        i = dist(g1);

    TransferPacketEIGER(destination.data(), source.data(), CONVERTED_MODULE_COLS, 64+45);

    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 516] == 0); // Masked
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 516 + 1] == source[1]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 516 + 500] == source[498]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 516 + 512] == source[510]);
    CHECK(destination[45 * 4 * CONVERTED_MODULE_COLS + 516 + 513] == source[511]);

    CHECK(destination[(45 * 4 + 1) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 253] == source[512 + 253]);

    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 254] == source[512*2 + 254]);
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 255] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 256] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 257] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 258] == 0); // Masked
    CHECK(destination[(45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 259] == source[512*2 + 257]);

    CHECK(destination[(45 * 4 + 3) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(45 * 4 + 3) * CONVERTED_MODULE_COLS + 516 + 455] == source[512*3 + 453]);

    CHECK(destination[(45 * 4 + 3) * CONVERTED_MODULE_COLS + 1029] == source[2047]);
}

TEST_CASE("TransferPacketEIGER_uint16_BottomLeft","[RawToConvertedGeometry]") {
    std::vector<uint16_t> destination(CONVERTED_MODULE_SIZE);
    std::vector<uint16_t> source(2048);

    std::mt19937 g1(348);
    std::uniform_int_distribution<uint16_t> dist;

    for (auto &i: source)
        i = dist(g1);


    TransferPacketEIGER(destination.data(), source.data(), CONVERTED_MODULE_COLS, 128+45);

    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS] == source[0]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 1] == source[1]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 500] == source[498]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 512] == source[510]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 513] == 0); // Masked

    CHECK(destination[(258 + 45 * 4 + 2) * CONVERTED_MODULE_COLS] == source[512]);
    CHECK(destination[(258 + 45 * 4 + 2) * CONVERTED_MODULE_COLS + 253] == source[512 + 253]);

    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 254] == source[512*2 + 254]);
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 255] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 256] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 257] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 258] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 259] == source[512*2 + 257]);

    CHECK(destination[(258 + 45 * 4    ) * CONVERTED_MODULE_COLS] == source[512*3]);
    CHECK(destination[(258 + 45 * 4    ) * CONVERTED_MODULE_COLS + 455] == source[512*3 + 453]);
}

TEST_CASE("TransferPacketEIGER_uint16_BottomRight","[RawToConvertedGeometry]") {
    std::vector<uint16_t> destination(CONVERTED_MODULE_SIZE);
    std::vector<uint16_t> source(2048);

    std::mt19937 g1(349);
    std::uniform_int_distribution<uint16_t> dist;

    for (auto &i: source)
        i = dist(g1);

    TransferPacketEIGER(destination.data(), source.data(), CONVERTED_MODULE_COLS, 128+64+45);

    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 516] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 516 + 1] == source[1]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 516 + 500] == source[498]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 516 + 512] == source[510]);
    CHECK(destination[(258 + 45 * 4 + 3) * CONVERTED_MODULE_COLS + 516 + 513] == source[511]);

    CHECK(destination[(258 + 45 * 4 + 2) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 45 * 4 + 2) * CONVERTED_MODULE_COLS + 516 + 253] == source[512 + 253]);

    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 254] == source[512*2 + 254]);
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 255] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 256] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 257] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 258] == 0); // Masked
    CHECK(destination[(258 + 45 * 4 + 1) * CONVERTED_MODULE_COLS + 516 + 259] == source[512*2 + 257]);

    CHECK(destination[(258 + 45 * 4    ) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 45 * 4    ) * CONVERTED_MODULE_COLS + 516 + 455] == source[512*3 + 453]);
    CHECK(destination[(258 + 45 * 4    ) * CONVERTED_MODULE_COLS + 1029] == source[2047]);
}

TEST_CASE("TransferPacketEIGER_uint8_BottomRight","[RawToConvertedGeometry]") {
    std::vector<uint8_t> destination(CONVERTED_MODULE_SIZE);
    std::vector<uint8_t> source(4096);

    std::mt19937 g1(380);
    std::uniform_int_distribution<uint8_t> dist;

    for (auto &i: source)
        i = dist(g1);

    TransferPacketEIGER(destination.data(), source.data(), CONVERTED_MODULE_COLS, 64+32+15);

    CHECK(destination[(258 + 15 * 8 + 7) * CONVERTED_MODULE_COLS + 516] == 0); // Masked
    CHECK(destination[(258 + 15 * 8 + 7) * CONVERTED_MODULE_COLS + 516 + 1] == source[1]);
    CHECK(destination[(258 + 15 * 8 + 7) * CONVERTED_MODULE_COLS + 516 + 500] == source[498]);
    CHECK(destination[(258 + 15 * 8 + 7) * CONVERTED_MODULE_COLS + 516 + 512] == source[510]);
    CHECK(destination[(258 + 15 * 8 + 7) * CONVERTED_MODULE_COLS + 516 + 513] == source[511]);

    CHECK(destination[(258 + 15 * 8 + 6) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 15 * 8 + 6) * CONVERTED_MODULE_COLS + 516 + 253] == source[512 + 253]);

    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 254] == source[512*2 + 254]);
    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 255] == 0); // Masked
    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 256] == 0); // Masked
    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 257] == 0); // Masked
    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 258] == 0); // Masked
    CHECK(destination[(258 + 15 * 8 + 5) * CONVERTED_MODULE_COLS + 516 + 259] == source[512*2 + 257]);

    CHECK(destination[(258 + 15 * 8 + 4) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 15 * 8 + 4) * CONVERTED_MODULE_COLS + 516 + 455] == source[512*3 + 453]);
    CHECK(destination[(258 + 15 * 8 + 4) * CONVERTED_MODULE_COLS + 1029] == source[2047]);

    CHECK(destination[(258 + 15 * 8 + 2) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 15 * 8 + 2) * CONVERTED_MODULE_COLS + 516 + 302] == source[512*5 + 300]);
    CHECK(destination[(258 + 15 * 8 + 2) * CONVERTED_MODULE_COLS + 1029] == source[512*6-1]);

    CHECK(destination[(258 + 15 * 8    ) * CONVERTED_MODULE_COLS + 516] == 0);
    CHECK(destination[(258 + 15 * 8    ) * CONVERTED_MODULE_COLS + 516 + 154] == source[512*7 + 154]);
    CHECK(destination[(258 + 15 * 8    ) * CONVERTED_MODULE_COLS + 1029] == source[512*8-1]);
}