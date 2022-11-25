// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHRECEIVER_H
#define JUNGFRAUJOCH_JFJOCHRECEIVER_H

#include <optional>
#include <condition_variable>
#include <atomic>
#include <future>

#include "../../common/DiffractionExperiment.h"
#include "../../common/JFJochException.h"
#include "../../common/FrameTransformation.h"
#include "../../common/StrongPixelSet.h"
#include "../../common/JFCalibration.h"
#include "../../common/ImagePusher.h"

#include "../host/AcquisitionDevice.h"
#include "../../common/Logger.h"
#include "../../common/ThreadSafeFIFO.h"
#include "../../common/ZMQPreviewPublisher.h"
#include "../../common/ZMQSpotPublisher.h"

#include "../../common/RadialIntegration.h"
#include "../../common/Latch.h"

#include "../../common/StatusVector.h"

#define RMS_THRESHOLD_FOR_MASK (100)
#define ALLOWED_WRONG_PEDESTAL_FOR_MASK (2)

class JFJochReceiver {
    DiffractionExperiment experiment;
    std::vector<JFPedestal> pedestal_result;
    std::optional<JFCalibration> calib;

    std::vector<uint8_t> one_byte_mask;
    Logger &logger;

    std::vector<std::future<int64_t>> frame_transformation_futures;
    std::vector<std::future<int64_t>> data_acquisition_futures;

    std::unique_ptr<RadialIntegrationMapping> rad_int_mapping;
    uint16_t rad_int_min_bin;
    uint16_t rad_int_max_bin;

    std::vector<float> rad_int_profile;
    constexpr static const size_t rad_int_profile_window = 100;
    std::mutex rad_int_profile_mutex;

    std::vector<uint8_t> spot_finder_mask;

    std::future<void> measurement;

    ThreadSafeFIFO<uint64_t> images_to_go;

    ZMQPreviewPublisher *preview_publisher = nullptr;
    ZMQSpotPublisher *spot_publisher = nullptr;

    ImagePusher &image_pusher;
    bool push_images_to_writer;

    volatile int abort{0};

    std::vector<AcquisitionDevice *> &acquisition_device;
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

    StatusVector<float> bkg_estimate;
    StatusVector<uint64_t> spot_count;

    int64_t preview_stride;
    int64_t spotfinder_stride;
    int64_t bkg_estimate_stride;

    int64_t max_image_number_sent = 0;
    std::mutex max_image_number_sent_mutex;

    int64_t AcquireThread(uint16_t data_stream);
    int64_t FrameTransformationThread();
    int64_t MeasurePedestalThread(uint16_t data_stream, uint16_t module, uint16_t storage_cell);

    void Abort(const JFJochException &e);
    void FinalizeMeasurement();
    void SendImage(void *buffer, size_t image_number, size_t image_size, const std::vector<DiffractionSpot>& spots);
    JFJochProtoBuf::DataProcessingSettings GetDataProcessingSettings();

    void AddRadialIntegrationProfile(const std::vector<float> &result);

    void GetPlots(JFJochProtoBuf::ReceiverStatus &status);
    void GetRadialIntegrationProfile(JFJochProtoBuf::ReceiverStatus &status);
public:
    JFJochReceiver(const JFJochProtoBuf::ReceiverInput &settings,
                   std::vector<AcquisitionDevice *> &open_capi_device,
                   ImagePusher &image_pusher,
                   Logger &logger, int64_t forward_and_sum_nthreads,
                   ZMQPreviewPublisher* preview_publisher, ZMQSpotPublisher* spot_publisher);
    ~JFJochReceiver();
    JFJochReceiver(const JFJochReceiver &other) = delete;
    JFJochReceiver& operator=(const JFJochReceiver &other) = delete;
    void StopReceiver();
    void GetStatistics(JFJochProtoBuf::ReceiverOutput &out) const;

    void Abort();
    void Cancel();
    int GetStatus() const;
    double GetProgress() const;
    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &data_processing_settings);

    void GetStatus(JFJochProtoBuf::ReceiverStatus &status);

};

#endif //JUNGFRAUJOCH_JFJOCHRECEIVER_H

