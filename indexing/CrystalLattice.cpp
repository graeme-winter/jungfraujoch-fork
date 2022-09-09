// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include <gemmi/unitcell.hpp>
#include "CrystalLattice.h"

#define DEG_TO_RAD (M_PI/180.0)

CrystalLattice::CrystalLattice() {}

CrystalLattice::CrystalLattice(const JFJochProtoBuf::UnitCell &cell) {
    vec[0] = {cell.a(), 0, 0};
    vec[1] = {cell.b() * cos(cell.gamma() * DEG_TO_RAD), cell.b() * sin(cell.gamma() * DEG_TO_RAD), 0};
    double cx = cell.c() * cos(cell.beta() * DEG_TO_RAD);
    double cy = cell.c()
                * (cos(cell.alpha() * DEG_TO_RAD) - cos(cell.beta() * DEG_TO_RAD) * cos(cell.gamma() * DEG_TO_RAD))
                / sin(cell.gamma() * DEG_TO_RAD);
    vec[2] = {cx, cy, sqrt(cell.c()*cell.c()-cx*cx-cy*cy)};
}

CrystalLattice CrystalLattice::ReciprocalLattice() const {
    // Need transpose(A^-1)
    CrystalLattice l;
    auto det = vec[0] * (vec[1] % vec[2]);

    if (abs(det) > 1e-10) {
        l.vec[0] = (1 / det) * (vec[1] % vec[2]);
        l.vec[1] = (1 / det) * (vec[2] % vec[0]);
        l.vec[2] = (1 / det) * (vec[0] % vec[1]);
    }
    return l;
}

CrystalLattice CrystalLattice::Uncenter(char centering) const {
    if (centering == 'P')
        return *this;
    else {
        CrystalLattice l;
        auto c2p = gemmi::rot_as_mat33(gemmi::centred_to_primitive(centering));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                l.vec[i][j] = c2p[i][0] * vec[0][j] + c2p[i][1] * vec[1][j] + c2p[i][2] * vec[2][j];
            }
        }
        return l;
    }
}