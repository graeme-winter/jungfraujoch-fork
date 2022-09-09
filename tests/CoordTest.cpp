// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../common/Coord.h"

TEST_CASE("Coord_arithmetic", "[LinearAlgebra][Coord]") {
    Coord a{1,2,3};
    Coord b{3,4,5};

    REQUIRE(!(a==b));

    auto neg = -a;
    REQUIRE(neg.x == - a.x);
    REQUIRE(neg.y == - a.y);
    REQUIRE(neg.z == - a.z);

    auto sum = a + b;
    REQUIRE(sum.x == a.x + b.x);
    REQUIRE(sum.y == a.y + b.y);
    REQUIRE(sum.z == a.z + b.z);

    REQUIRE(a + b == b + a);

    auto diff = a - b;
    REQUIRE(diff.x == a.x - b.x);
    REQUIRE(diff.y == a.y - b.y);
    REQUIRE(diff.z == a.z - b.z);

    REQUIRE(a - b == -(b - a));

    auto scalar_mult = 5 * a;
    REQUIRE(scalar_mult.x == 5 * a.x);
    REQUIRE(scalar_mult.y == 5 * a.y);
    REQUIRE(scalar_mult.z == 5 * a.z);
    REQUIRE (5 * a == a * 5);

    auto scalar_div = a / 4;
    REQUIRE(scalar_div == a * 0.25);

    auto dot_mult = a * b;
    REQUIRE (dot_mult == a.x * b.x + a.y * b.y + a.z *b.z);
    REQUIRE (a * b == b * a);

    REQUIRE(Coord(1,0,0) % Coord(1,0,0) == Coord(0,0,0));
    REQUIRE(Coord(1,0,0) % Coord(0,1,0) == Coord(0,0,1));
    REQUIRE(Coord(0,1,0) % Coord(0,0,1) == Coord(1,0,0));
    REQUIRE(Coord(0,0,1) % Coord(1,0,0) == Coord(0,1,0));

    auto vec_mult = a % b;
    REQUIRE (a % b == -(b % a));


    REQUIRE(vec_mult.x == a.y * b.z - a.z * b.y);
    REQUIRE(vec_mult.y == a.z * b.x - a.x * b.z);
    REQUIRE(vec_mult.z == a.x * b.y - a.y * b.x);

    double M[3][3];
    M[0][0] = 1.0; M[0][1] = 2.0; M[0][2] = 3.0;
    M[1][1] = 4.0; M[1][1] = 5.0; M[1][2] = 6.0;
    M[2][2] = 7.0; M[2][2] = 8.0; M[2][2] = 9.0;

    auto matrix_mult = M * a;
    REQUIRE(matrix_mult.x == M[0][0] * a.x + M[0][1] * a.y + M[0][2] * a.z);
    REQUIRE(matrix_mult.y == M[1][0] * a.x + M[1][1] * a.y + M[1][2] * a.z);
    REQUIRE(matrix_mult.z == M[2][0] * a.x + M[2][1] * a.y + M[2][2] * a.z);
}

TEST_CASE("Coord_operator_bracket","[LinearAlgebra][Coord]") {
    Coord a{1,2,3};
    REQUIRE(a[0] == 1);
    REQUIRE(a[1] == 2);
    REQUIRE(a[2] == 3);
    REQUIRE_THROWS(a[4]);
    REQUIRE_THROWS(a[-1]);
    REQUIRE_THROWS(a[56667]);
}

TEST_CASE("Coord_normalize","[LinearAlgebra][Coord]") {
    Coord a{4,0,0};
    REQUIRE(a.Length() == 4.0);
    REQUIRE(a.Normalize().Length() == 1.0);
    REQUIRE(a.Normalize().x == 1.0);
    REQUIRE(a.Normalize().y == 0.0);
    REQUIRE(a.Normalize().z == 0.0);

    Coord b{3, 4, 5};
    REQUIRE(b.Length() == Approx(sqrt(3*3+4*4+5*5)));
    REQUIRE(b.Normalize().Length() == Approx(1.0));
    REQUIRE(b.Normalize().x == Approx(3 /sqrt(3*3+4*4+5*5)));
    REQUIRE(b.Normalize().y == Approx(4 /sqrt(3*3+4*4+5*5)));
    REQUIRE(b.Normalize().z == Approx(5 /sqrt(3*3+4*4+5*5)));
}

TEST_CASE("Rodrigues") {
    Coord a{7,0,1};

    Coord b{0,1,0};

    REQUIRE(RodriguesRotation(a, a, M_PI).x == Approx(a.x));

    REQUIRE(RodriguesRotation(a, b, M_PI/2.0).x == Approx(a.z));
    REQUIRE(RodriguesRotation(a, b, M_PI/2.0).y == Approx(a.y));
    REQUIRE(RodriguesRotation(a, b, M_PI/2.0).z == -Approx(a.x));

    REQUIRE(RodriguesRotation(a, b, M_PI).x == Approx(-a.x));
    REQUIRE(RodriguesRotation(a, b, M_PI).y == Approx(a.y));
}

TEST_CASE("Coord_Gaussian_Reduction") {
    Coord a{4, 0, 0};
    Coord b{0, 5, 0};
    Coord c{0, 0, 2};
    Coord x = a;
    Coord y = b + c;
    Coord z = a + b;

    Coord p, q, r;

    SemaevReduction(x, y, z, p, q, r);

    REQUIRE(p == -c);
    REQUIRE(q == a);
    REQUIRE(r == b);
}
