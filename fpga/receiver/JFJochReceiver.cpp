// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiver.h"

#include <thread>

#include "../../common/PedestalCalc.h"
#include "../../common/SpotFinder.h"

int64_t JFJochReceiver::AcquireThread(uint16_t data_stream) {
    try {
        if (calib)
            acquisition_device[data_stream]->InitializeCalibration(experiment, calib.value());
        acquisition_device[data_stream]->PrepareAction(experiment);

        logger.Debug("Device thread {} prepare FPGA action", data_stream);

        data_acquisition_prepared->CountDown();
        frame_transformation_ready->Wait();

        logger.Debug("Device thread {} start FPGA action", data_stream);
        acquisition_device[data_stream]->StartAction(experiment);
        data_acquisition_ready->CountDown();
        logger.Debug("Device thread {} wait for FPGA action complete", data_stream);
        acquisition_device[data_stream]->WaitForActionComplete();

    } catch (const JFJochException &e) { Abort(e); }
    logger.Debug("Device thread {} done", data_stream);
    return -1;
}

int64_t JFJochReceiver::MeasurePedestalThread(uint16_t data_stream, uint16_t module, uint16_t storage_cell) {
    auto pedestal_calc = MakePedestalCalc(experiment, RAW_MODULE_LINES, data_stream);

    bool storage_cell_G0 = (experiment.GetStorageCellNumber() > 1)
                           && (experiment.GetDetectorMode() == DetectorMode::PedestalG0);

    bool storage_cell_G1G2 = (experiment.GetStorageCellNumber() > 1)
                             && ((experiment.GetDetectorMode() == DetectorMode::PedestalG1)
                                 || (experiment.GetDetectorMode() == DetectorMode::PedestalG2));

    int64_t delay = 0;

    size_t staring_frame;
    size_t frame_stride;

    size_t offset = experiment.GetFirstModuleOfDataStream(data_stream) + module;
    if (experiment.GetDetectorMode() == DetectorMode::PedestalG0) {
        staring_frame = storage_cell;
        frame_stride = experiment.GetStorageCellNumber();
        offset += experiment.GetModulesNum() * storage_cell;
    } else {
        staring_frame = 0;
        frame_stride = 1;
    }

    logger.Debug("Pedestal calculation thread for data stream {} module {} starting", data_stream, module);
    try {
        for (size_t frame = staring_frame; frame < experiment.GetFrameNum(); frame += frame_stride) {
            // Frame will be processed only if one already collects frame+2
            acquisition_device[data_stream]->WaitForFrame(frame + 2, module);
            if (!storage_cell_G1G2 || (frame % 2 == 1)) {
                // Partial packets will bring more problems, than benefit
                if (acquisition_device[data_stream]->IsFullModuleCollected(frame, module)) {
                    pedestal_calc->AnalyzeImage(
                            (uint16_t *) acquisition_device[data_stream]->GetFrameBuffer(frame, module));
                }
            }
            acquisition_device[data_stream]->FrameBufferRelease(frame, module);

            delay = std::max(delay, acquisition_device[data_stream]->CalculateDelay(frame, module));
        }

        if (experiment.GetDetectorMode() == DetectorMode::PedestalG0)
            pedestal_calc->Export(pedestal_result[offset], ALLOWED_WRONG_PEDESTAL_FOR_MASK, RMS_THRESHOLD_FOR_MASK);
        else
            pedestal_calc->Export(pedestal_result[offset]);
        pedestal_result[offset].frames = experiment.GetFrameNum();
        pedestal_result[offset].collection_time = start_time.time_since_epoch().count() / 1e9;
    } catch (const JFJochException &e) { Abort(e); }
    logger.Debug("Pedestal calculation thread for data stream {} module {} done", data_stream, module);
    return delay;
}

void JFJochReceiver::SendImage(void *buffer, size_t image_number, size_t image_size, const std::vector<DiffractionSpot>& spots) {
    if (push_images_to_writer)
        image_pusher.SendData(buffer, image_size, spots, image_number);
    {
        std::unique_lock<std::mutex> ul(max_image_number_sent_mutex);
        if (image_number > max_image_number_sent)
            max_image_number_sent = image_number;
    }
    compressed_size += image_size;
    images_sent++;
}

