// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../broker/JFJochBrokerService.h"
#include "../common/JFJochException.h"

TEST_CASE("JFJochBrokerService_States") {
    Logger logger("JFJochBrokerService_States");
    JFJochBrokerService broker(logger);

    JFJochProtoBuf::BrokerSetup setup;

    JFJochProtoBuf::BrokerStatus status;
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::NOT_INITIALIZED);

    REQUIRE(!broker.Start(nullptr, &setup, nullptr).ok());

    REQUIRE(broker.Initialize(nullptr, nullptr, nullptr).ok());
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::IDLE);

    REQUIRE(broker.Deactivate(nullptr, nullptr, nullptr).ok());
    REQUIRE(broker.GetStatus(nullptr, nullptr, &status).ok());
    REQUIRE(status.current_state() == JFJochProtoBuf::BrokerStatus::NOT_INITIALIZED);
}