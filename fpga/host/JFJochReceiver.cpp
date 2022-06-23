// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiver.h"

#include <thread>
#include "../../common/RawToConvertedGeometry.h"
#include "../../common/PedestalCalc.h"
#include "../../common/SpotFinder.h"
#include "../../common/FrameTransformationPreview.h"

int64_t JFJochReceiver::AcquireThread(uint16_t data_stream) {
    try {
        open_capi_device[data_stream]->InitializeCalibration(experiment, calib);
        open_capi_device[data_stream]->PrepareAction(experiment);

        data_acquisition_prepared->CountDown();
        frame_transformation_ready->Wait();

        open_capi_device[data_stream]->StartAction(experiment);
        data_acquisition_ready->CountDown();
        open_capi_device[data_stream]->WaitForActionComplete();

    } catch (const JFJochException &e) { Abort(e); }
    logger.Debug("open-capi-done-" + std::to_string(data_stream));
    return -1;
}

int64_t JFJochReceiver::MeasurePedestalThread(uint16_t data_stream, uint16_t module) {
    auto pedestal_calc = MakePedestalCalc(experiment, RAW_MODULE_LINES, data_stream);

    int64_t delay = 0;
    try {
        for (size_t frame = 0; frame < experiment.GetFrameNum(); frame++) {
            // Frame will be processed only if one already collects frame+2
            open_capi_device[data_stream]->WaitForFrame(frame+2, module);

            // Partial packets will bring more problems, than benefit
            if (open_capi_device[data_stream]->GetPacketCount(frame, module) == 128) {
                pedestal_calc->AnalyzeImage((uint16_t *) open_capi_device[data_stream]->GetFrameBuffer(frame, module));
            }
            open_capi_device[data_stream]->FrameBufferRelease(frame, module);

            delay = std::max(delay, open_capi_device[data_stream]->CalculateDelay(frame, module));
        }
        size_t offset = (experiment.GetFirstModuleOfDataStream(data_stream) + module) * RAW_MODULE_SIZE;
        if (experiment.GetDetectorMode() == DetectorMode::PedestalG0)
            pedestal_calc->Export(calib, offset, ALLOWED_WRONG_PEDESTAL_FOR_MASK, RMS_THRESHOLD_FOR_MASK);
        else
            pedestal_calc->Export(calib, offset);

    } catch (const JFJochException &e) { Abort(e); }
    logger.Debug("pedestal-thread-done");
    return delay;
}

void JFJochReceiver::SendImage(void *buffer, size_t image_number, size_t image_size) {
    image_pusher.SendData(buffer, image_number, image_size);
    {
        std::unique_lock<std::mutex> ul(max_image_number_sent_mutex);
        if (image_number > max_image_number_sent)
            max_image_number_sent = image_number;
    }
    compressed_size += image_size;
    images_sent++;
}

