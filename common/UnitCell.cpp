// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "UnitCell.h"

UnitCell::UnitCell() : cell() {}

UnitCell::UnitCell(double in_a, double in_b, double in_c, double in_alpha, double in_beta, double in_gamma) {
    cell.set_a(in_a);
    cell.set_b(in_b);
    cell.set_c(in_c);
    cell.set_alpha(in_alpha);
    cell.set_beta(in_beta);
    cell.set_gamma(in_gamma);
}

UnitCell::UnitCell(const JFJochProtoBuf::UnitCell &pbuf) { cell = pbuf; }
UnitCell::operator JFJochProtoBuf::UnitCell() const { return cell; }

double UnitCell::a() const { return cell.a();}
double UnitCell::b() const { return cell.b();}
double UnitCell::c() const { return cell.c();}

double UnitCell::alpha() const { return cell.alpha();}
double UnitCell::beta() const { return cell.beta();}
double UnitCell::gamma() const { return cell.gamma();}

bool UnitCell::operator==(const UnitCell &other) const {
    return (a() == other.a()) && (b() == other.b()) && (c() == other.c()) && (alpha() == other.alpha()) &&
           (beta() == other.beta()) && (gamma() == other.gamma());
}

bool UnitCell::operator!=(const UnitCell &other) const {
    return !operator==(other);
}

bool UnitCell::empty() const {
    return (*this == UnitCell());
}