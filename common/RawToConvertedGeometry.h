// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_RAWTOCONVERTEDGEOMETRY_H
#define JUNGFRAUJOCH_RAWTOCONVERTEDGEOMETRY_H

#include <cmath>
#include "DiffractionExperiment.h"

// Take half of the number, but only if not bad pixel/overload
template <typename T> T half(T in, T min, T max) {
    T tmp = in;
    if ((in > min) && (in < max)) tmp /= 2;
    return tmp;
}

template <typename T> T quarter(T in, T min, T max) {
    T tmp = in;
    if ((in > min) && (in < max)) tmp /= 4;
    return tmp;
}

// Copy line, divide everything by 2 and extend multipixels + divide them by additional factor of 2
template <typename Td, typename Ts> void LineCopyAndAdjustMultipixelMidRow(Td *destination, const Ts* source, Ts min, Ts max) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++) {
            destination[i+chip*258] = half(source[i+chip*256], min, max);
        }
    }
    for (int i = 0; i < 3; i++) {
        destination[255+i*258] = quarter(source[255 + i*256], min, max);
        destination[256+i*258] = quarter(source[255 + i*256], min, max);
        destination[257+i*258] = quarter(source[256 + i*256], min, max);
        destination[258+i*258] = quarter(source[256 + i*256], min, max);
    }
}

// Copy line and extend multipixels + divide them by 2
template <typename Td, typename Ts> void LineCopyAndAdjustMultipixel(Td *destination, const Ts* source, Ts min, Ts max) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++) {
            destination[i + chip * 258] = source[i + chip * 256];
        }
    }
    for (int chip = 0; chip < 3; chip++) {
            destination[255+chip*258] = half(source[255 + chip*256], min, max);
            destination[256+chip*258] = half(source[255 + chip*256], min, max);
            destination[257+chip*258] = half(source[256 + chip*256], min, max);
            destination[258+chip*258] = half(source[256 + chip*256], min, max);
    }
}

template <class Td, class Ts> void TransferPacketAdjustMultipixels(Td *destination, const Ts *source, int64_t line_shift,
        int64_t packet_number, Ts min, Ts max) {
    for (int i = 0; i < 4; i++) {
        size_t line = packet_number * 4 + i;
        if ((line == 255) || (line == 256)) {
            LineCopyAndAdjustMultipixelMidRow<Td, Ts>(destination + line_shift * (line + 1),
                                                      source + RAW_MODULE_COLS * i, min, max);
            memcpy(destination + line_shift * (line + ((line > 255) ? 2 : 0)),
                   destination + line_shift * (line + 1),RAW_MODULE_COLS*sizeof(Td));
        } else {
            LineCopyAndAdjustMultipixel<Td, Ts>(destination + line_shift * (line + ((line > 255) ? 2 : 0)),
                                                source + RAW_MODULE_COLS * i, min, max);
        }
    }
}

template <class Td, class Ts> void TransferModuleAdjustMultipixels(Td *destination, const Ts *source, int64_t line_shift, Ts min, Ts max) {
    for (size_t line = 0; line < RAW_MODULE_LINES; line++) {
        if ((line == 255) || (line == 256)) {
            LineCopyAndAdjustMultipixelMidRow<Td, Ts>(destination + line_shift * (line + 1),
                                                      source + RAW_MODULE_COLS * line, min, max);
            memcpy(destination + line_shift * (line + ((line > 255) ? 2 : 0)),
                   destination + line_shift * (line + 1),RAW_MODULE_COLS*sizeof(Td));
        } else {
            LineCopyAndAdjustMultipixel<Td, Ts>(destination + line_shift * (line + ((line > 255) ? 2 : 0)),
                                                source + RAW_MODULE_COLS * line, min, max);
        }
    }
}

// Copy line and extend multipixels
template <typename Td, typename Ts> void LineCopyAndAddMultipixel(Td *destination, const Ts* source) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++)
            destination[i+chip*258] = source[i+chip*256];
    }
    for (int i = 0; i < 3; i++) {
        destination[256+i*258] = source[255 + i*256];
        destination[257+i*258] = source[256 + i*256];
    }
}

// Copy line and extend multipixels
template <typename Td, typename Ts> void LineCopyAndAddMultipixelHPerf(Td *destination, const Ts* source) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++) {
            if ((i + chip * 256 != 0) && (i+chip * 256 != 1030))
                destination[i + chip * 258 - 1] = source[i + chip * 256];
        }
    }
    for (int i = 0; i < 3; i++) {
        destination[256+i*258] = source[255 + i*256];
        destination[257+i*258] = source[256 + i*256];
    }
}

