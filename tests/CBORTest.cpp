// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include "../frame_serialize/JFJochFrameSerializer.h"
#include "../frame_serialize/JFJochFrameDeserializer.h"

TEST_CASE("CBORSerialize_Start", "[CBOR]") {
    JFJochFrameSerializer serializer(8*1024*1024);

    size_t image_size = 0;
    REQUIRE_NOTHROW(image_size = serializer.SerializeSequenceStart());
    REQUIRE (image_size > 0);

    auto image = serializer.GetBuffer();
    image.resize(image_size);

    JFJochFrameDeserializer deserializer;
    REQUIRE_NOTHROW(deserializer.Process(image));
    REQUIRE(deserializer.GetType() == JFJochFrameDeserializer::Type::START);
}

TEST_CASE("CBORSerialize_End", "[CBOR]") {
    JFJochFrameSerializer serializer(8*1024*1024);

    size_t image_size = 0;
    REQUIRE_NOTHROW(image_size = serializer.SerializeSequenceEnd());
    REQUIRE (image_size > 0);

    auto image = serializer.GetBuffer();
    image.resize(image_size);

    JFJochFrameDeserializer deserializer;
    REQUIRE_NOTHROW(deserializer.Process(image));
    REQUIRE(deserializer.GetType() == JFJochFrameDeserializer::Type::END);
}

TEST_CASE("CBORSerialize_Image", "[CBOR]") {
    JFJochFrameSerializer serializer(8*1024*1024);

    std::vector<SpotToSave> spots;

    std::vector<uint8_t> test(1024);
    for (int i = 0; i < test.size(); i++)
        test[i] = (i * 253 + 56) % 256;

    size_t serialized_size = 0;
    REQUIRE_NOTHROW(serialized_size = serializer.SerializeImage(test.data(), test.size(),
                                                           std::make_pair<uint64_t, uint64_t>(456,789),
                                                                   spots));
    REQUIRE (serialized_size > 0);

    auto serialized = serializer.GetBuffer();
    serialized.resize(serialized_size);

    JFJochFrameDeserializer deserializer;
    REQUIRE_NOTHROW(deserializer.Process(serialized));
    REQUIRE(deserializer.GetType() == JFJochFrameDeserializer::Type::IMAGE);
    REQUIRE(deserializer.GetFileNumber() == 456);
    REQUIRE(deserializer.GetImageNumber() == 789);
    REQUIRE(deserializer.GetImageSize() == test.size());
    REQUIRE(memcmp(deserializer.GetImageData(), test.data(), test.size()) == 0);
}

TEST_CASE("CBORSerialize_Image_Spots", "[CBOR]") {
    JFJochFrameSerializer serializer(8*1024*1024);

    std::vector<SpotToSave> spots;
    spots.push_back(SpotToSave{.x = 7, .y = 8, .intensity = 34});
    spots.push_back(SpotToSave{.x = 37, .y = 48, .intensity = 123});

    std::vector<uint8_t> test(1024);
    for (int i = 0; i < test.size(); i++)
        test[i] = (i * 253 + 56) % 256;

    size_t serialized_size = 0;
    REQUIRE_NOTHROW(serialized_size = serializer.SerializeImage(test.data(), test.size(),
                                                                std::make_pair<uint64_t, uint64_t>(456,789),
                                                                spots));
    REQUIRE (serialized_size > 0);

    auto serialized = serializer.GetBuffer();
    serialized.resize(serialized_size);

    JFJochFrameDeserializer deserializer;
    REQUIRE_NOTHROW(deserializer.Process(serialized));
    REQUIRE(deserializer.GetType() == JFJochFrameDeserializer::Type::IMAGE);
    REQUIRE(deserializer.GetFileNumber() == 456);
    REQUIRE(deserializer.GetImageNumber() == 789);
    REQUIRE(deserializer.GetImageSize() == test.size());
    REQUIRE(memcmp(deserializer.GetImageData(), test.data(), test.size()) == 0);
    REQUIRE(deserializer.GetSpots().size() == 2);

    REQUIRE(deserializer.GetSpots()[0].intensity == 34);

    REQUIRE(deserializer.GetSpots()[1].x == 37);
    REQUIRE(deserializer.GetSpots()[1].y == 48);
    REQUIRE(deserializer.GetSpots()[1].intensity == 123);
}

TEST_CASE("CBORSerialize_Image_Spots_NLohmann", "[CBOR]") {
    JFJochFrameSerializer serializer(8*1024*1024);

    std::vector<SpotToSave> spots;
    spots.push_back(SpotToSave{.x = 7, .y = 8, .intensity = 34});
    spots.push_back(SpotToSave{.x = 37, .y = 48, .intensity = 123});

    std::vector<uint8_t> test(1024);
    for (int i = 0; i < test.size(); i++)
        test[i] = (i * 253 + 56) % 256;

    size_t serialized_size = 0;
    REQUIRE_NOTHROW(serialized_size = serializer.SerializeImage(test.data(), test.size(),
                                                                std::make_pair<uint64_t, uint64_t>(456,789),
                                                                spots));
    REQUIRE (serialized_size > 0);

    auto serialized = serializer.GetBuffer();
    serialized.resize(serialized_size);

    nlohmann::json j ;
    REQUIRE_NOTHROW(j = nlohmann::json::from_cbor(serialized));
    REQUIRE(j["image_number"] == 789);
    REQUIRE(j["file_number"] == 456);
    REQUIRE(j["type"] == "image");
    REQUIRE(j["spots"].is_array());
    REQUIRE(j["spots"].size() == 6);
    REQUIRE(j["data"].is_binary());
    REQUIRE(j["data"].get_binary().size() == 1024);
}