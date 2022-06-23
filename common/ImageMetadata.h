// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_IMAGEMETADATA_H
#define JUNGFRAUJOCH_IMAGEMETADATA_H

struct ImageMetadata {
    uint32_t version;
    uint32_t frameid;
    uint64_t image_size;
};

#endif // JUNGFRAUJOCH_IMAGEMETADATA_H