int64_t JFJochReceiver::FrameTransformationThread() {
    // Assume  experiment.GetSummationForPreview() == N * experiment.GetSummation()
    // where N = 1, 2, 3, ... , experiment.GetSummationForPreview()

#ifdef CUDA_SPOT_FINDING
    std::unique_ptr<SpotFinder> spot_finder;
    try {
        spot_finder = std::make_unique<SpotFinder>(experiment);
    } catch (...) {
        frame_transformation_ready->CountDown();
        logger.Error("Error creating GPU spot finder");
        Abort();
        throw;
    }
    int16_t *preview_buffer = spot_finder->GetInputBuffer();
#else
    std::vector<int16_t> preview_buffer_v(experiment.GetPixelsNum());
    int16_t *preview_buffer = preview_buffer_v.data();
#endif
    std::vector<char> writer_buffer(sizeof(ImageMetadata) + experiment.GetMaxCompressedSize());
    auto image_metadata = (ImageMetadata *) writer_buffer.data();
    image_metadata->version = IMAGE_METADATA_VERSION;

    FrameTransformation transformation_writing(experiment, calib);
    FrameTransformationPreview transformation_preview(experiment, spot_finder_mask, preview_buffer);

    int64_t max_thread_delay = 0;
    uint64_t image_number;
    uint64_t preview_to_write_ratio = experiment.GetSummationForPreview() / experiment.GetSummation();

    transformation_writing.SetOutput(writer_buffer.data() + sizeof(ImageMetadata));

    frame_transformation_ready->CountDown();

    while (images_to_go.Get(image_number) != 0) {
        try {
            bool send_preview = false;
            bool send_spots = false;
            bool send_bkg_estimate = false;

            if ((preview_publisher != nullptr) && (preview_stride > 0) && (image_number % preview_stride == 0))
                send_preview = true;

#ifdef CUDA_SPOT_FINDING
            if ((spot_publisher != nullptr) && (spotfinder_stride > 0) && (image_number % spotfinder_stride == 0))
                send_spots = true;
#endif
            if ((bkg_estimate_stride > 0) && (image_number % bkg_estimate_stride == 0))
                send_bkg_estimate = true;

            bool generate_preview = send_preview || send_spots || send_bkg_estimate;

            size_t preview_packets = 0;
            for (int i = 0; i < preview_to_write_ratio; i++) {
                size_t image_packets = 0;
                for (int j = 0; j < experiment.GetSummation(); j++) {
                    size_t frame_number = image_number * experiment.GetSummationForPreview() + i * experiment.GetSummation() + j;
                    size_t frame_packets = 0;

                    for (int d = 0; d < open_capi_device.size(); d++) {
                        open_capi_device[d]->WaitForFrame(frame_number+2);

                        for (int m = 0; m < experiment.GetModulesNum(d); m++) {
                            size_t packets = open_capi_device[d]->GetPacketCount(frame_number, m);
                            frame_packets += packets;
                            if (packets == 128) {
                                auto src = open_capi_device[d]->GetFrameBuffer(frame_number, m);
                                transformation_writing.ProcessModule(src, frame_number, m, d);
                                if (generate_preview)
                                    transformation_preview.ProcessModule(src, frame_number, m, d);
                            } else if (packets > 0) {
                                // handle eth packets
                                for (int packet = 0; packet < 128; packet++) {
                                    auto src = open_capi_device[d]->GetPacketBuffer(frame_number, m, packet);
                                    transformation_writing.ProcessPacket(src, frame_number, m, packet, d);
                                    if (generate_preview)
                                        transformation_preview.ProcessPacket(src, frame_number, m, packet, d);
                                }
                            }
                            open_capi_device[d]->FrameBufferRelease(frame_number, m);
                        }
                        max_thread_delay = std::max(max_thread_delay, open_capi_device[d]->CalculateDelay(frame_number));
                    }
                    image_packets += frame_packets;
                    preview_packets += frame_packets;
                }

                if (image_packets > 0) {
                    size_t image_size = transformation_writing.PackStandardOutput();
                    SendImage(writer_buffer.data(), image_number, image_size);
                }
            }

            if ((generate_preview) && (preview_packets > 0)) {
                if (send_preview)
                    preview_publisher->Publish(experiment, preview_buffer, image_number);

#ifdef CUDA_SPOT_FINDING
                if (send_spots) {
                    std::vector<DiffractionSpot> spots;
                    spot_finder->FindSpots(experiment, GetDataProcessingSettings(), spots, 0);
                    spot_publisher->Publish(experiment, spots, image_number);
                }
#endif

                if (send_bkg_estimate)
                    AddBkgEstimate(bkg_estimator->Process(preview_buffer), image_number);
            }
        } catch (const JFJochException &e) { Abort(e); }
    }

    logger.Debug("sum-and-forward-done");

    return max_thread_delay;
}

