// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WriteTIFF.h"
#include "JFJochException.h"

#include <tiffio.h>
#include <tiffio.hxx>
#include <sstream>

void WriteTIFF(TIFF *tiff, void *buff, size_t cols, size_t lines, size_t elem_size, bool is_signed) {
    if (tiff == nullptr)
        throw JFJochException(JFJochExceptionCategory::TIFFGeneratorError, "TIFFStreamOpen error");

    TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, cols);  // set the width of the image
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, lines);    // set the height of the image
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);   // set number of channels per pixel
    TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, elem_size * 8);    // set the size of the channels
    TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW); // setc ompression to LZW
    TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, lines);
    if (is_signed)
        TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
    else
        TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

    if (TIFFWriteEncodedStrip(tiff, 0, buff,cols * lines * elem_size) < 0)
        throw JFJochException(JFJochExceptionCategory::TIFFGeneratorError, "TIFFWriteEncodedStrip error");

}

std::string WriteTIFFToString(void *buff, size_t cols, size_t lines, size_t elem_size, bool is_signed) {
    std::stringstream os;

    TIFF *tiff = TIFFStreamOpen("x", (std::ostream *) &os);
    WriteTIFF(tiff, buff, cols, lines, elem_size, is_signed);
    TIFFClose(tiff);

    return os.str();
}

void WriteTIFFToFile(const std::string &filename, void *buff, size_t cols, size_t lines, size_t elem_size,
                     bool is_signed) {
    TIFF *tiff = TIFFOpen(filename.c_str(), "w");

    WriteTIFF(tiff, buff, cols, lines, elem_size, is_signed);

    TIFFClose(tiff);
}
