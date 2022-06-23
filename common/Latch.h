// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_LATCH_H
#define JUNGFRAUJOCH_LATCH_H

#include <mutex>
#include <condition_variable>

class Latch {
    std::mutex m;
    std::condition_variable c;
    uint32_t count;
public:
    Latch(uint32_t count);
    void CountDown();
    void Wait();
};


#endif //JUNGFRAUJOCH_LATCH_H
