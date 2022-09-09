// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_CRYSTALLATTICE_H
#define JUNGFRAUJOCH_CRYSTALLATTICE_H

#include "../common/Coord.h"

struct CrystalLattice {
    Coord vec[3];
    CrystalLattice();
    CrystalLattice(const JFJochProtoBuf::UnitCell &cell);
    CrystalLattice ReciprocalLattice() const;
    CrystalLattice Uncenter(char centering) const;
};


#endif //JUNGFRAUJOCH_CRYSTALLATTICE_H
