// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H
#define JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H

#include <string>
#include <mutex>
#include <future>
#include <optional>

#include "../common/DiffractionExperiment.h"
#include "../common/JFCalibration.h"
#include "../common/Logger.h"

#include "JFJochServices.h"

enum class JFJochState {Inactive, Idle, Measuring, Error, Busy, Pedestal};

class JFJochStateMachine {
    JFJochServices &services;

    mutable std::mutex m;
    std::condition_variable c;
    // mutex m is protecting:
    DiffractionExperiment experiment;
    volatile JFJochState state = JFJochState::Inactive;
    volatile bool cancel_sequence = false;
    std::unique_ptr<JFCalibration> calibration;

    std::future<void> measurement;

    mutable std::mutex calibration_statistics_mutex;
    JFJochProtoBuf::JFCalibrationStatistics calibration_statistics;

    mutable std::mutex last_receiver_output_mutex;
    JFJochProtoBuf::BrokerFullStatus last_receiver_output;
    std::optional<JFJochProtoBuf::MeasurementStatistics> last_measurement_statistics;
    void SetFullMeasurementOutput(JFJochProtoBuf::BrokerFullStatus &output);
    void ClearMeasurementStatistics();
    JFJochProtoBuf::BrokerFullStatus GetFullMeasurementOutput() const;

    mutable std::mutex data_processing_settings_mutex;
    JFJochProtoBuf::DataProcessingSettings data_processing_settings;

    // Private functions assume that lock m is acquired
    void SetDatasetDefaults(JFJochProtoBuf::DatasetSettings& settings);
    void WaitTillMeasurementDone();
    void ImportPedestal(const JFJochProtoBuf::ReceiverOutput &receiver_output, size_t gain_level, size_t storage_cell = 0);
    void ImportPedestalG0(const JFJochProtoBuf::ReceiverOutput &receiver_output);

    void TakePedestalInternalAll(std::unique_lock<std::mutex> &ul);
    void TakePedestalInternalG0(std::unique_lock<std::mutex> &ul);
    void TakePedestalInternalG1(std::unique_lock<std::mutex> &ul, int32_t storage_cell = 0);
    void TakePedestalInternalG2(std::unique_lock<std::mutex> &ul, int32_t storage_cell = 0);
public:
    explicit JFJochStateMachine(JFJochServices &in_services);
    ~JFJochStateMachine();

    void Initialize();
    void Pedestal();
    void Deactivate();
    void Start(const JFJochProtoBuf::DatasetSettings& settings);
    void Stop();
    void Trigger();

    void Abort();
    void Cancel();

    void LoadMask(const std::vector<uint32_t> &vec, uint32_t bit);
    void SetCalibrationStatistics(const JFJochProtoBuf::JFCalibrationStatistics &input);
    JFCalibration GetCalibration() const;

    JFJochProtoBuf::DetectorSettings GetDetectorSettings() const;
    void SetDetectorSettings(const JFJochProtoBuf::DetectorSettings& settings);

    // return by value to ensure thread safety
    std::optional<JFJochProtoBuf::MeasurementStatistics> GetMeasurementStatistics() const;
    JFJochProtoBuf::JFCalibrationStatistics GetCalibrationStatistics() const;

    JFJochProtoBuf::BrokerStatus GetStatus() const;
    JFJochProtoBuf::BrokerPlots GetPlots() const;

    JFJochProtoBuf::Image GetNeXusMask() const;
    JFJochProtoBuf::Image GetPedestalG0() const;
    JFJochProtoBuf::Image GetPedestalG1() const;
    JFJochProtoBuf::Image GetPedestalG2() const;

    void SetDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings& settings);
    JFJochProtoBuf::DataProcessingSettings GetDataProcessingSettings() const;

    JFJochState GetState() const;

    // Not thread safe - only for configuration in serial context
    DiffractionExperiment& NotThreadSafe_Experiment();

    // Function for debug only - UNSAFE for real operation
    void DebugOnly_SetState(JFJochState state);
};


#endif //JUNGFRAUJOCH_JFJOCHSTATEMACHINE_H
