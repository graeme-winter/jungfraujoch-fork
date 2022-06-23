// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <unistd.h>
#include <thread>

#include "JFJochDetector.h"
#include "../common/JFJochException.h"

JFJochDetector::JFJochDetector(Logger &in_logger) : logger(in_logger) {}

grpc::Status JFJochDetector::Start(grpc::ServerContext *context, const JFJochProtoBuf::JFJochDetectorInput *request,
                                   JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);

    if (!det)
        return {grpc::StatusCode::ABORTED, "Detector not initialized"};

    try {
        DiffractionExperiment x(request->jungfraujoch_settings());

        logger.Info("Detector Start request for " + x.GetFilePrefix());

        if (!x.CheckGitSha1Consistent())
            logger.Warning(x.CheckGitSha1Msg());

        //StopDetector();

        if (!IsIdle()) {
            logger.Error("Detector must be idle for Start");
            throw JFJochException(JFJochExceptionCategory::Detector, "Detector must be idle to setup");
        }
        if (det->size() != x.GetModulesNum())
            throw JFJochException(JFJochExceptionCategory::Detector, "Discrepancy in module number between DAQ and detector");

        switch (x.GetDetectorMode()) {
            case DetectorMode::PedestalG1:
                det->setGainMode(slsDetectorDefs::gainMode::FORCE_SWITCH_G1);
                break;
            case DetectorMode::PedestalG2:
                det->setGainMode(slsDetectorDefs::gainMode::FORCE_SWITCH_G2);
                break;
            default:
                det->setGainMode(slsDetectorDefs::gainMode::DYNAMIC);
                break;
        }

        det->setNextFrameNumber(1);

        if (x.GetTimeResolvedMode() && (
                (x.GetDetectorMode() == DetectorMode::Conversion) ||
                (x.GetDetectorMode() == DetectorMode::Raw))) {
            det->setNumberOfFrames(x.GetFrameNumPerTrigger());
            if (x.GetFrameNumPerTrigger() < DELAY_FRAMES_STOP_AND_QUIT)
                det->setNumberOfTriggers(x.GetNumTriggers() + x.GetPedestalG0Frames()/x.GetFrameNumPerTrigger()
                                         + DELAY_FRAMES_STOP_AND_QUIT);
            else
                det->setNumberOfTriggers(x.GetNumTriggers() + x.GetPedestalG0Frames()/x.GetFrameNumPerTrigger()
                                         + 1);
        } else {
            det->setNumberOfFrames(x.GetFrameNum() + DELAY_FRAMES_STOP_AND_QUIT);
            det->setNumberOfTriggers(1);
        }

        if (x.IsDetectorFullSpeed())
            det->setReadoutSpeed(slsDetectorDefs::speedLevel::FULL_SPEED);
        else
            det->setReadoutSpeed(slsDetectorDefs::speedLevel::HALF_SPEED);
        det->setPeriod(x.GetFrameTime());
        det->setExptime(x.GetFrameCountTime());

        if ((x.GetDetectorMode() == DetectorMode::PedestalG0)
            || (x.GetDetectorMode() == DetectorMode::PedestalG1)
            || (x.GetDetectorMode() == DetectorMode::PedestalG2))
            det->setTimingMode(slsDetectorDefs::timingMode::AUTO_TIMING);
        else
            det->setTimingMode(slsDetectorDefs::timingMode::TRIGGER_EXPOSURE);

        StartDetector();
        logger.Info("Detector running");

        Trigger();

        return grpc::Status::OK;
    } catch (sls::RuntimeError &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::Stop(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                  JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);

    if (!det)
        return {grpc::StatusCode::ABORTED, "Detector not initialized"};

    try {
        logger.Info("Stopping detector ....");
        if (!IsIdle()) {
            StopDetector();
            while (!IsIdle())
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        logger.Info("   ... detector stopped");
        return grpc::Status::OK;
    } catch (sls::RuntimeError &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

bool JFJochDetector::IsIdle() const {
    auto result = det->getDetectorStatus();
    bool is_idle = true;
    for (auto & i : result) {
        if (i == slsDetectorDefs::runStatus::ERROR)
            throw JFJochException(JFJochExceptionCategory::Detector, "Detector in error state");
        if ((i != slsDetectorDefs::runStatus::IDLE) &&
            (i != slsDetectorDefs::runStatus::STOPPED) &&
            (i != slsDetectorDefs::runStatus::RUN_FINISHED))
            is_idle = false;
    }
    return is_idle;
}

grpc::Status JFJochDetector::On(grpc::ServerContext *context, const JFJochProtoBuf::JFJochDetectorConfig *request,
                                JFJochProtoBuf::Empty *response) {
    std::unique_lock<std::mutex> ul(m);
    logger.Info("Detector on ...");
    try {
        if (!det)
            det = std::make_unique<sls::Detector>();

        if (request->modules_size() > det->size())
            throw JFJochException(JFJochExceptionCategory::WrongNumber,
                                  "Module number in real detector exceeded");

        StopDetector();
        logger.Info("   ... stop detector");

        // Ensure detector is not running
        StopDetector();

        det->setNumberofUDPInterfaces(2);
        logger.Info("   ... setup 2 UDP interfaces / module");

        for (int i = 0; i < request->modules_size(); i++) {
            auto &cfg = request->modules(i);

            det->setSourceUDPIP(sls::IpAddr(cfg.ipv4_src_addr_1()), {i});
            det->setSourceUDPIP2(sls::IpAddr(cfg.ipv4_src_addr_2()), {i});
            det->setSourceUDPMAC(sls::MacAddr(BASE_DETECTOR_MAC + i * 2), {i});
            det->setSourceUDPMAC2(sls::MacAddr(BASE_DETECTOR_MAC + i * 2 + 1), {i});

            det->setDestinationUDPPort (cfg.udp_dest_port_1(), i);
            det->setDestinationUDPPort2(cfg.udp_dest_port_2(), i);
            det->setDestinationUDPIP(  sls::IpAddr(cfg.ipv4_dest_addr_1()), {i});
            det->setDestinationUDPIP2( sls::IpAddr(cfg.ipv4_dest_addr_2()), {i});
            det->setDestinationUDPMAC( sls::MacAddr(cfg.mac_addr_dest_1()), {i});
            det->setDestinationUDPMAC2(sls::MacAddr(cfg.mac_addr_dest_2()), {i});
        }

        logger.Info("   ... setup UDP");

        det->setTemperatureControl(true);
        det->setThresholdTemperature(55);
        logger.Info("   ... setup temperature control");

        det->setAutoComparatorDisable(true);
        logger.Info("   ... setup auto comparator disable");

        det->setBit(0x5c, 18);
        det->setBit(0x5c, 19);
        logger.Info("   ... setup TTL detection at 50% of exposure");

        if (!det->getPowerChip().squash(false)) {
            det->setPowerChip(true);
            logger.Info("   ... set power chip on");
            logger.Info("   ... waiting 5 s");
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
/*
        det->setTimingMode(slsDetectorDefs::timingMode::AUTO_TIMING);
        det->setNumberOfAdditionalStorageCells(15);
        det->setNumberOfFrames(1);
        det->setExptime(std::chrono::microseconds(1));
        det->startDetector();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        det->setNumberOfAdditionalStorageCells(0);
*/
        det->setHighVoltage(HIGH_VOLTAGE);
        logger.Info("   ... set module bias voltage to " + std::to_string(HIGH_VOLTAGE) + " V");

        return grpc::Status::OK;
    } catch (const sls::RuntimeError &e) {
        logger.Error("SLS Detector: " + std::string(e.what()));
        return {grpc::StatusCode::ABORTED, e.what()};
    } catch (const JFJochException &e) {
        logger.Error(e.what());
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

void JFJochDetector::Off() {
    det->stopDetector();
    det->setHighVoltage(0);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    try {
        det->setPowerChip(false);
    } catch (...) {
        // This sometimes needs to be done second time...
        det->setPowerChip(false);
    }
    det.reset();
}

grpc::Status JFJochDetector::Off(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                 JFJochProtoBuf::Empty *response) {

    std::unique_lock<std::mutex> ul(m);

    try {
        Off();
        return grpc::Status::OK;
    } catch (std::runtime_error &e) {
        return {grpc::StatusCode::ABORTED, e.what()};
    }
}

grpc::Status JFJochDetector::Status(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                    JFJochProtoBuf::DetectorStatus *response) {
    if (!det)
        return {grpc::StatusCode::ABORTED, "Detector not yet initialized"};

    auto result = det->getDetectorStatus();
    bool is_idle = true;
    for (auto & i : result) {
        if ((i != slsDetectorDefs::runStatus::IDLE) &&
            (i != slsDetectorDefs::runStatus::STOPPED) &&
            (i != slsDetectorDefs::runStatus::RUN_FINISHED))
            is_idle = false;
    }
    response->set_idle(is_idle);
    return grpc::Status::OK;
}

JFJochDetector::~JFJochDetector() {
    try {
        Off();
    } catch (...) {

    }
}

void JFJochDetector::StartDetector() {
    det->setBit(0x4f, 14);
    det->clearBit(0x4f, 14);
    det->setBit(0x4f, 0);
    det->clearBit(0x4f, 0);
}

void JFJochDetector::StopDetector() {
    det->setBit(0x4f, 1);
    det->clearBit(0x4f, 1);
}

JFJochDetector &JFJochDetector::SerialPort(const std::string &device) {
    try {
        serial = std::make_unique<LibSerial::SerialPort>(device);
        serial->SetBaudRate(LibSerial::BaudRate::BAUD_9600);
        serial->SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial->SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial->SetParity(LibSerial::Parity::PARITY_NONE);
        serial->SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    }  catch (const std::runtime_error& e) {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        serial.reset();
        throw JFJochException(JFJochExceptionCategory::Detector, "Serial port error " +
                                                                 std::string(e.what()));
    }
    return *this;
}

void JFJochDetector::Trigger() {
    if (serial) {
        logger.Info("   ... trigger");
        serial->WriteByte('x');
        serial->DrainWriteBuffer();
    }
}

grpc::Status JFJochDetector::Trigger(grpc::ServerContext *context, const JFJochProtoBuf::Empty *request,
                                     JFJochProtoBuf::Empty *response) {
    Trigger();
    return grpc::Status::OK;
}