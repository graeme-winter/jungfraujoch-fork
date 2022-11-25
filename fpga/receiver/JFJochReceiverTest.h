// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiver.h"

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H
#define JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H

JFJochProtoBuf::ReceiverOutput RunJFJochReceiverTest(std::vector<AcquisitionDevice *> &aq_devices, ImagePusher &pusher,
                                                     const DiffractionExperiment &x, Logger &logger, JFCalibration &calib,
                                                     uint16_t nthreads, bool abort = false,
                                                     ZMQPreviewPublisher *in_preview_writer = nullptr);

bool JFJochReceiverTest(JFJochProtoBuf::ReceiverOutput &output, Logger &logger,
                        std::vector<AcquisitionDevice *> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, bool abort = false,
                        bool verbose = true, ZMQPreviewPublisher *in_preview_writer = nullptr);

bool JFJochReceiverTest(JFJochProtoBuf::ReceiverOutput &output, Logger &logger,
                        std::vector<std::unique_ptr<AcquisitionDevice>> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, bool abort = false,
                        bool verbose = true, ZMQPreviewPublisher *in_preview_writer = nullptr);

#endif //JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H
