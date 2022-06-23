// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_CRYSTALLATTICE_H
#define JUNGFRAUJOCH_CRYSTALLATTICE_H

#include "Coord.h"
#include "UnitCell.h"

struct CrystalLattice {
    Coord vec_a;
    Coord vec_b;
    Coord vec_c;
    CrystalLattice();
    CrystalLattice(const UnitCell &cell);
    CrystalLattice ReciprocalLattice() const;
};


#endif //JUNGFRAUJOCH_CRYSTALLATTICE_H
