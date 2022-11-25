// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ACQUISITIONDEVICE_H
#define JUNGFRAUJOCH_ACQUISITIONDEVICE_H

#include <map>
#include <mutex>
#include <future>
#include <unistd.h>

#include <jfjoch.pb.h>

#include "../../common/Definitions.h"
#include "../../common/DiffractionExperiment.h"
#include "../../common/Logger.h"
#include "../../common/JFCalibration.h"
#include "../../common/ThreadSafeFIFO.h"
#include "../../common/JFModuleGainCalibration.h"

#include "ActionConfig.h"
#include "AcquisitionOnlineCounters.h"
#include "Completion.h"
#include "AcquisitionOfflineCounters.h"

void *mmap_acquisition_buffer(size_t size, int16_t numa_node);

class AcquisitionDevice {
    uint64_t bytes_received = 0;

    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain0;
    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain1;
    std::vector<std::array<uint16_t, RAW_MODULE_SIZE> > gain2;

    std::vector<int16_t> buffer_err;

    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;


    void FillActionRegister(const DiffractionExperiment& x, ActionConfig& job);

    int64_t expected_frames;
    AcquisitionOnlineCounters counters;

    ThreadSafeFIFO<Completion> work_completion_queue;
    std::future<void> read_work_completion_future;
    void ReadWorkCompletionThread();

    ThreadSafeSet<uint64_t> work_request_queue;
    std::future<void> send_work_request_future;

    AcquisitionOfflineCounters completion_vector;

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
    virtual void HW_GetStatus(ActionStatus *status) const = 0;
    virtual void HW_GetEnvParams(ActionEnvParams *status) const {
        memset(status, 0, sizeof(ActionEnvParams));
    }
    virtual uint64_t HW_GetMACAddress() const = 0;
    virtual void HW_EndAction() {}; // do clean-up after action is done
    virtual void CopyInternalPacketGenFrameToDeviceBuffer();
protected:
    std::vector<uint16_t *> buffer_device;
    std::vector<uint16_t> internal_pkt_gen_frame;

    uint16_t data_stream;
    Logger *logger;
    uint32_t max_modules = 1;

    AcquisitionDevice(uint16_t data_stream);

    void UnmapBuffers();
    void MapBuffersStandard(size_t c2h_buffer_count, size_t h2c_buffer_count, int16_t numa_node);
public:
    static constexpr const uint64_t HandleNotValid = UINT64_MAX;

    virtual ~AcquisitionDevice() { UnmapBuffers(); };

    void StartAction(const DiffractionExperiment &experiment);
    void PrepareAction(const DiffractionExperiment &experiment);

    void WaitForActionComplete();
    void ActionAbort();

    void SendWorkRequest(uint32_t handle);
    void EnableLogging(Logger *logger);

    // Post measurement statistics - only guaranteed valid after WaitForActionComplete ends
    uint64_t GetBytesReceived() const;

    void SaveStatistics(const DiffractionExperiment &experiment, JFJochProtoBuf::AcquisitionDeviceStatistics &statistics) const;
    JFJochProtoBuf::FPGAStatus GetStatus() const;

    // Internal frame generator
    void SetCustomInternalGeneratorFrame(const std::vector<uint16_t> &v);
    const std::vector<uint16_t> &GetInternalGeneratorFrame() const;

    const int16_t *GetFrameBuffer(size_t frame_number, uint16_t module) const;
    void FrameBufferRelease(size_t frame_number, uint16_t module);
    const int16_t *GetErrorFrameBuffer() const;

    int16_t *GetDeviceBuffer(size_t handle);

    // Calibration
    virtual void InitializeCalibration(const DiffractionExperiment &experiment, const JFCalibration &calib);

    void LoadModuleGain(const JFModuleGainCalibration &gain_calibration, uint16_t module);

    uint64_t GetHead(uint8_t module) const;
    uint64_t GetSlowestHead() const;
    void WaitForFrame(size_t curr_frame, uint16_t module = UINT16_MAX) const;
    int64_t CalculateDelay(size_t curr_frame, uint16_t module = UINT16_MAX) const; // mutex acquired indirectly
    uint64_t GetBufferHandle(size_t frame, uint8_t module) const;
    bool IsFullModuleCollected(size_t frame, uint8_t module) const;
    ActionConfig ReadActionRegister();
    bool IsDone() const;

    std::string GetMACAddress() const;
};

inline uint64_t bit_concat(uint32_t high, uint32_t low) {
    return (uint64_t(high) << 32) | low;
}

#endif //JUNGFRAUJOCH_ACQUISITIONDEVICE_H
