// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerHttp.h"
#include "../common/WriteTIFF.h"

void JFJochBrokerHttp::ServeTIFF(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter &response,
                                 const std::string &s, size_t width, size_t height, size_t pixel_depth, bool is_signed) {
    std::string s_copy = s;

    if (s_copy.size() != width * height * sizeof(uint16_t))
        response.send(Pistache::Http::Code::Internal_Server_Error, "Mismatch in image size",
                      Pistache::Http::Mime::MediaType::fromString("application/tiff"));

    std::string tmp = WriteTIFFToString(s_copy.data(), width, height, pixel_depth,
                                        is_signed);
    response.send(Pistache::Http::Code::Ok, tmp,
                  Pistache::Http::Mime::MediaType::fromString("application/tiff"));
}


void JFJochBrokerHttp::GetPreviewTIFF(const Pistache::Rest::Request &request,
                                      Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = services.GetPreviewFrame();
        ServeTIFF(request, response, tmp.data(), tmp.width(), tmp.height(), tmp.pixel_depth(), true);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetPreviewDioptasTIFF(const Pistache::Rest::Request &request,
                                      Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = services.GetPreviewFrame();

        if ((tmp.width() <= 0) || (tmp.height() <= 0))
            response.send(Pistache::Http::Code::Internal_Server_Error, "Weird size of PreviewFrame");

        auto src = (int16_t *) tmp.data().data();
        std::string dest(tmp.width() * tmp.height() * sizeof(int16_t),'0');
        auto dest16 = (uint16_t *) dest.data();

        // For Dioptas two operations needs to be performed:
        // 1. Flip image in the vertical direction
        // 2. Remove negatives and high counts

        for (int i = 0; i < tmp.width() * tmp.height(); i++) {
            size_t dest_x = i % tmp.width();
            size_t dest_y = tmp.height() - (i / tmp.width()) - 1;

            uint16_t val = src[i];
            if ((src[i] < 0) || (src[i] > 16384))
                val = 16384;

            dest16[dest_x + dest_y * tmp.width()] = val;
        }

        ServeTIFF(request, response, dest, tmp.width(), tmp.height(),
                  sizeof(uint16_t), false);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetPedestalG0TIFF(const Pistache::Rest::Request &request,
                                         Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = state_machine.GetPedestalG0();
        ServeTIFF(request, response, tmp.data(), tmp.width(), tmp.height(),
                  tmp.pixel_depth(),false);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetPedestalG1TIFF(const Pistache::Rest::Request &request,
                                         Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = state_machine.GetPedestalG1();
        ServeTIFF(request, response, tmp.data(), tmp.width(), tmp.height(),
                  tmp.pixel_depth(), false);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetPedestalG2TIFF(const Pistache::Rest::Request &request,
                                         Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = state_machine.GetPedestalG2();
        ServeTIFF(request, response, tmp.data(), tmp.width(), tmp.height(),
                  tmp.pixel_depth(), false);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void JFJochBrokerHttp::GetNeXusMask(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
    try {
        auto tmp = state_machine.GetNeXusMask();
        ServeTIFF(request, response, tmp.data(), tmp.width(), tmp.height(),
                  tmp.pixel_depth(), false);
    } catch (const JFJochException &e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}