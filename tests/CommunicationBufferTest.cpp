// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/CommunicationBuffer.h"

#define TEST_NMODULES 5

TEST_CASE("CommunicationBuffer", "[CommBuffer]") {
    size_t buffer_size = 64 *1024L * 1024L;
    size_t image_size = TEST_NMODULES * RAW_MODULE_SIZE * sizeof(uint16_t);
    size_t location_size = image_size + sizeof(ImageMetadata);

    CommunicationBuffer buffer(buffer_size); // 64 MiB
    DiffractionExperiment experiment;
    experiment.DataStreamModuleSize(1,{TEST_NMODULES});
    experiment.Mode(DetectorMode::Raw);

    REQUIRE(buffer.GetBufferSize() == buffer_size);

    // Before setup all these need to throw exception
    REQUIRE_THROWS(buffer.GetImage(2));
    REQUIRE_THROWS(buffer.GetBufferLocationSize());
    REQUIRE_THROWS(buffer.GetBufferLocation(2));
    REQUIRE_THROWS(buffer.GetNumOfImages());
    REQUIRE_THROWS(buffer.Metadata(1));

    REQUIRE_NOTHROW(buffer.SetMaxImageSize(experiment.GetMaxCompressedSize()));

    REQUIRE(buffer.GetBufferLocationSize() == location_size);
    REQUIRE(buffer.GetNumOfImages() == buffer_size / location_size);

    REQUIRE(buffer.GetImage(2) - buffer.GetImage(0) == 2 * buffer.GetBufferLocationSize());
    REQUIRE(buffer.GetBufferLocation(2) - buffer.GetBufferLocation(0) == 2 * buffer.GetBufferLocationSize());
    REQUIRE(buffer.GetImage(2) - buffer.GetBufferLocation(2) == sizeof(ImageMetadata));
    REQUIRE_THROWS(buffer.GetImage(buffer.GetNumOfImages()));

    REQUIRE(buffer.GetBufferLocationID(buffer.GetBuffer() + location_size * 5) == 5);
    REQUIRE(buffer.GetBufferLocationID(buffer.GetImage(4)) == 4);

    REQUIRE_THROWS(buffer.GetBufferLocationID(buffer.GetBuffer() - 5));
    REQUIRE_THROWS(buffer.GetBufferLocationID(buffer.GetBuffer() + buffer_size + 1));
}

TEST_CASE("CommunicationBuffer_Cap", "[CommBuffer]") {
    size_t buffer_size = 64 * 1024L * 1024L;
    CommunicationBuffer buffer(buffer_size, 64); // 64 MiB

    buffer.SetMaxImageSize(2*1024L*1024L);
    REQUIRE(buffer.GetNumOfImages() == buffer_size / (2*1024L*1024L + sizeof(ImageMetadata)));

    buffer.SetMaxImageSize(1024L*1024L);
    REQUIRE(buffer.GetNumOfImages() == buffer_size / (1024L*1024L + sizeof(ImageMetadata)));

    buffer.SetMaxImageSize(792L*1024L);
    REQUIRE(buffer.GetNumOfImages() == 64);

    buffer.SetMaxImageSize(512L*1024L);
    REQUIRE(buffer.GetNumOfImages() == 64);

    buffer.SetMaxImageSize(1024L);
    REQUIRE(buffer.GetNumOfImages() == 64);
}
