// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_MAKEDIRECTORY_H
#define JUNGFRAUJOCH_MAKEDIRECTORY_H

#include <filesystem>
#include "../common/JFJochException.h"

inline void MakeDirectory(const std::string &input) {
    try {
        std::filesystem::path path(input);
        if (path.has_parent_path())
            std::filesystem::create_directories(path.parent_path());
    } catch (const std::filesystem::filesystem_error &err) {
        throw JFJochException(JFJochExceptionCategory::FileWriteError,
                              "Cannot create subdirectory for file " + input + ": " + std::string(err.what()));
    }
}

#endif //JUNGFRAUJOCH_MAKEDIRECTORY_H
