// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_CBORERR_H
#define JUNGFRAUJOCH_CBORERR_H

#include "tinycbor/src/cbor.h"
#include "../common/JFJochException.h"

inline void cborErr(CborError err) {
    if (err != CborNoError)
        throw JFJochException(JFJochExceptionCategory::CBORError, cbor_error_string(err), err);
}

#endif //JUNGFRAUJOCH_CBORERR_H
