// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>

#include "JFJochStateMachine.h"
#include "../common/JFJochException.h"

JFJochStateMachine::JFJochStateMachine(JFJochServices &in_services)
: services(in_services),
data_processing_settings(DiffractionExperiment::DefaultDataProcessingSettings()) {

}

void JFJochStateMachine::ImportPedestalG0(const JFJochProtoBuf::ReceiverOutput &receiver_output) {
    if (receiver_output.pedestal_result_size() != experiment.GetModulesNum() * experiment.GetStorageCellNumber())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Mismatch in pedestal output");

    for (int s = 0; s < experiment.GetStorageCellNumber(); s++) {
        for (int module = 0; module < experiment.GetModulesNum(); module++)
            calibration->Pedestal(module, 0, s)
                    = receiver_output.pedestal_result(module + s * experiment.GetModulesNum());
    }
    SetCalibrationStatistics(calibration->GetModuleStatistics());
}

void JFJochStateMachine::ImportPedestal(const JFJochProtoBuf::ReceiverOutput &receiver_output, size_t gain_level,
                                        size_t storage_cell) {
    for (int i = 0; i < receiver_output.pedestal_result_size(); i++)
        calibration->Pedestal(i, gain_level, storage_cell) = receiver_output.pedestal_result(i);
    SetCalibrationStatistics(calibration->GetModuleStatistics());
}

void JFJochStateMachine::TakePedestalInternalAll(std::unique_lock<std::mutex> &ul) {
    calibration = std::make_unique<JFCalibration>(experiment);

    cancel_sequence = false;

    TakePedestalInternalG0(ul);

    for (int i = 0; i < experiment.GetStorageCellNumber(); i++) {
        TakePedestalInternalG1(ul, i);
        TakePedestalInternalG2(ul, i);
    }
}

void JFJochStateMachine::TakePedestalInternalG0(std::unique_lock<std::mutex> &ul) {
    state = JFJochState::Pedestal;
    DiffractionExperiment local_experiment(experiment);
    local_experiment.Mode(DetectorMode::PedestalG0);

    if (!cancel_sequence && (local_experiment.GetPedestalG0Frames() > 0)) {
        services.Start(local_experiment, *calibration);
        services.Trigger();
        ul.unlock();
        // Allow to cancel/abort during the pedestal data collection
        // Must ensure that while state is Pedestal, nothing can take lock for longer time, to avoid deadlock
        auto pedestal_output = services.Stop(*calibration);
        ul.lock();

        // SetFullMeasurementOutput(pedestal_output);
        ImportPedestalG0(pedestal_output.receiver());
    }
}

void JFJochStateMachine::TakePedestalInternalG1(std::unique_lock<std::mutex> &ul, int32_t storage_cell) {
    state = JFJochState::Pedestal;
    DiffractionExperiment local_experiment(experiment);
    local_experiment.Mode(DetectorMode::PedestalG1);

    if (local_experiment.GetStorageCellNumber() == 2)
        local_experiment.StorageCellStart((storage_cell + 15) % 16); // one previous

    if (!cancel_sequence && (local_experiment.GetPedestalG1Frames() > 0)) {
        services.Start(local_experiment, *calibration);
        services.Trigger();
        ul.unlock();
        // Allow to cancel/abort during the pedestal data collection
        // Must ensure that while state is Pedestal, nothing can take lock for longer time, to avoid deadlock
        auto pedestal_output = services.Stop(*calibration);
        ul.lock();

        // SetFullMeasurementOutput(pedestal_output);
        ImportPedestal(pedestal_output.receiver(), 1, storage_cell);
    }
}

void JFJochStateMachine::TakePedestalInternalG2(std::unique_lock<std::mutex> &ul, int32_t storage_cell) {
    state = JFJochState::Pedestal;
    DiffractionExperiment local_experiment(experiment);
    local_experiment.Mode(DetectorMode::PedestalG2);

    if (local_experiment.GetStorageCellNumber() == 2)
        local_experiment.StorageCellStart((storage_cell + 15) % 16); // one previous

    if (!cancel_sequence && (local_experiment.GetPedestalG2Frames() > 0)) {
        services.Start(local_experiment, *calibration);
        services.Trigger();
        ul.unlock();
        // Allow to cancel/abort during the pedestal data collection
        // Must ensure that while state is Pedestal, nothing can take lock for longer time, to avoid deadlock
        auto pedestal_output = services.Stop(*calibration);
        ul.lock();

        // SetFullMeasurementOutput(pedestal_output);
        ImportPedestal(pedestal_output.receiver(), 2, storage_cell);
    }
}

