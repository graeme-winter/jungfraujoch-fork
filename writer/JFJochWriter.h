// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHWRITER_H
#define JUNGFRAUJOCH_JFJOCHWRITER_H

#include <future>
#include "jfjoch.grpc.pb.h"
#include "ZMQImagePuller.h"
#include "HDF5Writer.h"
#include "../common/Logger.h"

class JFJochWriter {
    ZMQImagePuller image_puller;
    Logger &logger;
    std::unique_ptr<HDF5Writer> data_file_set;
    std::future<JFJochProtoBuf::WriterOutput> measurement;
    JFJochProtoBuf::WriterOutput MeasurementThread();
public:
    JFJochWriter(const JFJochProtoBuf::WriterInput &request, ZMQContext& context, Logger &logger);
    JFJochProtoBuf::WriterOutput Stop();
    void Abort();
    ~JFJochWriter();
};


#endif //JUNGFRAUJOCH_JFJOCHWRITER_H
