// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiver.h"

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H
#define JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H

JFJochProtoBuf::JFJochReceiverOutput RunJFJochReceiverTest(std::vector<AcquisitionDevice *> &aq_devices, const DiffractionExperiment &x,
                                                           Logger &logger, JungfrauCalibration &calib, size_t &nframes,
                                                           std::vector<uint8_t> &image_with_metadata,
                                                           uint16_t nthreads, bool abort = false,
                                                           ZMQPreviewPublisher *in_preview_writer = nullptr);

bool JFJochReceiverTest(JFJochProtoBuf::JFJochReceiverOutput &output, Logger &logger,
                        std::vector<AcquisitionDevice *> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, const std::string &test_path, bool abort = false,
                        bool verbose = true, ZMQPreviewPublisher *in_preview_writer = nullptr);

bool JFJochReceiverTest(JFJochProtoBuf::JFJochReceiverOutput &output, Logger &logger,
                        std::vector<std::unique_ptr<AcquisitionDevice>> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, const std::string &test_path, bool abort = false,
                        bool verbose = true, ZMQPreviewPublisher *in_preview_writer = nullptr);

#endif //JUNGFRAUJOCH_JFJOCHRECEIVERTEST_H
