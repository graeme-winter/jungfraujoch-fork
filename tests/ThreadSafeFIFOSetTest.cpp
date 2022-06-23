// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include <../common/ThreadSafeFIFO.h>

TEST_CASE("ThreadSafeFIFO","[ThreadSafeFIFO]") {
    ThreadSafeFIFO<uint32_t> fifo;
    uint32_t tmp;
    fifo.Put(0);
    fifo.Put(1);

    REQUIRE(fifo.Get(tmp) == 1);
    CHECK (tmp == 0);

    fifo.Put(0);

    REQUIRE(fifo.Get(tmp) == 1);
    CHECK (tmp == 1);

    REQUIRE(fifo.Get(tmp) == 1);
    CHECK (tmp == 0);

    REQUIRE(fifo.Get(tmp) == 0);
}


TEST_CASE("ThreadSafeFIFO_LimitedSize","[ThreadSafeFIFO]") {
    ThreadSafeFIFO<uint32_t> fifo(5);
    uint32_t tmp;
    REQUIRE(fifo.Put(0));
    REQUIRE(fifo.Put(1));
    REQUIRE(fifo.Put(2));
    REQUIRE(fifo.Put(3));
    REQUIRE(fifo.Put(4));
    REQUIRE(!fifo.Put(5));
    REQUIRE(fifo.Size() == 5);

    REQUIRE(fifo.Get(tmp) == 1);
    CHECK (tmp == 0);

    REQUIRE(fifo.Size() == 4);

    fifo.PutBlocking(5);
    REQUIRE(fifo.Size() == 5);
    fifo.GetBlocking();
    fifo.GetBlocking();
    fifo.GetBlocking();
    fifo.GetBlocking();
    fifo.GetBlocking();
}

TEST_CASE("ThreadSafeSet","[ThreadSafeFIFO]") {
    ThreadSafeSet<uint32_t> set;
    uint32_t tmp;
    set.Put(0);
    set.Put(1);

    REQUIRE(set.Get(tmp) == 1);
    CHECK (tmp == 0);

    set.Put(0);

    REQUIRE(set.Get(tmp) == 1);
    CHECK (tmp == 0);

    REQUIRE(set.Get(tmp) == 1);
    CHECK (tmp == 1);

    REQUIRE(set.Get(tmp) == 0);
}