void JFJochStateMachine::Initialize() {
    std::unique_lock<std::mutex> ul(m);

    if ((state == JFJochState::Measuring) || (state == JFJochState::Pedestal))
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Cannot initialize during measurement");

    state = JFJochState::Busy;
    ClearMeasurementStatistics();
    try {
        services.On(experiment);
        TakePedestalInternalAll(ul);
        state = JFJochState::Idle;
    } catch (...) {
        state = JFJochState::Error;
        throw;
    }
}

void JFJochStateMachine::Pedestal() {
    std::unique_lock<std::mutex> ul(m);

    if (state != JFJochState::Idle)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,"Must be idle to take pedestal");
    try {
        TakePedestalInternalAll(ul);
        state = JFJochState::Idle;
    } catch (...) {
        state = JFJochState::Error;
        throw;
    }
}

void JFJochStateMachine::Trigger() {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Measuring)
        services.Trigger();
}

void JFJochStateMachine::Start(const JFJochProtoBuf::DatasetSettings& settings) {
    std::unique_lock<std::mutex> ul(m);

    if (state != JFJochState::Idle)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Must be idle to start measurement");

    if (measurement.valid())
        measurement.get(); // In case measurement was running - clear thread

    ClearMeasurementStatistics();

    auto mod_settings = settings;
    SetDatasetDefaults(mod_settings);
    experiment.LoadDatasetSettings(mod_settings);

    cancel_sequence = false;

    try {
        state = JFJochState::Busy;
        services.SetDataProcessingSettings(GetDataProcessingSettings());
        services.Start(experiment, *calibration);

        state = JFJochState::Measuring;
        measurement = std::async(std::launch::async, &JFJochStateMachine::WaitTillMeasurementDone, this);
    } catch (...) {
        state = JFJochState::Error;
        services.Abort();
        throw;
    }
}

void JFJochStateMachine::SetDatasetDefaults(JFJochProtoBuf::DatasetSettings &settings) {
    if (settings.data_file_count() <= 0)
        settings.set_data_file_count(std::max<int64_t>(1, services.WriterZMQCount()));
    if (settings.detector_distance_mm() <= 0)
        settings.set_detector_distance_mm(100);
    if (settings.ntrigger() <= 0)
        settings.set_ntrigger(1);
}

void JFJochStateMachine::Stop() {
    std::unique_lock<std::mutex> ul(m);

    if (state == JFJochState::Pedestal)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Cannot use the function during pedestal collection");

    c.wait(ul, [&] { return state != JFJochState::Measuring; });

    if (!measurement.valid())
        return; // This is for unlikely condition of two parallel stops
    else
        measurement.get();

    switch (state) {
        case JFJochState::Inactive:
            throw JFJochException(JFJochExceptionCategory::WrongDAQState,"Not initialized");
        case JFJochState::Error:
            throw JFJochException(JFJochExceptionCategory::WrongDAQState,"Detector in error state");
        case JFJochState::Measuring:
        case JFJochState::Busy:
        case JFJochState::Pedestal:
            throw JFJochException(JFJochExceptionCategory::WrongDAQState,"Detector in not expected state to end measurment");
        case JFJochState::Idle:
            break;
    }
}

void JFJochStateMachine::WaitTillMeasurementDone() {
    try {
        auto tmp_output = services.Stop(*calibration);
        SetFullMeasurementOutput(tmp_output);
        {
            std::unique_lock<std::mutex> ul(m);
            state = JFJochState::Idle;
        }
    } catch (...) {
        std::unique_lock<std::mutex> ul(m);
        state = JFJochState::Error;
    }
    c.notify_all();
}

void JFJochStateMachine::Abort() {
    // This is inconsistency in naming - need to solve later
    std::unique_lock<std::mutex> ul(m);
    if ((state == JFJochState::Pedestal) || (state == JFJochState::Measuring)) {
        services.Abort();
        cancel_sequence = true;
    }
}

void JFJochStateMachine::Cancel() {
    // This is inconsistency in naming - need to solve later
    std::unique_lock<std::mutex> ul(m);
    if ((state == JFJochState::Pedestal) || (state == JFJochState::Measuring)) {
        services.Cancel();
        cancel_sequence = true;
    }
}

void JFJochStateMachine::DebugOnly_SetState(JFJochState in_state) {
    std::unique_lock<std::mutex> ul(m);
    state = in_state;
}

