// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/BackgroundEstimation.h"

TEST_CASE("BackgroundEstimation_Process","[BackgroundEstimation]") {
    DiffractionExperiment x(2, {4,4}, 8, 36);
    x.DetectorDistance_mm(75).Wavelength_A(1.0);
    x.LowResForBkgEstimation(50).HighResForBkgEstimation(2);

    std::vector<int16_t> v(x.GetPixelsNum(), 1);

    BackgroundEstimation est(x);

    REQUIRE(est.Process(v.data()).first == Approx(1.0));
}