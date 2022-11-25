// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H
#define JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H

#include <string>
#include <mutex>
#include <future>
#include "../common/DiffractionExperiment.h"
#include "../common/JFCalibration.h"
#include "JFJochServices.h"

enum class JFJochState {Inactive, Idle, Measuring, Error, Busy, Pedestal};

class JFJochStateMachine {
    mutable std::mutex m;
    std::condition_variable c;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::future<void> measurement;

    std::unique_ptr<JFCalibration> calibration;
    mutable std::mutex calibration_statistics_mutex;
    JFJochProtoBuf::JFCalibrationStatistics calibration_statistics;

    volatile JFJochState state = JFJochState::Inactive;
    JFJochServices &services;
    // Private functions assume that lock is acquired
    void WaitTillMeasurementDone();
    void ImportPedestal(const JFJochProtoBuf::ReceiverOutput &receiver_output, size_t gain_level, size_t storage_cell = 0);
    void ImportPedestalG0(const DiffractionExperiment &experiment,
                          const JFJochProtoBuf::ReceiverOutput &receiver_output);

    void TakePedestalInternalAll(const DiffractionExperiment &experiment);
    void TakePedestalInternalG0(const DiffractionExperiment &experiment);
    void TakePedestalInternalG1(const DiffractionExperiment &experiment, int32_t storage_cell = 0);
    void TakePedestalInternalG2(const DiffractionExperiment &experiment, int32_t storage_cell = 0);

    mutable std::mutex last_receiver_output_mutex;
    JFJochProtoBuf::BrokerFullStatus last_receiver_output;
    JFJochProtoBuf::MeasurementStatistics last_measurement_statistics;
    void SetFullMeasurementOutput(JFJochProtoBuf::BrokerFullStatus &output);
public:
    explicit JFJochStateMachine(JFJochServices &in_services, const DiffractionExperiment &experiment);
    ~JFJochStateMachine();
    JFJochState GetState() const;

    void Initialize(const DiffractionExperiment& experiment);
    void TakePedestal(const DiffractionExperiment& experiment);
    void Deactivate();
    void MoveToErrorState();
    void MeasureStart(const DiffractionExperiment& experiment);
    void MeasureEnd();

    void Abort();
    void Cancel();

    void LoadMask(const DiffractionExperiment& experiment, const std::vector<uint32_t> &vec, uint32_t bit);
    void SetCalibrationStatistics(const JFJochProtoBuf::JFCalibrationStatistics &input);
    JFCalibration GetCalibration() const;

    // return by value to ensure thread safety
    JFJochProtoBuf::BrokerFullStatus GetFullMeasurementOutput() const;
    JFJochProtoBuf::MeasurementStatistics GetMeasurementStatistics() const;
    JFJochProtoBuf::JFCalibrationStatistics GetCalibrationStatistics() const;
};


#endif //JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H
