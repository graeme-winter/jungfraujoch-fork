// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONDEVICE_H
#define JUNGFRAUJOCH_ACQUISITIONDEVICE_H

#include <map>
#include <mutex>
#include <future>
#include <unistd.h>

#include "../../common/Definitions.h"
#include "../../common/DiffractionExperiment.h"
#include "../../common/Logger.h"
#include "../../common/JFCalibration.h"
#include "../../common/ThreadSafeFIFO.h"

#include "jfjoch.pb.h"

#include "ActionConfig.h"
#include "AcquisitionDeviceFilter.h"
#include "AcquisitionDeviceCounters.h"
#include "Completion.h"

void *mmap_acquisition_buffer(size_t size, int16_t numa_node);

class AcquisitionDevice {
    uint32_t packets_ok = 0;

    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain0;
    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain1;
    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain2;

    std::vector<int16_t> buffer_err;

    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;

    void UnmapBuffers();
    void FillActionRegister(const DiffractionExperiment& x, ActionConfig& job);

    AcquisitionDeviceFilter filter;
    AcquisitionDeviceCounters counters;

    ThreadSafeFIFO<Completion> work_completion_queue;
    std::future<void> read_work_completion_future;
    void ReadWorkCompletionThread();

    ThreadSafeSet<uint32_t> work_request_queue;
    std::future<void> send_work_request_future;

    void SendWorkRequestThread();
    void EndWorkRequestAndSignalQueues();

    Completion ReadCompletion();
    void ReadMailbox(uint32_t values[4]);

    virtual void HW_WriteActionRegister(const ActionConfig *job) = 0;
    virtual void HW_ReadActionRegister(ActionConfig *job) = 0;
    virtual void HW_StartAction() = 0;
    virtual bool HW_IsIdle() const = 0;
    virtual bool HW_ReadMailbox(uint32_t values[4]) = 0;
    virtual void HW_SetCancelDataCollectionBit() = 0;
    virtual bool HW_SendWorkRequest(uint32_t handle) = 0;
    virtual uint32_t HW_GetMaxModuleNum() = 0;
    virtual uint32_t HW_GetInternalPacketGeneratorModuleNum() = 0;
    virtual void HW_GetStatus(ActionStatus *status) const {
        memset(status, 0, sizeof(ActionStatus));
    };
    virtual void HW_GetEnvParams(ActionEnvParams *status) const {
        memset(status, 0, sizeof(ActionEnvParams));
    }
    virtual uint64_t HW_GetMACAddress() const = 0;
protected:
    std::vector<int16_t *> buffer_c2h;
    std::vector<uint16_t *> buffer_h2c;

    uint16_t data_stream;
    Logger *logger;

    AcquisitionDevice(uint16_t data_stream);

    void SetDefaultInternalGeneratorFrame(size_t buf_h2c_number);
    void MapBuffersStandard(size_t c2h_buffer_count, size_t h2c_buffer_count, int16_t numa_node);
public:
    static const uint16_t HandleNotValid = UINT16_MAX;

    virtual ~AcquisitionDevice() { UnmapBuffers(); };

    void StartAction(const DiffractionExperiment &experiment);
    void PrepareAction(const DiffractionExperiment &experiment);

    void WaitForActionComplete();
    void ActionAbort();

    void SendWorkRequest(uint32_t handle);
    void EnableLogging(Logger *logger);

    // Post measurement statistics - only guaranteed valid after WaitForActionComplete ends
    uint32_t GetPacketsOK() const;

    void SaveStatistics(const DiffractionExperiment &experiment, JFJochProtoBuf::AcquisitionDeviceStatistics &statistics) const;
    JFJochProtoBuf::FPGAStatus GetStatus() const;

    // Internal frame generator
    void SetCustomInternalGeneratorFrame(const void *input, size_t input_size);
    template <class T> void SetCustomInternalGeneratorFrame(const std::vector<T> &v) {
        SetCustomInternalGeneratorFrame(v.data(), v.size() * sizeof(T));
    }

    const int16_t *GetFrameBuffer(size_t frame_number, uint16_t module) const;
    void FrameBufferRelease(size_t frame_number, uint16_t module);
    const int16_t *GetPacketBuffer(size_t frame_number, uint16_t module, uint16_t packet);

    // Calibration
    void InitializeCalibration(const DiffractionExperiment &experiment, const JFCalibration &calib);

    template<class T> void LoadModuleGain(const std::vector<T> &vector, uint16_t module);
    void LoadModuleGain(const std::string &filename, uint16_t module);

    uint64_t GetHead(uint8_t module) const;
    uint64_t GetSlowestHead() const;
    void WaitForFrame(size_t curr_frame, uint16_t module = UINT16_MAX) const;
    int64_t CalculateDelay(size_t curr_frame, uint16_t module = UINT16_MAX) const; // mutex acquired indirectly
    uint64_t GetPacketMaskHalfModule(size_t frame, uint8_t module) const;
    uint16_t GetBufferHandle(size_t frame, uint8_t module) const;
    uint16_t GetPacketCount(size_t frame, uint8_t module) const;
    uint16_t GetPacketCountHalfModule(size_t frame, uint8_t module) const;
    bool GetTriggerField(size_t frame, uint8_t module) const;
    ActionConfig ReadActionRegister();
    bool IsDone() const;

    uint32_t GetInternalPacketGeneratorModuleNum();

    std::string GetMACAddress() const;
};

inline uint64_t bit_concat(uint32_t high, uint32_t low) {
    return (uint64_t(high) << 32) | low;
}

#endif //JUNGFRAUJOCH_ACQUISITIONDEVICE_H
