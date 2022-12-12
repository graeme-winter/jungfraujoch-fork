// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/StrongPixelSet.h"

TEST_CASE("DiffractionSpot_AddOperator","[StrongPixelSet]") {
    DiffractionSpot spot1(4,1,2,10), spot2(3,4,4,5);

    spot1 += spot2;

    // Spot2 is not changed
    REQUIRE(spot2.Count() == 5);

    REQUIRE(spot1.Count() == 15);
    REQUIRE(spot1.RawCoord().x == Approx((4*10+3*5)/15.0));
    REQUIRE(spot1.RawCoord().y == Approx((1*10+4*5)/15.0));
    REQUIRE(spot1.Frame() == Approx((2*10+4*5)/15.0));
    REQUIRE(spot1.Depth() == 3);
    REQUIRE(spot1.Size() == 2*4);
    REQUIRE(spot1.MaxCount() == 10);
    REQUIRE(spot1.PixelCount() == 2);
}

TEST_CASE("DiffractionSpot_LabCoord","[StrongPixelSet]") {
    DiffractionSpot spot(212, 2047, 12, 45);
    DiffractionExperiment experiment(2, {4, 4}, 8, 36, true);
    experiment.DetectorDistance_mm(30).BeamX_pxl(120)
    .BeamY_pxl(150.0).ScatteringVector(Coord(0,0,1));

    REQUIRE(spot.RawCoord().x == 212);
    REQUIRE(spot.RawCoord().y == 2047);
    REQUIRE(spot.Frame() == 12);

    REQUIRE(spot.Count() == 45);

    auto spot_transformed_0 = spot.TransformCoordinates(experiment, 0);
    auto spot_transformed_1 = spot.TransformCoordinates(experiment, 1);
    REQUIRE(spot_transformed_0.LabCoord(experiment, 0).x == (212 + CONVERTED_MODULE_COLS + 8 - 120) * PIXEL_SIZE_IN_MM);
    REQUIRE(spot_transformed_1.LabCoord(experiment, 1).x == (212 + CONVERTED_MODULE_COLS + 8 - 120) * PIXEL_SIZE_IN_MM);

    REQUIRE(spot_transformed_0.LabCoord(experiment, 0).y == (CONVERTED_MODULE_LINES * 2 + 36 * 2 - 150) * PIXEL_SIZE_IN_MM);
    REQUIRE(spot_transformed_1.LabCoord(experiment, 1).y == -150 * PIXEL_SIZE_IN_MM);
}


TEST_CASE("DiffractionSpot_ReciprocalCoord","[StrongPixelSet]") {
    double beam_center_x = 31 + 1000; // 1000
    double beam_center_y = (514 * 4 + 36 * 3) - 100;
    double detector_distance = 75.0 / sqrt(3);

    DiffractionSpot spot(31, 99, 180, 45);
    DiffractionExperiment experiment(2, {4, 4}, 8, 36, true);

    experiment.BeamX_pxl(beam_center_x).BeamY_pxl(beam_center_y).DetectorDistance_mm(detector_distance)
    .PhotonEnergy_keV(WVL_1A_IN_KEV/2.4).ScatteringVector(Coord(0,0,1));

    auto spot_transformed_0 = spot.TransformCoordinates(experiment, 0);

    Coord lab = spot_transformed_0.LabCoord(experiment, 0);

    REQUIRE(experiment.GetScatteringVector().z == 1.0 / 2.4);

    REQUIRE(lab.y == 0.0);
    REQUIRE(lab.x == -75.0);
    REQUIRE(lab.z == detector_distance);

    // 2 * theta = 60 deg
    // theta = 30 deg
    // sin(theta) = 0.5
    // d = lambda / (2*sin(theta))
    REQUIRE(spot_transformed_0.GetResolution(experiment, 0) == Approx(2.4));

    REQUIRE(spot_transformed_0.ReciprocalCoord(experiment, 0).x == -75.0 / (2.4 * lab.Length()));
    REQUIRE(spot_transformed_0.ReciprocalCoord(experiment, 0).y == 0);
    REQUIRE(spot_transformed_0.ReciprocalCoord(experiment, 0).z == Approx(75.0 / sqrt(3) / (2.4 * lab.Length()) - 1.0 / 2.4));
}

TEST_CASE("StrongPixelSet_coord2uint64_t","[StrongPixelSet]") {
    uint64_t val = strong_pixel_coord(15,6667,1024000);
    REQUIRE(col_from_strong_pixel(val) == 15);
    REQUIRE(line_from_strong_pixel(val) == 6667);
    REQUIRE(image_from_strong_pixel(val) == 1024000);
}

TEST_CASE("StrongPixelSet_BuildSpots","[StrongPixelSet]") {
    DiffractionExperiment experiment;
    experiment.Mode(DetectorMode::Raw);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_low_resolution_limit(200.0);
    settings.set_high_resolution_limit(0.5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);

    std::vector<DiffractionSpot> spots;
    StrongPixelSet strong_pixel_set;
    strong_pixel_set.AddStrongPixel(7,105,1);
    strong_pixel_set.AddStrongPixel(7,106,1);
    strong_pixel_set.AddStrongPixel(7,104,1);
    strong_pixel_set.AddStrongPixel(6,106,1);
    strong_pixel_set.AddStrongPixel(6,105,1);
    strong_pixel_set.AddStrongPixel(6,104,1);
    strong_pixel_set.AddStrongPixel(8,106,1);
    strong_pixel_set.AddStrongPixel(8,105,1);
    strong_pixel_set.AddStrongPixel(8,104,1);
    strong_pixel_set.AddStrongPixel(7,105,2);
    strong_pixel_set.FindSpots(experiment, settings, spots);
    REQUIRE(strong_pixel_set.Count() == 0);
    REQUIRE(spots.size() == 1);

    REQUIRE(spots[0].Count() == 9.0f);
    REQUIRE(spots[0].RawCoord().x == Approx(7.0));
    REQUIRE(spots[0].RawCoord().y == Approx(105.0));
    REQUIRE(spots[0].Frame() == Approx(1.0));
    REQUIRE(spots[0].Depth() == 1);
    REQUIRE(spots[0].Size() == 9);
}