int64_t JFJochReceiver::FrameTransformationThread() {

    FrameTransformation transformation(experiment);

#ifdef CUDA_SPOT_FINDING
    std::unique_ptr<SpotFinder> spot_finder;

    try {
        spot_finder = std::make_unique<SpotFinder>(experiment.GetXPixelsNum(), experiment.GetYPixelsNum());
        spot_finder->SetInputBuffer(transformation.GetPreview16BitImage());
        spot_finder->RegisterBuffer();
        spot_finder->LoadMask(one_byte_mask);
    } catch (const JFJochException& e) {
        frame_transformation_ready->CountDown();
        logger.Error("Error creating GPU spot finder");
        Abort(e);
        return -1;
    }
#endif

    std::unique_ptr<RadialIntegration> rad_int;
    if (rad_int_mapping)
        rad_int = std::make_unique<RadialIntegration>(*rad_int_mapping);

    std::vector<char> writer_buffer(experiment.GetMaxCompressedSize());

    int64_t max_thread_delay = 0;
    uint64_t image_number;

    transformation.SetOutput(writer_buffer.data());

    frame_transformation_ready->CountDown();

    std::vector<float> rad_int_result;

    while (images_to_go.Get(image_number) != 0) {
        try {
            bool send_preview = false;
            bool send_bkg_estimate = false;

            if ((preview_publisher != nullptr) && (preview_stride > 0) && (image_number % preview_stride == 0))
                send_preview = true;

            if ((rad_int != nullptr) && (bkg_estimate_stride > 0) && (image_number % bkg_estimate_stride == 0))
                send_bkg_estimate = true;

            bool send_image = false; // We send image if at least one module was collected in full

            bool send_spots = (spot_publisher != nullptr) && (spotfinder_stride > 0) && (image_number % spotfinder_stride == 0);

            for (int j = 0; j < experiment.GetSummation(); j++) {
                size_t frame_number = image_number * experiment.GetSummation() + j;

                for (int d = 0; d < acquisition_device.size(); d++) {
                    acquisition_device[d]->WaitForFrame(frame_number + 2);

                    for (int m = 0; m < experiment.GetModulesNum(d); m++) {
                        const int16_t *src;

                        if (acquisition_device[d]->IsFullModuleCollected(frame_number, m)) {
                            src = acquisition_device[d]->GetFrameBuffer(frame_number, m);
                            send_image = true;
                        } else
                            src = acquisition_device[d]->GetErrorFrameBuffer();

                        transformation.ProcessModule(src, frame_number, m, d);
                        acquisition_device[d]->FrameBufferRelease(frame_number, m);
                    }
                    max_thread_delay = std::max(max_thread_delay, acquisition_device[d]->CalculateDelay(frame_number));
                }
            }

            if (send_image) {
                size_t image_size = transformation.PackStandardOutput();

                std::vector<DiffractionSpot> spots;
                auto local_data_processing_settings = GetDataProcessingSettings();
#ifdef CUDA_SPOT_FINDING
                // Spot finding is async, so it can be sandwiched between sending image and other tasks
                if (send_spots)
                    spot_finder->RunSpotFinder(local_data_processing_settings);
#endif

                if (send_preview)
                    preview_publisher->Publish(experiment,
                                               transformation.GetPreview16BitImage(),
                                               image_number);

                if (send_bkg_estimate) {
                    rad_int->ProcessOneImage(transformation.GetPreview16BitImage(),experiment.GetPixelsNum());

                    uint16_t rad_int_min_bin = std::floor(
                            rad_int_mapping->QToBin(local_data_processing_settings.bkg_estimate_low_q()));
                    uint16_t rad_int_max_bin = std::ceil(
                            rad_int_mapping->QToBin(local_data_processing_settings.bkg_estimate_high_q()));
                    float bkg_estimate_val = rad_int->GetRangeValue(rad_int_min_bin, rad_int_max_bin);
                    bkg_estimate.AddElement(image_number, bkg_estimate_val);

                    rad_int->GetResult(rad_int_result);
                    AddRadialIntegrationProfile(rad_int_result);
                }

#ifdef CUDA_SPOT_FINDING
                if (send_spots) {
                    spot_finder->GetResults(experiment, GetDataProcessingSettings(), spots, 0);
                    spot_count.AddElement(image_number, spots.size());
                    spot_publisher->Publish(experiment, spots, image_number);
                }
#endif
                SendImage(writer_buffer.data(), image_number, image_size, spots);
            }
        } catch (const JFJochException &e) { Abort(e); }
    }

#ifdef CUDA_SPOT_FINDING
    spot_finder->UnregisterBuffer();
#endif

    logger.Debug("Sum&compression thread done");

    return max_thread_delay;
}