JFJochReceiver::JFJochReceiver(const JFJochProtoBuf::JFJochReceiverInput &settings,
                               std::vector<AcquisitionDevice *> &in_open_capi_device,
                               ZMQImagePusher &in_image_sender,
                               Logger &in_logger, int64_t in_forward_and_sum_nthreads,
                               ZMQPreviewPublisher* in_preview_publisher,
                               ZMQSpotPublisher* in_spot_publisher) :
        experiment(settings.jungfraujoch_settings()),
        calib(settings.calibration()),
        open_capi_device(in_open_capi_device),
        logger(in_logger),
        image_pusher(in_image_sender),
        frame_transformation_nthreads(in_forward_and_sum_nthreads),
        preview_publisher(in_preview_publisher),
        spot_publisher(in_spot_publisher)
{
    ndatastreams = experiment.GetDataStreamsNum();

    if (!experiment.CheckGitSha1Consistent())
        logger.Warning(experiment.CheckGitSha1Msg());

    if (open_capi_device.size() > ndatastreams)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Number of acquisition devices has to match data streams");
    if (frame_transformation_nthreads <= 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Number of threads must be more than zero");

    std::vector<std::string> zmq_writer_addrs;
    if (!settings.writer_zmq_address().empty())
        zmq_writer_addrs = {settings.writer_zmq_address().begin(),settings.writer_zmq_address().end()};

    if (preview_publisher != nullptr)
        preview_publisher->Start(experiment, calib);

    preview_stride = experiment.GetPreviewStride();
    spotfinder_stride = experiment.GetSpotFindingStride();
    bkg_estimate_stride = experiment.GetBackgroundEstimationStride();

    logger.Info("Preview stride: " + std::to_string(preview_stride));
    logger.Info("Spot finder stride: " + std::to_string(spotfinder_stride));
    logger.Info("Bkg estimate stride: " + std::to_string(bkg_estimate_stride));

    bkg_estimator = std::make_unique<BackgroundEstimation>(experiment);
    if (bkg_estimate_stride > 0)
        bkg_estimate.reserve(experiment.GetImageNum() / bkg_estimate_stride);

    spot_finder_mask = calib.GetMaskOneByte(experiment);

    // Create latches
    data_acquisition_ready = std::make_unique<Latch>(ndatastreams);
    data_acquisition_prepared = std::make_unique<Latch>(ndatastreams);

    if (experiment.GetImageNum() > 0)
        frame_transformation_ready = std::make_unique<Latch>(frame_transformation_nthreads);
    else
        frame_transformation_ready = std::make_unique<Latch>(0);

    for (int d = 0; d < ndatastreams; d++) {
        auto handle = std::async(std::launch::async, &JFJochReceiver::AcquireThread, this, d);
        data_acquisition_futures.emplace_back(std::move(handle));
    }

    data_acquisition_prepared->Wait();
    logger.Info("Data acquisition devices ready");

    if ((experiment.GetDetectorMode() == DetectorMode::PedestalG0)
        || (experiment.GetDetectorMode() == DetectorMode::PedestalG1)
        || (experiment.GetDetectorMode() == DetectorMode::PedestalG2)) {

        if (experiment.GetImageNum() > 0) {
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                                  "Saving and calculating pedestal is not supported for the time being");
        }
        for (int d = 0; d < ndatastreams; d++)
            for (int m = 0; m < experiment.GetModulesNum(d); m++) {
                auto handle = std::async(std::launch::async, &JFJochReceiver::MeasurePedestalThread, this, d, m);
                frame_transformation_futures.emplace_back(std::move(handle));
            }
        logger.Info("Pedestal threads ready");
    }

    if (experiment.GetImageNum() > 0) {
        image_pusher.Connect(zmq_writer_addrs, settings.jungfraujoch_settings());

        for (int i = 0; i < experiment.GetImageNumForPreview(); i++)
            images_to_go.Put(i);

        // Setup frames summation and forwarding
        for (int i = 0; i < frame_transformation_nthreads; i++) {
            auto handle = std::async(std::launch::async, &JFJochReceiver::FrameTransformationThread, this);
            frame_transformation_futures.emplace_back(std::move(handle));
        }

        logger.Info("Image compression/forwarding threads started");

        frame_transformation_ready->Wait();
        logger.Info("Image compression/forwarding threads ready");
    }

    data_acquisition_ready->Wait();

    logger.Info("OpenCAPI devices ready");

    start_time = std::chrono::system_clock::now();
    logger.Info("Receiving data started");

    measurement = std::async(std::launch::async, &JFJochReceiver::FinalizeMeasurement, this);
}

