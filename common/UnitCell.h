// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_UNITCELL_H
#define JUNGFRAUJOCH_UNITCELL_H

#include "jfjoch.pb.h"
#include "Coord.h"

class UnitCell {
    JFJochProtoBuf::UnitCell cell;
public:
    UnitCell();
    UnitCell(double in_a, double in_b, double in_c, double in_alpha, double in_beta, double in_gamma);
    UnitCell(const JFJochProtoBuf::UnitCell &pbuf);

    operator JFJochProtoBuf::UnitCell() const;

    bool operator==(const UnitCell &other) const;
    bool operator!=(const UnitCell &other) const;
    bool empty() const;

    double a() const;
    double b() const;
    double c() const;

    double alpha() const;
    double beta() const;
    double gamma() const;
};


#endif //JUNGFRAUJOCH_UNITCELL_H
