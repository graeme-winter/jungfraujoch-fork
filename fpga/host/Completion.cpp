// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Completion.h"

// UINT64_MAX-1 is reserved for pedestal
const uint64_t Completion::FrameAfterFilterEnd = (UINT64_MAX-2);
const uint64_t Completion::FrameIgnore = (UINT64_MAX-3);
const uint64_t Completion::MeasurementDone = UINT64_MAX;