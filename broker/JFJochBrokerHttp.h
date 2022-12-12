// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHBROKERHTTP_H
#define JUNGFRAUJOCH_JFJOCHBROKERHTTP_H

#include <thread>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/client.h>

#include "JFJochStateMachine.h"

class JFJochBrokerHttp {
    constexpr static const uint16_t DefaultThreads = 4;
    std::string frontend_directory;

    Logger &logger;

    JFJochServices services;
    JFJochStateMachine state_machine {services};

    Pistache::Http::Endpoint server;
    Pistache::Rest::Router router;

    void SetRoutes();

    void Start(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Stop(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Pedestal(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Initialize(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Cancel(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Deactivate(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void Trigger(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetDataProcessingSettings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void PutDataProcessingSettings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetDataProcessingPlots(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetDetectorSettings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void PutDetectorSettings(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetMeasurementStatistics(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetCalibrationStatistics(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetStatus(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetBrokerStatusProtoBuf(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    static void ServeTIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter &response,
                   const std::string &s, size_t width, size_t height, size_t pixel_depth, bool is_signed);

    void GetPreviewTIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetPreviewDioptasTIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetPedestalG0TIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetPedestalG1TIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetPedestalG2TIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void GetNeXusMask(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    void GetStaticFile(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    static void AllowPut(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    static void AllowPost(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    static void NotFound(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
public:
    JFJochBrokerHttp(Logger &logger, uint16_t tcp_port, uint16_t nthreads = DefaultThreads);
    void Run();

    JFJochBrokerHttp& FrontendDirectory(const std::string& s);

    DiffractionExperiment& Experiment(); // Not protected via mutex, don't execute in parallel context !
    JFJochServices& Services();
};


#endif //JUNGFRAUJOCH_JFJOCHBROKERHTTP_H
