// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVER_H
#define JUNGFRAUJOCH_JFJOCHRECEIVER_H

#include <condition_variable>
#include <atomic>
#include <future>

#include "../../common/DiffractionExperiment.h"
#include "../../common/JFJochException.h"
#include "../../common/FrameTransformation.h"
#include "../../common/StrongPixelSet.h"

#include "AcquisitionDevice.h"
#include "../../common/Logger.h"
#include "../../common/CommunicationBuffer.h"
#include "../../common/ThreadSafeFIFO.h"
#include "../../common/ZMQImagePusher.h"
#include "../../common/ZMQPreviewPublisher.h"
#include "../../common/ZMQSpotPublisher.h"

#include "../../common/BackgroundEstimation.h"
#include "../../common/Latch.h"

#define RMS_THRESHOLD_FOR_MASK (100)
#define ALLOWED_WRONG_PEDESTAL_FOR_MASK (2)

class JFJochReceiver {
    DiffractionExperiment experiment;
    JungfrauCalibration calib;
    Logger &logger;

    std::vector<std::future<int64_t>> frame_transformation_futures;
    std::vector<std::future<int64_t>> data_acquisition_futures;

    std::unique_ptr<BackgroundEstimation> bkg_estimator;
    std::vector<uint8_t> spot_finder_mask;

    std::future<void> measurement;

    ThreadSafeFIFO<uint64_t> images_to_go;

    ZMQPreviewPublisher *preview_publisher = nullptr;
    ZMQSpotPublisher *spot_publisher = nullptr;
    ZMQImagePusher &image_pusher;

    volatile int abort{0};

    std::vector<AcquisitionDevice *> &open_capi_device;
    uint16_t ndatastreams{0};

    std::vector<size_t> max_delay;
    std::atomic<size_t> compressed_size{0};
    std::atomic<size_t> images_sent{0};

    const int64_t frame_transformation_nthreads;
    std::unique_ptr<Latch> data_acquisition_prepared;
    std::unique_ptr<Latch> frame_transformation_ready;
    std::unique_ptr<Latch> data_acquisition_ready;

    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;

    JFJochProtoBuf::DataProcessingSettings data_processing_settings;
    std::mutex data_processing_settings_mutex;

    std::vector<float> bkg_estimate;
    std::mutex bkg_estimate_mutex;

    int64_t preview_stride;
    int64_t spotfinder_stride;
    int64_t bkg_estimate_stride;

    int64_t max_image_number_sent = 0;
    std::mutex max_image_number_sent_mutex;

    int64_t AcquireThread(uint16_t data_stream);
    int64_t FrameTransformationThread();
    int64_t MeasurePedestalThread(uint16_t data_stream, uint16_t module);

    void Abort(const JFJochException &e);
    void FinalizeMeasurement();
    void SendImage(void *buffer, size_t image_number, size_t image_size);
    JFJochProtoBuf::DataProcessingSettings GetDataProcessingSettings();
    void AddBkgEstimate(const std::pair<float, float> &val, uint64_t image_number);
public:
    JFJochReceiver(const JFJochProtoBuf::JFJochReceiverInput &settings,
                   std::vector<AcquisitionDevice *> &open_capi_device,
                   ZMQImagePusher &image_pusher,
                   Logger &logger, int64_t forward_and_sum_nthreads,
                   ZMQPreviewPublisher* preview_publisher, ZMQSpotPublisher* spot_publisher);
    ~JFJochReceiver();
    JFJochReceiver(const JFJochReceiver &other) = delete;
    JFJochReceiver& operator=(const JFJochReceiver &other) = delete;
    void StopReceiver();
    void GetStatistics(JFJochProtoBuf::JFJochReceiverOutput &out) const;

    void Abort();
    void Cancel();
    int GetStatus() const;
    double GetProgress() const;
    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &data_processing_settings);
    void GetBkgEstimateStatus(JFJochProtoBuf::ReceiverStatus &status);
};

#endif //JUNGFRAUJOCH_JFJOCHRECEIVER_H

