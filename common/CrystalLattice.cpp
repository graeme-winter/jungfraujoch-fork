// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CrystalLattice.h"
#include <cmath>
#define DEG_TO_RAD (M_PI/180.0)

CrystalLattice::CrystalLattice() {}

CrystalLattice::CrystalLattice(const UnitCell &cell) {
    vec_a = {cell.a(), 0, 0};
    vec_b = {cell.b() * cos(cell.gamma() * DEG_TO_RAD), cell.b() * sin(cell.gamma() * DEG_TO_RAD), 0};
    double cx = cell.c() * cos(cell.beta() * DEG_TO_RAD);
    double cy = cell.c()
                * (cos(cell.alpha() * DEG_TO_RAD) - cos(cell.beta() * DEG_TO_RAD) * cos(cell.gamma() * DEG_TO_RAD))
                / sin(cell.gamma() * DEG_TO_RAD);
    vec_c = {cx, cy, sqrt(cell.c()*cell.c()-cx*cx-cy*cy)};
}

CrystalLattice CrystalLattice::ReciprocalLattice() const {
    // Need transpose(A^-1)

    CrystalLattice l;
    auto det = vec_a * (vec_b % vec_c);

    if (abs(det) > 1e-10) {
        l.vec_a = (1 / det) * (vec_b % vec_c);
        l.vec_b = (1 / det) * (vec_c % vec_a);
        l.vec_c = (1 / det) * (vec_a % vec_b);
    }
    return l;
}