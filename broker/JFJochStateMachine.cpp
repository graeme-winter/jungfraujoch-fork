// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>

#include "JFJochStateMachine.h"
#include "../common/JFJochException.h"

JFJochStateMachine::JFJochStateMachine(JFJochServices &in_services, const DiffractionExperiment &experiment)
: services(in_services), calibration(experiment) {
}

JFJochState JFJochStateMachine::GetState() const {
    return state;
}

void JFJochStateMachine::ImportPedestalAndMask(const JFJochProtoBuf::JFJochReceiverOutput &receiver_output) {
    if (receiver_output.calibration().nmodules() > 0)
        calibration.ImportPedestalAndMask(receiver_output.calibration());
}

void JFJochStateMachine::TakePedestalInternalAll(const DiffractionExperiment &in_experiment) {
    TakePedestalInternalG0(in_experiment);
    TakePedestalInternalG1(in_experiment);
    TakePedestalInternalG2(in_experiment);
}

void JFJochStateMachine::TakePedestalInternalG0(const DiffractionExperiment &in_experiment) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG1);
    if (experiment.GetPedestalG1Frames() > 0) {
        services.Start(experiment, calibration);
        auto pedestal_output = services.Stop();
        SetLastReceiverOutput(pedestal_output);
        ImportPedestalAndMask(pedestal_output);
    }
}

void JFJochStateMachine::TakePedestalInternalG1(const DiffractionExperiment &in_experiment) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG2);
    if (experiment.GetPedestalG2Frames() > 0) {
        services.Start(experiment, calibration);
        auto pedestal_output = services.Stop();
        SetLastReceiverOutput(pedestal_output);
        ImportPedestalAndMask(pedestal_output);
    }
}

void JFJochStateMachine::TakePedestalInternalG2(const DiffractionExperiment &in_experiment) {
    state = JFJochState::Pedestal;
    DiffractionExperiment experiment(in_experiment);
    experiment.Mode(DetectorMode::PedestalG0);

    if (experiment.GetPedestalG0Frames() > 0) {
        services.Start(experiment, calibration);
        auto pedestal_output = services.Stop();
        SetLastReceiverOutput(pedestal_output);
        ImportPedestalAndMask(pedestal_output);
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

    if (!services.IsDetectorIdle())
        throw JFJochException(JFJochExceptionCategory::Detector,
                               "Detector is not idle; restart at your own risk");

    if (measurement.valid())
        measurement.get(); // In case measurement was running - clear thread

    start_time = std::chrono::system_clock::now();

    try {
        TakePedestalInternalG1(experiment);
        TakePedestalInternalG2(experiment);
        TakePedestalInternalG0(experiment);

        state = JFJochState::Busy;
        services.Start(experiment, calibration);

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
        auto tmp_output = services.Stop();
        SetLastReceiverOutput(tmp_output);
        {
            std::unique_lock<std::mutex> ul(m);
            ImportPedestalAndMask(tmp_output);
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

JFJochProtoBuf::JFJochReceiverOutput JFJochStateMachine::GetLastReceiverOutput() const {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    return last_receiver_output;
}

JungfrauCalibration JFJochStateMachine::GetCalibration() const {
    std::unique_lock<std::mutex> ul(m);
    return calibration;
}

void JFJochStateMachine::SetLastReceiverOutput(JFJochProtoBuf::JFJochReceiverOutput &output) {
    std::unique_lock<std::mutex> ul(last_receiver_output_mutex);
    last_receiver_output = output;
}

void JFJochStateMachine::LoadMask(const DiffractionExperiment &experiment, const std::vector<uint32_t> &vec, uint32_t bit) {
    std::unique_lock<std::mutex> ul(m);
    calibration.LoadMaskTransformed(experiment, vec, bit);
}