// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>

#include "JFJochStateMachine.h"
#include "../common/JFJochException.h"

JFJochStateMachine::JFJochStateMachine(JFJochServices &in_services, const DiffractionExperiment &experiment)
: services(in_services) {
    calibration = std::make_unique<JFCalibration>(experiment);
}

JFJochState JFJochStateMachine::GetState() const {
    return state;
}

void JFJochStateMachine::ImportPedestalG0(const DiffractionExperiment &experiment,
                                          const JFJochProtoBuf::ReceiverOutput &receiver_output) {
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

void JFJochStateMachine::TakePedestalInternalAll(const DiffractionExperiment &in_experiment) {
    calibration = std::make_unique<JFCalibration>(in_experiment);

    TakePedestalInternalG0(in_experiment);

    for (int i = 0; i < in_experiment.GetStorageCellNumber(); i++) {
        TakePedestalInternalG1(in_experiment, i);
        TakePedestalInternalG2(in_experiment, i);
    }
}

void JFJochStateMachine::TakePedestalInternalG0(const DiffractionExperiment &in_experiment) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG0);

    if (experiment.GetPedestalG0Frames() > 0) {
        services.Start(experiment, *calibration);
        auto pedestal_output = services.Stop(*calibration);
        SetFullMeasurementOutput(pedestal_output);
        ImportPedestalG0(in_experiment, pedestal_output.receiver());
    }
}

void JFJochStateMachine::TakePedestalInternalG1(const DiffractionExperiment &in_experiment, int32_t storage_cell) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG1);

    if (experiment.GetStorageCellNumber() == 2)
        experiment.StorageCellStart((storage_cell + 15) % 16); // one previous

    if (experiment.GetPedestalG1Frames() > 0) {
        services.Start(experiment, *calibration);
        auto pedestal_output = services.Stop(*calibration);
        SetFullMeasurementOutput(pedestal_output);
        ImportPedestal(pedestal_output.receiver(), 1, storage_cell);
    }
}

void JFJochStateMachine::TakePedestalInternalG2(const DiffractionExperiment &in_experiment, int32_t storage_cell) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG2);

    if (experiment.GetStorageCellNumber() == 2)
        experiment.StorageCellStart((storage_cell + 15) % 16); // one previous

    if (experiment.GetPedestalG2Frames() > 0) {
        services.Start(experiment, *calibration);
        auto pedestal_output = services.Stop(*calibration);
        SetFullMeasurementOutput(pedestal_output);
        ImportPedestal(pedestal_output.receiver(), 2, storage_cell);
    }
}

void JFJochStateMachine::Initialize(const DiffractionExperiment& experiment) {
    std::unique_lock<std::mutex> ul(m);

    if (state == JFJochState::Measuring)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Cannot initialize during measurement");

    state = JFJochState::Busy;
    try {
        services.On(experiment);
        TakePedestalInternalAll(experiment);
        state = JFJochState::Idle;
    } catch (...) {
        state = JFJochState::Error;
        throw;
    }
}

void JFJochStateMachine::TakePedestal(const DiffractionExperiment& in_experiment) {
    std::unique_lock<std::mutex> ul(m);
    DiffractionExperiment experiment(in_experiment);

    if (state != JFJochState::Idle)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,"Must be idle to take pedestal");

    try {
        TakePedestalInternalAll(experiment);
        state = JFJochState::Idle;
    } catch (...) {
        state = JFJochState::Error;
        throw;
    }
}

void JFJochStateMachine::MeasureStart(const DiffractionExperiment &in_experiment) {
    std::unique_lock<std::mutex> ul(m);

    DiffractionExperiment experiment(in_experiment);

    if (state != JFJochState::Idle)
        throw JFJochException(JFJochExceptionCategory::WrongDAQState,
                              "Must be idle to start measurement");

    if (measurement.valid())
        measurement.get(); // In case measurement was running - clear thread

    start_time = std::chrono::system_clock::now();

    try {
        state = JFJochState::Busy;
        services.Start(experiment, *calibration);

        state = JFJochState::Measuring;
        measurement = std::async(std::launch::async, &JFJochStateMachine::WaitTillMeasurementDone, this);
    } catch (...) {
        state = JFJochState::Error;
        services.Abort();
        throw;
    }
}

void JFJochStateMachine::MeasureEnd() {
    std::unique_lock<std::mutex> ul(m);

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
    if (state == JFJochState::Measuring)
        services.Abort();
}

void JFJochStateMachine::Cancel() {
    // This is inconsistency in naming - need to solve later
    std::unique_lock<std::mutex> ul(m);
    if (state == JFJochState::Measuring)
        services.Cancel();
}

void JFJochStateMachine::MoveToErrorState() {
    std::unique_lock<std::mutex> ul(m);
    state = JFJochState::Error;
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

JFJochProtoBuf::MeasurementStatistics JFJochStateMachine::GetMeasurementStatistics() const {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    return last_measurement_statistics;
}


void JFJochStateMachine::SetFullMeasurementOutput(JFJochProtoBuf::BrokerFullStatus &output) {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    last_receiver_output = output;

    last_measurement_statistics.set_compression_ratio(last_receiver_output.receiver().compressed_ratio());
    last_measurement_statistics.set_collection_efficiency(last_receiver_output.receiver().efficiency());
    last_measurement_statistics.set_images_collected(last_receiver_output.receiver().images_sent());
    last_measurement_statistics.set_file_name(last_receiver_output.receiver().master_file_name());
    if (last_receiver_output.receiver().jungfraujoch_settings().sample().run_number() != DiffractionExperiment::RunNumberNotSet)
        last_measurement_statistics.set_run_number(last_receiver_output.receiver().jungfraujoch_settings().sample().run_number());

    double writer_perf = 0.0;
    int64_t images_written = 0;

    for (const auto &i: last_receiver_output.writer()) {
        writer_perf += i.performance_mbs();
        images_written += i.nimages();
    }

    last_measurement_statistics.set_images_written(images_written);
    last_measurement_statistics.set_writer_performance_mbs(writer_perf);
}

JFCalibration JFJochStateMachine::GetCalibration() const {
    std::unique_lock<std::mutex> ul(m);
    return *calibration;
}

void JFJochStateMachine::LoadMask(const DiffractionExperiment &experiment, const std::vector<uint32_t> &vec, uint32_t bit) {
    std::unique_lock<std::mutex> ul(m);
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
