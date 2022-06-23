// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <unistd.h>

#include <arpa/inet.h>

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "JFJochException.h"
#include "DiffractionExperiment.h"
#include "Logger.h"
#include "GitInfo.h"
#include <spdlog/spdlog.h>

void Logger::Info(const std::string& msg)    { spdlog_logger->info(msg);  }
void Logger::Warning(const std::string& msg) { spdlog_logger->warn(msg);  }
void Logger::Error(const std::string& msg)   { spdlog_logger->error(msg); }
void Logger::Debug(const std::string& msg)   { spdlog_logger->debug(msg); }

inline uint64_t now_in_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Logger::Logger(const std::string &service_name, const std::string &file_name) {

    std::vector<spdlog::sink_ptr> sinks;

    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    if (!file_name.empty())
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(file_name, 23, 59));

    spdlog_logger = std::make_shared<spdlog::logger>(service_name, std::begin(sinks), std::end(sinks));
    spdlog_logger->info("Git sha " + jfjoch_git_sha1().substr(0, 6) + " (" + jfjoch_git_date() + ")");
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