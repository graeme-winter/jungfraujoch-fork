// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/StatusVector.h"

TEST_CASE("StatusVector_GetStatus","[StatusVector]") {
    StatusVector<uint64_t> status_vector;
    status_vector.AddElement(5, 11);

    status_vector.AddElement(2000, 45);
    status_vector.AddElement(2543, 44);
    status_vector.AddElement(2600, 41);

    auto status_out = status_vector.GetStatus(1000);

    REQUIRE(status_out.size() == 3);
    REQUIRE(status_out[0] == 11);
    REQUIRE(status_out[1] == 0);
    REQUIRE(status_out[2] == Approx((45 + 44 + 41) / 3.0));

    status_out = status_vector.GetStatus(500);

    REQUIRE(status_out.size() == 6);
    REQUIRE(status_out[0] == 11);
    REQUIRE(status_out[1] == 0);
    REQUIRE(status_out[2] == 0);
    REQUIRE(status_out[3] == 0);
    REQUIRE(status_out[4] == Approx(45));
    REQUIRE(status_out[5] == Approx((44+41)/2.0));
}

TEST_CASE("StatusVector_Plot","[StatusVector]") {
    StatusVector<uint64_t> status_vector;
    status_vector.AddElement(5, 11);

    status_vector.AddElement(2000, 45);
    status_vector.AddElement(2543, 44);
    status_vector.AddElement(2600, 41);

    JFJochProtoBuf::Plot plot_out;
    status_vector.GetPlot(plot_out, 1000);
    REQUIRE(plot_out.x_size() == 3);
    REQUIRE(plot_out.y_size() == 3);
    REQUIRE(plot_out.x(0) == Approx(500));
    REQUIRE(plot_out.y(0) == Approx(11));
    REQUIRE(plot_out.x(2) == Approx(2500));
    REQUIRE(plot_out.y(2) == Approx((45 + 44 + 41) / 3.0));
}

TEST_CASE("StatusVector_Plot_OneBin","[StatusVector]") {
    StatusVector<uint64_t> status_vector;
    status_vector.AddElement(5, 11);

    JFJochProtoBuf::Plot plot_out;
    status_vector.GetPlot(plot_out, 1000);
    REQUIRE(plot_out.x_size() == 1);
    REQUIRE(plot_out.y_size() == 1);
    REQUIRE(plot_out.x(0) == Approx(2.5));
    REQUIRE(plot_out.y(0) == Approx(11));
}