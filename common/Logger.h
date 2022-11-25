// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_LOGGER_H
#define JUNGFRAUJOCH_LOGGER_H

#include <cstdint>
#include <chrono>
#include <iostream>
#include <mutex>
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

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

    void Info(const std::string& msg) { spdlog_logger->info(msg); }
    void Warning(const std::string& msg) { spdlog_logger->warn(msg); }
    void Error(const std::string& msg) { spdlog_logger->error(msg); }
    void Debug(const std::string& msg) { spdlog_logger->debug(msg); }

    template<typename... Args>
    void Info(fmt::format_string<Args...> fmt, Args &&... args)  {
        spdlog_logger->info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(fmt::format_string<Args...> fmt, Args &&... args)  {
        spdlog_logger->error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Debug(fmt::format_string<Args...> fmt, Args &&... args)  {
        spdlog_logger->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warning(fmt::format_string<Args...> fmt, Args &&... args)  {
        spdlog_logger->warn(fmt, std::forward<Args>(args)...);
    }

    Logger& Verbose(bool input);
};

#endif //JUNGFRAUJOCH_LOGGER_H
