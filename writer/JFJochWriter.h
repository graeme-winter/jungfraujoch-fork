// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHWRITER_H
#define JUNGFRAUJOCH_JFJOCHWRITER_H

#include <future>
#include "jfjoch.grpc.pb.h"
#include "../common/ZMQImagePuller.h"
#include "HDF5Writer.h"
#include "../common/Logger.h"
#include "../common/ThreadSafeFIFO.h"

class JFJochWriter {
    DiffractionExperiment experiment;
    ZMQImagePuller image_puller;
    Logger &logger;
    std::unique_ptr<HDF5Writer> data_file_set;
    void SetGroupNumber(const DiffractionExperiment &experiment);
    std::future<void> measurement;
    std::future<void> writing_future;
    std::future<void> pulling_future;
    void PullingThread();
    void WritingThread();
    void MeasurementThread();
    ThreadSafeFIFO<std::shared_ptr<std::vector<uint8_t> > > image_fifo{1000};
public:
    JFJochWriter(DiffractionExperiment  experiment, ZMQContext& context, const std::string& zmq_addr,
                 Logger &logger);
    void Stop();
    void WaitTillDone();
    void Abort();
    ~JFJochWriter();
};


#endif //JUNGFRAUJOCH_JFJOCHWRITER_H
