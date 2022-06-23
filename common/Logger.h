// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_LOGGER_H
#define JUNGFRAUJOCH_LOGGER_H

#include <cstdint>
#include <chrono>
#include <iostream>
#include <mutex>
#include <memory>

namespace spdlog {
    class logger;
}

class Logger {
    mutable std::mutex m;

    std::shared_ptr<spdlog::logger> spdlog_logger;

    std::string service;
    std::string hostname;
public:
    Logger(const std::string &service_name, const std::string &file_name = "");
    void ErrorException(const std::exception &e);
    void Info(const std::string& msg);
    void Warning(const std::string& msg);
    void Error(const std::string& msg);
    void Debug(const std::string& msg);
    Logger& Verbose(bool input);
};

#endif //JUNGFRAUJOCH_LOGGER_H