void JFJochStateMachine::Deactivate() {
    std::unique_lock<std::mutex> ul(m);
    try {
        if (measurement.valid())
            measurement.get();
        services.Off();
        state = JFJochState::Inactive;
    } catch (...) {
        state = JFJochState::Error;
        throw;
    }
}

JFJochStateMachine::~JFJochStateMachine() {
    try {
        if (measurement.valid())
            measurement.get();
    } catch (...) {}
}

JFJochProtoBuf::BrokerFullStatus JFJochStateMachine::GetFullMeasurementOutput() const {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    return last_receiver_output;
}

void JFJochStateMachine::SetFullMeasurementOutput(JFJochProtoBuf::BrokerFullStatus &output) {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    last_receiver_output = output;

    auto tmp_last_measurement_statistics = JFJochProtoBuf::MeasurementStatistics(); // reset last measurement statistics

    tmp_last_measurement_statistics.set_file_prefix(experiment.GetFilePrefix());
    tmp_last_measurement_statistics.set_detector_width(experiment.GetXPixelsNum());
    tmp_last_measurement_statistics.set_detector_height(experiment.GetYPixelsNum());
    tmp_last_measurement_statistics.set_detector_pixel_depth(experiment.GetPixelDepth());

    if (last_receiver_output.has_receiver()) {
        tmp_last_measurement_statistics.set_compression_ratio(output.receiver().compressed_ratio());
        tmp_last_measurement_statistics.set_collection_efficiency(output.receiver().efficiency());
        tmp_last_measurement_statistics.set_images_collected(output.receiver().images_sent());
        tmp_last_measurement_statistics.set_cancelled(output.receiver().cancelled());
        tmp_last_measurement_statistics.set_max_image_number_sent(output.receiver().max_image_number_sent());
        tmp_last_measurement_statistics.set_max_receive_delay(output.receiver().max_receive_delay());
    }

    if (last_receiver_output.writer_size() > 0) {
        double writer_perf = 0.0;
        int64_t images_written = 0;

        for (const auto &i: output.writer()) {
            writer_perf += i.performance_mbs();
            images_written += i.nimages();
        }

        tmp_last_measurement_statistics.set_writer_performance_mbs(writer_perf);
        tmp_last_measurement_statistics.set_images_written(images_written);
    }

    if (output.has_indexer()) {
        double indexed = output.indexer().indexed_images();
        double analyzed = output.indexer().image_output_size();
        if (analyzed > 0)
            tmp_last_measurement_statistics.set_indexing_rate(indexed / analyzed);
        tmp_last_measurement_statistics.set_indexing_performance_ms(output.indexer().ms_per_image());
    }
    last_measurement_statistics = tmp_last_measurement_statistics;
}

void JFJochStateMachine::ClearMeasurementStatistics() {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    last_measurement_statistics.reset();
}

std::optional<JFJochProtoBuf::MeasurementStatistics> JFJochStateMachine::GetMeasurementStatistics() const {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    return last_measurement_statistics;
}

JFCalibration JFJochStateMachine::GetCalibration() const {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState, "Detector not calibrated");
    return *calibration;
}

void JFJochStateMachine::LoadMask(const std::vector<uint32_t> &vec, uint32_t bit) {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Detector not calibrated");
    if (state != JFJochState::Idle)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Cannot load mask if detector is not idle");
    calibration->LoadMask(experiment, vec, bit);
}

JFJochProtoBuf::JFCalibrationStatistics JFJochStateMachine::GetCalibrationStatistics() const {
    std::unique_lock<std::mutex> ul(calibration_statistics_mutex);
    return calibration_statistics;
}

void JFJochStateMachine::SetCalibrationStatistics(const JFJochProtoBuf::JFCalibrationStatistics &input) {
    std::unique_lock<std::mutex> ul(calibration_statistics_mutex);
    calibration_statistics = input;
}

JFJochProtoBuf::DetectorSettings JFJochStateMachine::GetDetectorSettings() const {
    std::unique_lock<std::mutex> ul(m);
    return experiment.GetDetectorSettings();
}

void JFJochStateMachine::SetDetectorSettings(const JFJochProtoBuf::DetectorSettings &settings) {
    std::unique_lock<std::mutex> ul(m);
    switch (state) {
        case JFJochState::Inactive:
        case JFJochState::Error:
            experiment.LoadDetectorSettings(settings);
            break;
        case JFJochState::Idle:
            experiment.LoadDetectorSettings(settings);
            TakePedestalInternalAll(ul);
            break;
        case JFJochState::Measuring:
        case JFJochState::Busy:
        case JFJochState::Pedestal:
            throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                                  "Cannot change detector set during data collection");
    }
}

