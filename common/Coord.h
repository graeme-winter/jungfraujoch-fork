// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INDEX_COORD_H
#define INDEX_COORD_H

#include <ostream>
#include <jfjoch.pb.h>

class Coord {
public:
    double x,y,z;
    Coord();
    Coord(const double in[3]);
    Coord(double x, double y, double z);

    Coord(const JFJochProtoBuf::Vector &v);
    operator JFJochProtoBuf::Vector() const;

    Coord operator+(const Coord &in) const;
    Coord operator-(const Coord &in) const;
    Coord operator*(double in) const;
    Coord operator/(double in) const;
    Coord operator-() const;

    Coord& operator+=(const Coord &in);
    Coord& operator-=(const Coord &in);
    Coord& operator*=(double in);
    Coord& operator/=(double in);

    bool operator==(const Coord &other) const;

    Coord operator%(const Coord &in) const; // Cross product
    double operator*(const Coord &in) const; // Dot product

    const double& operator[](int64_t val) const;
    double& operator[](int64_t val);

    double Length() const;
    Coord Normalize() const;

    friend std::ostream &operator<<( std::ostream &output, const Coord &in );
};

Coord operator*(double in1, const Coord& in2);
Coord operator*(const double M[3][3], const Coord& in);

Coord convert_spherical(double r, double theta, double phi);

double determinant(const Coord &in1, const Coord &in2, const Coord &in3);
double angle_deg(const Coord &in1, const Coord &in2);

Coord RodriguesRotation(const Coord &c, const Coord &k, double theta);

// |b2| <= |b1|
void PairReduction(const Coord b1, const Coord b2, Coord &a, Coord &b);
void SemaevReduction(Coord b1, Coord b2, Coord b3, Coord &a, Coord &b, Coord &c);

#endif //INDEX_COORD_H
