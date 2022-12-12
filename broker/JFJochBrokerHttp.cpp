// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include "JFJochBrokerHttp.h"

JFJochBrokerHttp::JFJochBrokerHttp(Logger &in_logger, uint16_t tcp_port, uint16_t nthreads)
: logger(in_logger), services(in_logger),
server(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(tcp_port))) {
    auto opts = Pistache::Http::Endpoint::options().threads(nthreads);
    server.init(opts);
    SetRoutes();
}

DiffractionExperiment &JFJochBrokerHttp::Experiment() {
    return state_machine.NotThreadSafe_Experiment();
}

JFJochServices &JFJochBrokerHttp::Services() {
    return services;
}

void JFJochBrokerHttp::Run() {
    try {
        logger.Info("Starting server...");
        server.serve();
    } catch (const std::runtime_error &e) {
        logger.Error(e.what());
        logger.Warning("Most likely: TCP/IP port busy, retrying in 70 seconds");
        std::this_thread::sleep_for(std::chrono::seconds(70));
        logger.Info("Starting server...");
        server.serve();
    }
}

#define POST(addr, function)  Pistache::Rest::Routes::Post(router, addr, Pistache::Rest::Routes::bind(&function, this)); \
                             Pistache::Rest::Routes::Options(router, addr, Pistache::Rest::Routes::bind(&JFJochBrokerHttp::AllowPost));
#define PUT(addr, function)  Pistache::Rest::Routes::Put(router, addr, Pistache::Rest::Routes::bind(&function, this)); \
                             Pistache::Rest::Routes::Options(router, addr, Pistache::Rest::Routes::bind(&JFJochBrokerHttp::AllowPut));
#define GET(addr, function)  Pistache::Rest::Routes::Get(router, addr, Pistache::Rest::Routes::bind(&function, this));

void JFJochBrokerHttp::SetRoutes() {
    POST("/detector/start", JFJochBrokerHttp::Start);
    POST("/detector/stop", JFJochBrokerHttp::Stop);
    POST("/detector/pedestal", JFJochBrokerHttp::Pedestal);
    POST("/detector/initialize", JFJochBrokerHttp::Initialize);
    POST("/detector/cancel", JFJochBrokerHttp::Cancel);
    POST("/detector/deactivate", JFJochBrokerHttp::Deactivate);
    POST("/detector/trigger", JFJochBrokerHttp::Trigger);

    GET("/detector/status", JFJochBrokerHttp::GetStatus);

    PUT("/detector/settings", JFJochBrokerHttp::PutDetectorSettings);
    GET("/detector/settings", JFJochBrokerHttp::GetDetectorSettings);

    GET("/detector/calibration", JFJochBrokerHttp::GetCalibrationStatistics);
    GET("/detector/last_measurement", JFJochBrokerHttp::GetMeasurementStatistics);

    PUT("/data_processing/settings", JFJochBrokerHttp::PutDataProcessingSettings);
    GET("/data_processing/settings", JFJochBrokerHttp::GetDataProcessingSettings);
    GET("/data_processing/plots", JFJochBrokerHttp::GetDataProcessingPlots);

    GET("/image/preview.tiff", JFJochBrokerHttp::GetPreviewTIFF);
    GET("/image/preview_dioptas.tiff", JFJochBrokerHttp::GetPreviewDioptasTIFF);
    GET("/calibration/pedestalG0.tiff", JFJochBrokerHttp::GetPedestalG0TIFF);
    GET("/calibration/pedestalG1.tiff", JFJochBrokerHttp::GetPedestalG1TIFF);
    GET("/calibration/pedestalG2.tiff", JFJochBrokerHttp::GetPedestalG2TIFF);
    GET("/calibration/mask.tiff", JFJochBrokerHttp::GetNeXusMask);

    GET("/", JFJochBrokerHttp::GetStaticFile);
    GET("/frontend", JFJochBrokerHttp::GetStaticFile);
    GET("/frontend/*", JFJochBrokerHttp::GetStaticFile);
    GET("/frontend/static/css/*", JFJochBrokerHttp::GetStaticFile);
    GET("/frontend/static/js/*", JFJochBrokerHttp::GetStaticFile);

    router.addNotFoundHandler(Pistache::Rest::Routes::bind(&JFJochBrokerHttp::NotFound));

    server.setHandler(router.handler());
}

void JFJochBrokerHttp::AllowPut(const Pistache::Rest::Request &request,
                             Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    response.headers().add<Pistache::Http::Header::AccessControlAllowMethods>("PUT");
    response.send(Pistache::Http::Code::No_Content);
}

void JFJochBrokerHttp::AllowPost(const Pistache::Rest::Request &request,
                                Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    response.headers().add<Pistache::Http::Header::AccessControlAllowMethods>("POST");
    response.send(Pistache::Http::Code::No_Content);
}

void JFJochBrokerHttp::NotFound(const Pistache::Rest::Request &request,
                            Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found);
}

void JFJochBrokerHttp::GetStaticFile(const Pistache::Rest::Request &request,
                                     Pistache::Http::ResponseWriter response) {
    if (!frontend_directory.empty()) {
        logger.Info("Requesting static resource {} from {}", request.resource(), frontend_directory);

        if ((request.resource() == "/")
            || (request.resource() == "/frontend")
            || (request.resource() == "/frontend/"))
            Pistache::Http::serveFile(response, frontend_directory + "/index.html");
        else if (request.resource().substr(0, 10) == "/frontend/")
            Pistache::Http::serveFile(response, frontend_directory + "/"
                + request.resource().substr(10));
        else NotFound(request, std::move(response));
    } else NotFound(request, std::move(response));
}

JFJochBrokerHttp &JFJochBrokerHttp::FrontendDirectory(const std::string &s) {
    frontend_directory = s;
    if (!std::filesystem::exists(frontend_directory) || !std::filesystem::is_directory(frontend_directory))
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Path " + frontend_directory + " doesn't exist or is not a directory");
    return *this;
}
