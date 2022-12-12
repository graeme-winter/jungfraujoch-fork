// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQPreviewPublisher.h"
#include "grpcToJson.h"

ZMQPreviewPublisher::ZMQPreviewPublisher(ZMQContext& context, const std::string& addr) :
        socket(context, ZMQSocketType::Pub) {
    socket.SendWaterMark(2).NoLinger();
    socket.Bind(addr);
}

void ZMQPreviewPublisher::Start(const DiffractionExperiment &experiment, const JFCalibration &calibration) {
    auto mask = calibration.CalculateNexusMask(experiment);
    JFJochProtoBuf::PreviewFrame frame;
    frame.set_image_number(-1);
    frame.set_width(experiment.GetXPixelsNum());
    frame.set_height(experiment.GetYPixelsNum());
    frame.set_pixel_depth(4);
    frame.set_data(mask.data(), experiment.GetPixelsNum() * sizeof(uint32_t));
    socket.Send(grpcToJson(frame));
}

void ZMQPreviewPublisher::Stop(const DiffractionExperiment& experiment) {}

void ZMQPreviewPublisher::Publish(const DiffractionExperiment& experiment, const int16_t* image_data, uint32_t image_number) {
    JFJochProtoBuf::PreviewFrame frame;
    frame.set_image_number(image_number);
    frame.set_total_images(experiment.GetImageNum());
    frame.set_wavelength_a(experiment.GetWavelength_A());
    frame.set_beam_x_pxl(experiment.GetBeamX_pxl());
    frame.set_beam_y_pxl(experiment.GetBeamY_pxl());
    frame.set_saturation_value(experiment.GetOverflow());
    frame.set_file_prefix(experiment.GetFilePrefix());
    frame.set_detector_distance_mm(experiment.GetDetectorDistance_mm());
    frame.set_width(experiment.GetXPixelsNum());
    frame.set_height(experiment.GetYPixelsNum());
    frame.set_pixel_depth(2);

    frame.set_data(image_data, experiment.GetPixelsNum() * sizeof(int16_t));

    SetPreviewImage(frame);
    socket.Send(grpcToJson(frame));
}

void ZMQPreviewPublisher::SetPreviewImage(const JFJochProtoBuf::PreviewFrame &frame) {
    std::unique_lock<std::mutex> ul(frame_mutex);
    saved_frame = frame;
}

JFJochProtoBuf::PreviewFrame ZMQPreviewPublisher::GetPreviewImage() const {
    std::unique_lock<std::mutex> ul(frame_mutex);
    return saved_frame;
}
