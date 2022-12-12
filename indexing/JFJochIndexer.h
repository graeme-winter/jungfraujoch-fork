// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_JFJOCHINDEXER_H
#define JUNGFRAUJOCH_JFJOCHINDEXER_H

#include <future>

#include "../common/ZMQWrappers.h"
#include "../common/Logger.h"
#include "../common/StatusVector.h"

#define MIN_SPOTS_TO_INDEX (8)

class JFJochIndexer {
    volatile int abort = 0;
    ZMQSocket socket;
    const int64_t bin_size;

    Logger &logger;

    std::vector<JFJochProtoBuf::IndexerImageOutput> output;
    int64_t output_indexed = 0;
    int64_t output_analyzed = 0;
    std::mutex output_mutex;

    std::vector<std::future<int64_t>> processing_threads;
    int64_t Run(const JFJochProtoBuf::IndexerInput &input);
    StatusVector<uint8_t> indexed_result;
    void AddIndexerImageOutput(const JFJochProtoBuf::IndexerImageOutput &output);
public:
    static char GetCentering(int64_t space_group);
    JFJochIndexer(ZMQContext &context, const JFJochProtoBuf::IndexerInput &input, Logger &logger, int64_t nthreads);
    JFJochProtoBuf::IndexerOutput End();
    JFJochProtoBuf::IndexerStatus GetStatus();
    JFJochProtoBuf::IndexerDataProcessingPlots GetPlots();
    ~JFJochIndexer();
};


#endif //JUNGFRAUJOCH_JFJOCHINDEXER_H