JFJochReceiver::JFJochReceiver(const JFJochProtoBuf::ReceiverInput &settings,
                               std::vector<AcquisitionDevice *> &in_aq_device,
                               ImagePusher &in_image_sender,
                               Logger &in_logger, int64_t in_forward_and_sum_nthreads,
                               ZMQPreviewPublisher* in_preview_publisher,
                               ZMQSpotPublisher* in_spot_publisher) :
        experiment(settings.jungfraujoch_settings()),
        acquisition_device(in_aq_device),
        logger(in_logger),
        image_pusher(in_image_sender),
        frame_transformation_nthreads(in_forward_and_sum_nthreads),
        preview_publisher(in_preview_publisher),
        spot_publisher(in_spot_publisher)
{
    ndatastreams = experiment.GetDataStreamsNum();

    if (settings.has_calibration()) {
        calib.emplace(settings.calibration());
        one_byte_mask = calib->CalculateOneByteMask(experiment);
    } else {
        one_byte_mask.resize(experiment.GetPixelsNum());
        for (auto &i: one_byte_mask) i = 1;
    }

    if (!experiment.CheckGitSha1Consistent())
        logger.Warning(experiment.CheckGitSha1Msg());

    push_images_to_writer = (experiment.GetImageNum() > 0) && (!experiment.GetFilePrefix().empty());

    if (acquisition_device.size() > ndatastreams)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Number of acquisition devices has to match data streams");
    if (frame_transformation_nthreads <= 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Number of threads must be more than zero");

    std::vector<std::string> zmq_writer_addrs;
    if (!settings.writer_zmq_address().empty())
        zmq_writer_addrs = {settings.writer_zmq_address().begin(),settings.writer_zmq_address().end()};


    preview_stride = experiment.GetPreviewStride();
    spotfinder_stride = experiment.GetSpotFindingStride();
    bkg_estimate_stride = experiment.GetBackgroundEstimationStride();

    logger.Info("Preview stride: {}", preview_stride);
    logger.Info("Spot finder stride: {}", spotfinder_stride);
    logger.Info("Bkg estimate stride: {}",bkg_estimate_stride);

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        if (preview_publisher != nullptr)
            preview_publisher->Start(experiment, calib.value());

        rad_int_mapping = std::make_unique<RadialIntegrationMapping>(experiment, one_byte_mask.data());
        spot_finder_mask = calib->CalculateOneByteMask(experiment);
    }

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

        if (experiment.GetDetectorMode() == DetectorMode::PedestalG0) {
            pedestal_result.resize(experiment.GetModulesNum() * experiment.GetStorageCellNumber());
            for (int s = 0; s < experiment.GetStorageCellNumber(); s++) {
                for (int d = 0; d < ndatastreams; d++) {
                    for (int m = 0; m < experiment.GetModulesNum(d); m++) {
                        auto handle = std::async(std::launch::async, &JFJochReceiver::MeasurePedestalThread, this, d, m,
                                                 s);
                        frame_transformation_futures.emplace_back(std::move(handle));
                    }
                }
            }
        } else {
            pedestal_result.resize(experiment.GetModulesNum());
            for (int d = 0; d < ndatastreams; d++) {
                for (int m = 0; m < experiment.GetModulesNum(d); m++) {
                    auto handle = std::async(std::launch::async, &JFJochReceiver::MeasurePedestalThread, this, d, m, 0);
                    frame_transformation_futures.emplace_back(std::move(handle));
                }
            }
        }

        logger.Info("Pedestal threads ready");
    }

    if (experiment.GetImageNum() > 0) {
        logger.Info("Data file count {}", experiment.GetDataFileCount());
        if (push_images_to_writer)
            image_pusher.StartDataCollection(experiment.GetDataFileCount());

        for (int i = 0; i < experiment.GetImageNum(); i++)
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

    logger.Info("Acquisition devices ready");

    start_time = std::chrono::system_clock::now();
    logger.Info("Receiving data started");

    measurement = std::async(std::launch::async, &JFJochReceiver::FinalizeMeasurement, this);
}

