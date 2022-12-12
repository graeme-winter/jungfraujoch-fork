// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <../common/WriteTIFF.h>

TEST_CASE("TIFFTest","[TIFF]") {
    std::vector<uint16_t> values(512*1024);
    REQUIRE_NOTHROW(WriteTIFFToString(values.data(), 1024, 512, 2));
}

TEST_CASE("TIFFTest_File","[TIFF]") {
    std::vector<uint16_t> values(512*1024);

    for (auto &i: values)
        i = 345;

    REQUIRE_NOTHROW(WriteTIFFToFile("test_image.tiff", values.data(), 1024, 512, 2));
}

TEST_CASE("TIFFTest_File_signed","[TIFF]") {
    std::vector<int16_t> values(512*1024);

    for (int i = 0; i < values.size(); i++) {
        values[i] = static_cast<int16_t>(((i % 2 == 0) ? 1 : -1) * i);
    }

    REQUIRE_NOTHROW(WriteTIFFToFile("test_image_signed.tiff", values.data(), 1024, 512,
                                    2, true));
}
