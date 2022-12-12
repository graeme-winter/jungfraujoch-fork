// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <thread>

#include "DetectorWrapper.h"
#include "../common/JFJochException.h"
#include "../common/Definitions.h"

void DetectorWrapper::Configure(const JFJochProtoBuf::DetectorConfig &request) {
    if (det.size() != request.modules_size())
        throw JFJochException(JFJochExceptionCategory::Detector,
                              "Discrepancy in module number between DAQ and detector");

    try {
        InternalStop();

        det.setNumberofUDPInterfaces(2);

        for (int i = 0; i < request.modules_size(); i++) {
            auto &cfg = request.modules(i);

            det.setSourceUDPIP(sls::IpAddr(cfg.ipv4_src_addr_1()), {i});
            det.setSourceUDPIP2(sls::IpAddr(cfg.ipv4_src_addr_2()), {i});
            det.setSourceUDPMAC(sls::MacAddr(BASE_DETECTOR_MAC + i * 2), {i});
            det.setSourceUDPMAC2(sls::MacAddr(BASE_DETECTOR_MAC + i * 2 + 1), {i});

            det.setDestinationUDPPort (cfg.udp_dest_port_1(), i);
            det.setDestinationUDPPort2(cfg.udp_dest_port_2(), i);
            det.setDestinationUDPIP(  sls::IpAddr(cfg.ipv4_dest_addr_1()), {i});
            det.setDestinationUDPIP2( sls::IpAddr(cfg.ipv4_dest_addr_2()), {i});
            det.setDestinationUDPMAC( sls::MacAddr(cfg.mac_addr_dest_1()), {i});
            det.setDestinationUDPMAC2(sls::MacAddr(cfg.mac_addr_dest_2()), {i});
        }

        det.setTemperatureControl(true);
        det.setThresholdTemperature(55);

        det.clearBit(0x4f, 15, {0});
        det.clearBit(0x4e, 4);

        det.setTimingMode(slsDetectorDefs::timingMode::TRIGGER_EXPOSURE);
        det.setAutoComparatorDisable(true);
        if (!det.getPowerChip().squash(false)) {
            det.setPowerChip(true);
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }

        det.setHighVoltage(HIGH_VOLTAGE);
    } catch (const std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

void DetectorWrapper::Start(const JFJochProtoBuf::DetectorInput &request) {
    if (det.size() != request.modules_num())
       throw JFJochException(JFJochExceptionCategory::Detector,
                             "Discrepancy in module number between DAQ and detector");

    try {
        InternalStop();
        switch (request.mode()) {
            case JFJochProtoBuf::PEDESTAL_G1:
                det.setGainMode(slsDetectorDefs::gainMode::FORCE_SWITCH_G1);
                break;
            case JFJochProtoBuf::PEDESTAL_G2:
                det.setGainMode(slsDetectorDefs::gainMode::FORCE_SWITCH_G2);
                break;
            default:
                det.setGainMode(slsDetectorDefs::gainMode::DYNAMIC);
                break;
        }

        det.setNextFrameNumber(1);
        det.setNumberOfFrames(request.num_frames());
        det.setNumberOfTriggers(request.num_triggers());
        det.setStorageCellStart(request.storage_cell_start());
        det.setNumberOfAdditionalStorageCells(request.storage_cell_number() - 1);
        det.setStorageCellDelay(std::chrono::microseconds(request.storage_cell_delay()));

        if (request.period_us() < MIN_FRAME_TIME_HALF_SPEED_IN_US)
            det.setReadoutSpeed(slsDetectorDefs::speedLevel::FULL_SPEED);
        else
            det.setReadoutSpeed(slsDetectorDefs::speedLevel::HALF_SPEED);

        det.setPeriod(std::chrono::microseconds(request.period_us()));
        det.setExptime(std::chrono::microseconds(request.count_time_us()));

        // Always synchronize to master
        det.setBit(0x4f, 15, {0});
        det.setBit(0x4e, 4);

        det.startDetector();
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

void DetectorWrapper::InternalStop() {
    // Assume it is executed in try-catch!
    auto state = GetState();
    if (state == DetectorState::ERROR)
        throw JFJochException(JFJochExceptionCategory::Detector,
                              "Detector in error state");
    else if (state == DetectorState::BUSY) {
        det.stopDetector();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        state = GetState();
        if (state != DetectorState::IDLE)
            throw JFJochException(JFJochExceptionCategory::Detector,
                                  "Detector busy and cannot be stopped");
    }
}

void DetectorWrapper::Deactivate() {
    try {
        InternalStop();
        det.setHighVoltage(0);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        det.setPowerChip(false);
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

void DetectorWrapper::Stop() {
    try {
        InternalStop();
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

void DetectorWrapper::Trigger() {
    try {
        for (int i = 1; i < det.size(); i++)
            det.setBit(0x4f, 2, {i});
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        det.setBit(0x4f, 2, {0});
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        det.clearBit(0x4f, 2);
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

DetectorWrapper::DetectorState DetectorWrapper::GetState() const {
    try {
        bool is_idle = true;
        for (auto & i : det.getDetectorStatus()) {
            if (i == slsDetectorDefs::runStatus::ERROR)
                return DetectorState::ERROR;
            if ((i != slsDetectorDefs::runStatus::IDLE) &&
                (i != slsDetectorDefs::runStatus::STOPPED) &&
                (i != slsDetectorDefs::runStatus::RUN_FINISHED))
                is_idle = false;
        }
        if (is_idle)
            return DetectorState::IDLE;
        else
            return DetectorState::BUSY;
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

int64_t DetectorWrapper::GetFirmwareVersion() {
    try {
       auto result = det.getFirmwareVersion();
       return result.squash(0x0);
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}

int64_t DetectorWrapper::GetDetectorServerVersion() {
    try {
        auto result = det.getDetectorServerVersion();
        return result.squash(0x0);
    } catch (std::exception &e) {
        throw JFJochException(JFJochExceptionCategory::Detector, e.what());
    }
}