// Copy line and extend multipixels
template <typename Td, typename Ts> void LineCopyNoMultipixel(Td *destination, const Ts* source) {
    destination[0] = source[0];
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 1; i < 255; i++)
            destination[i+chip*258] = source[i+chip*256];
    }
    destination[1029] = source[1023];
}

template <class Td, class Ts> void TransferPacketNoMultipixel(Td *destination, const Ts *source, int64_t line_shift, int64_t packet_number) {
    for (int i = 0; i < 4; i++) {
        size_t line = packet_number * 4 + i;
        if ((line != 255) && (line != 256))
            LineCopyNoMultipixel<Td, Ts>(destination + line_shift * (line + ((line > 255) ? 2 : 0)),
                                         source + RAW_MODULE_COLS * i);
    }
}

template <class Td, class Ts> void TransferModule(Td *destination, const Ts *source, int64_t line_shift) {
    for (size_t line = 0; line < RAW_MODULE_LINES; line++) {
        if ((line == 255) || (line == 256)) {
            LineCopyAndAddMultipixel<Td, Ts>(destination + line_shift * (line + 1), source + RAW_MODULE_COLS * line);
            LineCopyAndAddMultipixel<Td, Ts>(destination + line_shift * (line + ((line > 255) ? 2 : 0)), source + RAW_MODULE_COLS * line);
        } else {
            LineCopyAndAddMultipixel<Td, Ts>(destination + line_shift * (line + ((line > 255) ? 2 : 0)), source + RAW_MODULE_COLS * line);
        }
    }
}

inline Coord RawToConvertedCoordinate(const DiffractionExperiment& experiment, uint16_t data_stream, const Coord &raw) {
    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        size_t module = experiment.GetFirstModuleOfDataStream(data_stream) + raw.y / RAW_MODULE_LINES;
        double line = raw.y - floor(raw.y / RAW_MODULE_LINES) * RAW_MODULE_LINES;
        double col = raw.x;

        line += floor(line / 256) * 2;
        col += floor(col / 256) * 2;

        Coord out(experiment.GetPixel0OfModule(module) % experiment.GetXPixelsNum(),
                  experiment.GetPixel0OfModule(module) / experiment.GetXPixelsNum(),
                  raw.z);
        out.x += col;
        out.y += (experiment.IsUpsideDown() ? -1 : 1) * line;

        return out;
    } else return raw;
}

// Input coord is column + 1024 * (line + 512 * module)
template <class Td, class Ts> void RawToConvertedGeometry(const DiffractionExperiment& experiment, Td *destination, const Ts *source) {
    for (size_t module = 0; module < experiment.GetModulesNum(); module++)
        TransferModule<Td, Ts>(destination + experiment.GetPixel0OfModule(module),
                               source + module * RAW_MODULE_SIZE,
                               (experiment.IsUpsideDown() ? -1 : 1) * experiment.GetXPixelsNum());
}


// Input coord is column + 1024 * (line + 512 * module)
template <class Td, class Ts> void RawToConvertedGeometryAdjustMultipixels(const DiffractionExperiment& experiment, Td *destination, const Ts *source) {
    Ts min = experiment.GetUnderflow() + 1;
    if (min > 0) min = 0;
    Ts max = experiment.GetOverflow() - 1;

    for (size_t module = 0; module < experiment.GetModulesNum(); module++) {
        TransferModuleAdjustMultipixels<Td, Ts>(destination + experiment.GetPixel0OfModule(module),
                                                source + module * RAW_MODULE_SIZE,
                                                (experiment.IsUpsideDown() ? -1 : 1) * experiment.GetXPixelsNum(), min, max);
    }
}

template <class T> void LineConvtToRaw(T *destination, const T* source) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++)
            destination[i+chip*256] = source[i+chip*258];
    }
}

template <class T> void LineConvtToRawHPerf(T *destination, const T* source) {
    for (int chip = 0; chip < 4; chip++) {
        for (int i = 0; i < 256; i++) {
            if ((i + chip * 256 != 0) && (i + chip * 256 != 1023))
                destination[i + chip * 256] = source[i + chip * 258 - 1];
        }
    }
}

template <class T> void ConvertedToRawGeometry(const DiffractionExperiment& experiment, T *destination, const T* source) {
    for (size_t module = 0; module < experiment.GetModulesNum(); module++) {
        for (size_t line = 0; line < RAW_MODULE_LINES; line++) {
            LineConvtToRaw<T>(destination + module * RAW_MODULE_SIZE + RAW_MODULE_COLS * line,
                              source + experiment.GetPixel0OfModule(module) + (experiment.IsUpsideDown() ? -1 : 1) * experiment.GetXPixelsNum() * (line + ((line > 255) ? 2 : 0))
                              );
        }
    }
}

#endif