void JFJochReceiver::GetStatistics(JFJochProtoBuf::JFJochReceiverOutput &ret) const {
    uint64_t expected_packets = 0;
    uint64_t received_packets = 0;

    for (int d = 0; d < ndatastreams; d++) {
        open_capi_device[d]->SaveStatistics(experiment, *ret.add_device_statistics());
        expected_packets += ret.device_statistics(d).packets_expected();
        received_packets += ret.device_statistics(d).good_packets();
    }

    if ((expected_packets == received_packets) || (expected_packets == 0))
        ret.set_efficiency(1.0);
    else
        ret.set_efficiency(received_packets / static_cast<double>(expected_packets));

    ret.set_compressed_size(compressed_size);
    ret.set_max_image_number_sent(max_image_number_sent);

    if (experiment.GetImageNum() > 0) {
        ret.set_compressed_ratio( (double) (images_sent * experiment.GetPixelDepth() *
                                            experiment.GetPixelsNum()) / double(compressed_size));
    }

    if (!max_delay.empty())
        ret.set_max_receive_delay(*std::max_element(max_delay.begin(), max_delay.end()));
    else
        ret.set_max_receive_delay(0);

    ret.set_images_sent(images_sent);
    ret.set_start_time_ms(std::chrono::duration_cast<std::chrono::milliseconds>(start_time.time_since_epoch()).count());
    ret.set_end_time_ms(std::chrono::duration_cast<std::chrono::milliseconds>(end_time.time_since_epoch()).count());
    *ret.mutable_calibration() = calib;
    *ret.mutable_jungfraujoch_settings() = experiment;
}

void JFJochReceiver::Cancel() {
    // Remote abort: This tells FPGAs to stop, but doesn't do anything to CPU code
    logger.Error("Cancelling on request");

    for (int d = 0; d < ndatastreams; d++)
        open_capi_device[d]->ActionAbort();
}

void JFJochReceiver::Abort() {
    // Remote abort: This tells FPGAs to stop, but doesn't do anything to CPU code
    logger.Error("Aborting on request");

    for (int d = 0; d < ndatastreams; d++)
        open_capi_device[d]->ActionAbort();

    abort = 1;
}

void JFJochReceiver::Abort(const JFJochException &e) {
    logger.Error("Aborting data collection due to exception");
    logger.ErrorException(e);
    // Error abort: This tells FPGAs to stop and also prevents deadlock in CPU code, by setting abort to 1
    abort = 1;

    for (int d = 0; d < ndatastreams; d++)
        open_capi_device[d]->ActionAbort();
}

int JFJochReceiver::GetStatus() const {
    return abort;
}

double JFJochReceiver::GetProgress() const {
    if (experiment.GetImageNum() == 0)
        return 100.0;
    else
        return static_cast<double>(images_sent) / static_cast<double>(experiment.GetImageNum()) * 100.0;
}

void JFJochReceiver::FinalizeMeasurement() {
    for (auto &future : frame_transformation_futures) {
        auto val = future.get();
        if (val >= 0) max_delay.push_back(val);
    }
    logger.Info("All processing threads done");

    image_pusher.EndDataCollection();
    image_pusher.Disconnect();
    logger.Info("Disconnected from writers");

    end_time = std::chrono::system_clock::now();

    if (preview_publisher != nullptr)
        preview_publisher->Stop(experiment);

    // All devices can be stopped here
    for (int d = 0; d < ndatastreams; d++)
        open_capi_device[d]->ActionAbort();

    for (auto &future : data_acquisition_futures) {
        auto val = future.get();
        if (val >= 0) max_delay.push_back(val);
    }
    logger.Info("Devices stopped");

    if (experiment.IsPedestalChanging())
        calib.SetPedestalProperties(experiment, start_time.time_since_epoch().count() / 1e9);
    logger.Info("Receiving data done");
}

void JFJochReceiver::SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &in_data_processing_settings) {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    data_processing_settings = in_data_processing_settings;
}

void JFJochReceiver::StopReceiver() {
    if (measurement.valid()) {
        measurement.get();
        logger.Info("Receiver stopped");
    }
}

JFJochReceiver::~JFJochReceiver() {
    abort = 1;
    if (measurement.valid())
        measurement.get();
}

JFJochProtoBuf::DataProcessingSettings JFJochReceiver::GetDataProcessingSettings() {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    return data_processing_settings;
}

void JFJochReceiver::AddBkgEstimate(const std::pair<float, float> &val, uint64_t image_number) {
    std::unique_lock<std::mutex> ul(bkg_estimate_mutex);
    auto location = image_number / bkg_estimate_stride;
    if (location >= bkg_estimate.size())
        bkg_estimate.resize(location+1);
    bkg_estimate[location] = val.first;
}

void JFJochReceiver::GetBkgEstimateStatus(JFJochProtoBuf::ReceiverStatus &status) {
    std::unique_lock<std::mutex> ul(bkg_estimate_mutex);
    if (!bkg_estimate.empty())
        *status.mutable_bkg_estimate() = {bkg_estimate.begin(), bkg_estimate.end()};
}