// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Latch.h"

Latch::Latch(uint32_t in_count) {
    count = in_count;
}

void Latch::CountDown() {
    std::unique_lock<std::mutex> ul(m);
    if (count != 0)
        count--;
    if (count == 0)
        c.notify_all();
}

void Latch::Wait() {
    std::unique_lock<std::mutex> ul(m);
    c.wait(ul, [&]{return count == 0;});
}