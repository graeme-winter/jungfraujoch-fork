// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/bin_to_hex.h"

#include "Logger.h"
#include "GitInfo.h"

Logger::Logger(const std::string &service_name, const std::string &file_name) {

    std::vector<spdlog::sink_ptr> sinks;

    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    if (!file_name.empty())
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(file_name, 23, 59));

    spdlog_logger = std::make_shared<spdlog::logger>(service_name, std::begin(sinks), std::end(sinks));
    spdlog_logger->info("Git sha {} ({})", jfjoch_git_sha1().substr(0, 6), jfjoch_git_date());
}

void Logger::ErrorException(const std::exception &e) {
    spdlog_logger->error(e.what());
}

Logger &Logger::Verbose(bool input) {
    if (input)
        spdlog_logger->set_level(spdlog::level::debug);
    else
        spdlog_logger->set_level(spdlog::level::info);
    return *this;
}