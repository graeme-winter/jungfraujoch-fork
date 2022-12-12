// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_WRITETIFF_H
#define JUNGFRAUJOCH_WRITETIFF_H

#include <string>

std::string WriteTIFFToString(void *buff, size_t cols, size_t lines, size_t elem_size, bool is_signed = false);
void WriteTIFFToFile(const std::string &filename, void *buff, size_t cols, size_t lines, size_t elem_size,
                     bool is_signed = false);

#endif //JUNGFRAUJOCH_WRITETIFF_H
