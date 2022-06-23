// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochBrokerService.h"
#include "../common/JFJochException.h"

JFJochBrokerService::JFJochBrokerService(Logger &in_logger)
: logger(in_logger), services(in_logger) {
    data_processing_settings.set_local_bkg_size(5);
    data_processing_settings.set_signal_to_noise_threshold(3);
    data_processing_settings.set_photon_count_threshold(16);
    data_processing_settings.set_min_pix_per_spot(1);
    data_processing_settings.set_max_pix_per_spot(50);

    experiment.BeamlineDelay(std::chrono::seconds(30));
}

DiffractionExperiment &JFJochBrokerService::Experiment() {
    return experiment;
}

JFJochServices &JFJochBrokerService::Services() {
    return services;
}

grpc::Status JFJochBrokerService::Start(grpc::ServerContext *context, const JFJochProtoBuf::BrokerSetup *request,
                                        JFJochProtoBuf::Empty *response) {
    experiment.IncrementMeasurementSequenceNumber();
    DiffractionExperiment started_experiment(experiment);
    try {
        started_experiment.ImagesPerTrigger(request->images_per_trigger());
        started_experiment.ImageTime(std::chrono::microseconds(request->image_time_us()));
        started_experiment.BeamX_pxl(request->beam_center_x_pxl());
        started_experiment.BeamY_pxl(request->beam_center_y_pxl());
        started_experiment.DetectorDistance_mm(request->detector_distance_mm());
        started_experiment.PhotonEnergy_keV(request->photon_energy_kev());
        started_experiment.FilePrefix(request->name_pattern());
        started_experiment.SampleName(request->sample_name());

        if (request->has_ntrigger())
            started_experiment.NumTriggers(request->ntrigger());

        if (request->has_images_per_file())
            started_experiment.ImagesPerFile(request->images_per_file());

        if (request->has_compression())
            started_experiment.Compression_Text(request->compression());

        if (request->has_omega_start_deg())
            started_experiment.OmegaStart(request->omega_start_deg());

        if (request->has_omega_increment_deg())
            started_experiment.OmegaIncrement(request->omega_increment_deg());

        if (request->has_time_resolved_mode())
            started_experiment.TimeResolvedMode(request->time_resolved_mode());

        if (request->has_force_full_speed())
            started_experiment.ForceFullSpeed(request->force_full_speed());

        if (request->has_data_collection_mode())
            started_experiment.Mode_Text(request->data_collection_mode());

        if (request->has_pedestal_saved())
            started_experiment.PedestalSaved(request->pedestal_saved());

        if (request->has_preview_rate_ms())
            started_experiment.PreviewPeriod(std::chrono::milliseconds(request->preview_rate_ms()));

        if (request->has_spot_finding_rate_ms())
            started_experiment.SpotFindingPeriod(std::chrono::milliseconds(request->spot_finding_rate_ms()));

        if (request->has_bkg_estimate_rate_ms())
            started_experiment.BackgroundEstimationPeriod(std::chrono::milliseconds(request->bkg_estimate_rate_ms()));

        if (request->has_frame_summation())
            started_experiment.FrameSummationEnable(request->frame_summation());

        if (request->has_skip_pedestal())
            started_experiment.SkipPedestal(request->skip_pedestal());

        if (request->has_unit_cell())
            started_experiment.SetUnitCell(request->unit_cell());

        if (request->has_space_group_number())
            started_experiment.SpaceGroupNumber(request->space_group_number());

        if (request->has_run_number())
            started_experiment.RunNumber(request->run_number());
    } catch (JFJochException &e) {
        return {grpc::StatusCode::INVALID_ARGUMENT, e.what()};
    }

    try {
        services.SetDataProcessingSettings(GetDataProcessingSettings());
        state_machine.MeasureStart(started_experiment);
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}


grpc::Status JFJochBrokerService::Cancel(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                         JFJochProtoBuf::Empty *response) {
    try {
        state_machine.Cancel();
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::Abort(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                        JFJochProtoBuf::Empty *response) {
    try {
        state_machine.Abort();
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::Deactivate(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                             JFJochProtoBuf::Empty *response) {
    try {
        state_machine.Deactivate();
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                       JFJochProtoBuf::Empty *response) {
    try {
        state_machine.MeasureEnd();
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::Initialize(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                             JFJochProtoBuf::Empty *response) {
    try {
        state_machine.Initialize(experiment);
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::Pedestal(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                           JFJochProtoBuf::Empty *response) {
    try {
        state_machine.TakePedestal(experiment);
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetStatus(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                            JFJochProtoBuf::BrokerStatus *response) {
    try {
        auto tmp = services.GetReceiverStatus();
        *response->mutable_receiver_status() = tmp;
    } catch (JFJochException &e) {} // ignore exception in getting receiver status (don't really care, e.g. if receiver is down)

    try {
        auto tmp = services.GetIndexerStatus();
        *response->mutable_indexer_status() = tmp;
    } catch (JFJochException &e) {} // ignore exception in getting receiver status (don't really care, e.g. if receiver is down)

    auto last_receiver_output = state_machine.GetLastReceiverOutput();
    *response->mutable_module_statistics() = last_receiver_output.calibration().module_statistics();

    response->set_last_measurement_compression_ratio(last_receiver_output.compressed_ratio());
    response->set_last_measurement_collection_efficiency(last_receiver_output.efficiency());
    response->set_last_measurement_compression_ratio(last_receiver_output.compressed_ratio());
    response->set_last_measurement_images_collected(last_receiver_output.images_sent());
    response->set_last_measurement_name(last_receiver_output.jungfraujoch_settings().image_saving().file_prefix());
    response->set_last_measurement_sequence_number(last_receiver_output.jungfraujoch_settings().image_saving().measurement_sequence_num());

    switch (state_machine.GetState()) {
        case JFJochState::Inactive:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::NOT_INITIALIZED);
            break;
        case JFJochState::Idle:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::IDLE);
            break;
        case JFJochState::Measuring:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::DATA_COLLECTION);
            break;
        case JFJochState::Error:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::ERROR);
            break;
        case JFJochState::Busy:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::BUSY);
            break;
        case JFJochState::Pedestal:
            response->set_current_state(JFJochProtoBuf::BrokerStatus::PEDESTAL);
            break;
    }
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetCalibration(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                 JFJochProtoBuf::JFCalibration *response) {
    *response = state_machine.GetCalibration();
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::SetDataProcessingSettings(grpc::ServerContext *context,
                                                            const JFJochProtoBuf::DataProcessingSettings *request,
                                                            JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    data_processing_settings = *request;
    services.SetDataProcessingSettings(data_processing_settings);
    return grpc::Status::OK;
}

grpc::Status
JFJochBrokerService::GetDataProcessingSettings(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                               JFJochProtoBuf::DataProcessingSettings *response) {
    *response = GetDataProcessingSettings();
    return grpc::Status::OK;
}

JFJochProtoBuf::DataProcessingSettings JFJochBrokerService::GetDataProcessingSettings() {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    return data_processing_settings;
}

grpc::Status
JFJochBrokerService::GetDetailedReceiverOutput(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                               JFJochProtoBuf::JFJochReceiverOutput *response) {
    *response = state_machine.GetLastReceiverOutput();
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetMask(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                          JFJochProtoBuf::Image *response) {
    std::vector<uint32_t> mask;

    state_machine.GetCalibration().GetMaskTransformed(experiment, mask);

    response->set_width(experiment.GetXPixelsNum());
    response->set_height(experiment.GetYPixelsNum());
    *response->mutable_data() = {mask.begin(), mask.end()};

    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::LoadMask(grpc::ServerContext *context, const JFJochProtoBuf::MaskToLoad *request,
                                           JFJochProtoBuf::Empty *response) {
    if (request->mask_size() != experiment.GetPixelsNum())
        return {grpc::StatusCode::ABORTED, "Wrong mask size"};
    if ((request->bit_to_set() <= 0) || (request->bit_to_set() > 32))
        return {grpc::StatusCode::ABORTED, "Can only set bits 1 to 32"};
    std::vector<uint32_t> mask = {request->mask().begin(), request->mask().end()};
    state_machine.LoadMask(experiment, mask, request->bit_to_set());
    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetPedestalG0(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                JFJochProtoBuf::Image *response) {
    auto &pedestal = state_machine.GetCalibration().Pedestal(0);

    response->set_width(RAW_MODULE_COLS);
    response->set_height(experiment.GetModulesNum() * RAW_MODULE_LINES);
    *response->mutable_data() = {pedestal.begin(), pedestal.end()};

    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetMaskRawCoord(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                  JFJochProtoBuf::Image *response) {
    auto &mask = state_machine.GetCalibration().Mask();

    response->set_width(RAW_MODULE_COLS);
    response->set_height(experiment.GetModulesNum() * RAW_MODULE_LINES);
    *response->mutable_data() = {mask.begin(), mask.end()};

    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetPedestalG1(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                JFJochProtoBuf::Image *response) {
    auto &pedestal = state_machine.GetCalibration().Pedestal(0);

    response->set_width(RAW_MODULE_COLS);
    response->set_height(experiment.GetModulesNum() * RAW_MODULE_LINES);
    *response->mutable_data() = {pedestal.begin(), pedestal.end()};

    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetPedestalG2(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                JFJochProtoBuf::Image *response) {
    auto &pedestal = state_machine.GetCalibration().Pedestal(0);

    response->set_width(RAW_MODULE_COLS);
    response->set_height(experiment.GetModulesNum() * RAW_MODULE_LINES);
    *response->mutable_data() = {pedestal.begin(), pedestal.end()};

    return grpc::Status::OK;
}

grpc::Status JFJochBrokerService::GetPreviewFrame(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                                  JFJochProtoBuf::PreviewFrame *response) {
    try {
        *response = services.GetPreviewFrame();
    } catch (JFJochException &e) {
        logger.ErrorException(e);
        return {grpc::StatusCode::ABORTED, e.what()};
    }
    return grpc::Status::OK;
}