void JFJochReceiver::GetStatistics(JFJochProtoBuf::ReceiverOutput &ret) const {
    uint64_t expected_packets = 0;
    uint64_t received_packets = 0;

    for (int d = 0; d < ndatastreams; d++) {
        acquisition_device[d]->SaveStatistics(experiment, *ret.add_device_statistics());
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
    if (!pedestal_result.empty())
        *ret.mutable_pedestal_result() = {pedestal_result.begin(), pedestal_result.end()};

    ret.set_master_file_name(experiment.GetFilePrefix());
    ret.set_cancelled(cancelled);
}

void JFJochReceiver::Cancel() {
    // Remote abort: This tells FPGAs to stop, but doesn't do anything to CPU code
    logger.Warning("Cancelling on request");

    cancelled = true;

    for (int d = 0; d < ndatastreams; d++)
        acquisition_device[d]->ActionAbort();
}

void JFJochReceiver::Abort() {
    // Remote abort: This tells FPGAs to stop, but doesn't do anything to CPU code
    logger.Error("Aborting on request");

    cancelled = true;
    abort = 1;

    for (int d = 0; d < ndatastreams; d++)
        acquisition_device[d]->ActionAbort();
}

void JFJochReceiver::Abort(const JFJochException &e) {
    logger.Error("Aborting data collection due to exception");
    logger.ErrorException(e);
    // Error abort: This tells FPGAs to stop and also prevents deadlock in CPU code, by setting abort to 1
    cancelled = true;
    abort = 1;

    for (int d = 0; d < ndatastreams; d++)
        acquisition_device[d]->ActionAbort();
}

int JFJochReceiver::GetStatus() const {
    return abort;
}

double JFJochReceiver::GetProgress() const {
    if (experiment.GetImageNum() == 0)
        return 100.0;
    else
        return static_cast<double>(max_image_number_sent) / static_cast<double>(experiment.GetImageNum()) * 100.0;
}

void JFJochReceiver::FinalizeMeasurement() {
    for (auto &future : frame_transformation_futures) {
        auto val = future.get();
        if (val >= 0) max_delay.push_back(val);
    }
    logger.Info("All processing threads done");

    if (push_images_to_writer)
        image_pusher.EndDataCollection();

    logger.Info("Disconnected from writers");

    end_time = std::chrono::system_clock::now();

    if (preview_publisher != nullptr)
        preview_publisher->Stop(experiment);

    // All devices can be stopped here
    for (int d = 0; d < ndatastreams; d++)
        acquisition_device[d]->ActionAbort();

    for (auto &future : data_acquisition_futures) {
        auto val = future.get();
        if (val >= 0) max_delay.push_back(val);
    }
    logger.Info("Devices stopped");
    logger.Info("Receiving data done");
}

void JFJochReceiver::SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &in_data_processing_settings) {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    DiffractionExperiment::CheckDataProcessingSettings(in_data_processing_settings);
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

void JFJochReceiver::AddRadialIntegrationProfile(const std::vector<float> &result) {
    std::unique_lock<std::mutex> ul(rad_int_profile_mutex);
    if (rad_int_profile.empty())
        rad_int_profile = result;
    else if (rad_int_profile.size() == result.size()) {
        for (int i = 0; i < rad_int_profile.size(); i++)
            rad_int_profile[i] += (result[i] - rad_int_profile[i]) / rad_int_profile_window;
    } else {
        // Throw exception?
        Abort();
    }
}

void JFJochReceiver::GetRadialIntegrationProfile(JFJochProtoBuf::ReceiverDataProcessingPlots &plots) {
    std::unique_lock<std::mutex> ul(rad_int_profile_mutex);
    auto plot = plots.mutable_radial_int_profile();
    const auto &bin_to_q = rad_int_mapping->GetBinToQ();
    if (!rad_int_profile.empty()) {
        *plot->mutable_x() = {bin_to_q.begin(), bin_to_q.end()};
        *plot->mutable_y() = {rad_int_profile.begin(), rad_int_profile.end()};
    }
}


JFJochProtoBuf::ReceiverDataProcessingPlots JFJochReceiver::GetPlots() {
    JFJochProtoBuf::ReceiverDataProcessingPlots ret;
    spot_count.GetPlot(*ret.mutable_spot_count(), 100);
    bkg_estimate.GetPlot(*ret.mutable_bkg_estimate(), 100);
    GetRadialIntegrationProfile(ret);
    return ret;
}
