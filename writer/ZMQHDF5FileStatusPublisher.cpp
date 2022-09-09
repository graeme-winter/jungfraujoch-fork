// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQHDF5FileStatusPublisher.h"

ZMQHDF5FileStatusPublisher::ZMQHDF5FileStatusPublisher(const DiffractionExperiment &experiment, ZMQContext &zmq_context,
                                                       const std::string &zmq_addr)
: socket(zmq_context, ZMQSocketType::Pub) {
    socket.SendWaterMark(10).NoLinger();
    socket.Bind(zmq_addr);

    json_template["xpixel"] = experiment.GetXPixelsNum();
    json_template["ypixel"] = experiment.GetYPixelsNum();

    json_template["beam_x"] = experiment.GetBeamX_pxl();
    json_template["beam_y"] = experiment.GetBeamY_pxl();
    json_template["det_distance"] = experiment.GetDetectorDistance_mm();

    if (experiment.HasUnitCell()) {
        json_template["space_group"] = experiment.GetSpaceGroupName();
        json_template["unit_cell"]["a"] = experiment.GetUnitCell().a();
        json_template["unit_cell"]["b"] = experiment.GetUnitCell().b();
        json_template["unit_cell"]["c"] = experiment.GetUnitCell().c();
        json_template["unit_cell"]["alpha"] = experiment.GetUnitCell().alpha();
        json_template["unit_cell"]["beta"] = experiment.GetUnitCell().beta();
        json_template["unit_cell"]["gamma"] = experiment.GetUnitCell().gamma();
    }
}

void ZMQHDF5FileStatusPublisher::Publish(const std::string &filename, size_t max_image_number) {
    nlohmann::json j = json_template;
    j["max_image_number"] = max_image_number;
    j["filename"]         = filename;
    socket.Send(j.dump());
}