DiffractionExperiment &JFJochStateMachine::NotThreadSafe_Experiment() {
    return experiment;
}


JFJochProtoBuf::Image JFJochStateMachine::GetNeXusMask() const {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState, "Detector not calibrated");

    JFJochProtoBuf::Image ret;
    auto mask = calibration->CalculateNexusMask(experiment);

    ret.set_width(experiment.GetXPixelsNum());
    ret.set_height(experiment.GetYPixelsNum());
    ret.set_pixel_depth(4);
    *ret.mutable_data() = {mask.begin(), mask.end()};
    return ret;
}

JFJochProtoBuf::Image JFJochStateMachine::GetPedestalG0() const {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState, "Detector not calibrated");

    JFJochProtoBuf::Image ret;
    auto pedestal = calibration->GetPedestal(0);

    ret.set_width(experiment.GetXPixelsNum());
    ret.set_height(experiment.GetYPixelsNum());
    ret.set_pixel_depth(2);
    *ret.mutable_data() = {pedestal.begin(), pedestal.end()};
    return ret;
}

JFJochProtoBuf::Image JFJochStateMachine::GetPedestalG1() const {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState, "Detector not calibrated");

    JFJochProtoBuf::Image ret;
    auto pedestal = calibration->GetPedestal(1);

    ret.set_width(experiment.GetXPixelsNum());
    ret.set_height(experiment.GetYPixelsNum());
    ret.set_pixel_depth(2);
    *ret.mutable_data() = {pedestal.begin(), pedestal.end()};
    return ret;
}

JFJochProtoBuf::Image JFJochStateMachine::GetPedestalG2() const {
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Inactive)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState, "Detector not calibrated");

    JFJochProtoBuf::Image ret;
    auto pedestal = calibration->GetPedestal(2);

    ret.set_width(experiment.GetXPixelsNum());
    ret.set_height(experiment.GetYPixelsNum());
    ret.set_pixel_depth(2);
    *ret.mutable_data() = {pedestal.begin(), pedestal.end()};
    return ret;
}

JFJochProtoBuf::BrokerStatus JFJochStateMachine::GetStatus() const {
    JFJochProtoBuf::BrokerStatus ret;

    switch (state) {
        case JFJochState::Inactive:
            ret.set_broker_state(JFJochProtoBuf::NOT_INITIALIZED);
            break;
        case JFJochState::Idle:
            ret.set_broker_state(JFJochProtoBuf::IDLE);
            break;
        case JFJochState::Measuring:
            ret.set_broker_state(JFJochProtoBuf::DATA_COLLECTION);
            break;
        case JFJochState::Error:
            ret.set_broker_state(JFJochProtoBuf::ERROR);
            break;
        case JFJochState::Busy:
            ret.set_broker_state(JFJochProtoBuf::BUSY);
            break;
        case JFJochState::Pedestal:
            ret.set_broker_state(JFJochProtoBuf::PEDESTAL);
            break;
    }

    try {
        auto rcv_status = services.GetReceiverStatus();
        if (rcv_status.has_progress())
            ret.set_progress(rcv_status.progress());
    } catch (JFJochException &e) {} // ignore exception in getting receiver status (don't really care, e.g. if receiver is down)

    try {
        auto idx_status = services.GetIndexerStatus();
        if (idx_status.images_analyzed() > 0)
            ret.set_indexing_rate(static_cast<double>(idx_status.images_indexed())
                                        / static_cast<double>(idx_status.images_analyzed()));
    } catch (JFJochException &e) {} // ignore exception in getting receiver status (don't really care, e.g. if receiver is down)

    return ret;
}

JFJochProtoBuf::BrokerPlots JFJochStateMachine::GetPlots() const {
    return services.GetPlots();
}

void JFJochStateMachine::SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings) {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    DiffractionExperiment::CheckDataProcessingSettings(settings);
    data_processing_settings = settings;
    services.SetDataProcessingSettings(data_processing_settings);
}

JFJochProtoBuf::DataProcessingSettings JFJochStateMachine::GetDataProcessingSettings() const {
    std::unique_lock<std::mutex> ul(data_processing_settings_mutex);
    return data_processing_settings;
}

JFJochState JFJochStateMachine::GetState() const {
    return state;
}