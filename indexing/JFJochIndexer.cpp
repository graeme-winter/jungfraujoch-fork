// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include "JFJochIndexer.h"
#include "XgandalfWrapper.h"

JFJochIndexer::JFJochIndexer(ZMQContext &context, const JFJochProtoBuf::JFJochIndexerInput &input, Logger &in_logger,
                             int64_t nthreads) :
socket(context, ZMQSocketType::Sub), bin_size(input.bin_size()), logger(in_logger) {
    logger.Info("Starting ... ");
    socket.ReceiveWaterMark(250);
    socket.Connect(input.zmq_recv_pub_addr());
    socket.SubscribeAll();

    DiffractionExperiment experiment(input.jungfraujoch_settings());

    logger.Info("   ... subscribed to ZeroMQ: " + input.zmq_recv_pub_addr());
    for (int i = 0; i < nthreads; i++)
        processing_threads.emplace_back(std::async(std::launch::async, &JFJochIndexer::Run, this,
                                                   experiment));
    logger.Info("   ... started " + std::to_string(nthreads) + " indexing threads");

    image_stride = experiment.GetSpotFindingStride();
    if (image_stride <= 0)
        image_stride = 1;
    output.reserve(experiment.GetImageNumForPreview() / image_stride);
}

JFJochIndexer::~JFJochIndexer() {
    abort = 1;
}

JFJochProtoBuf::JFJochIndexerOutput JFJochIndexer::End() {
    JFJochProtoBuf::JFJochIndexerOutput ret;

    if (abort)
        return ret; // Already ended

    abort = 1;

    int64_t total_processing_time_ms = 0;
    for (auto &iter: processing_threads)
        total_processing_time_ms += iter.get();

    int64_t total_processed_images = output.size();

    if (total_processed_images > 0) {
        ret.set_ms_per_image(
                static_cast<double>(total_processing_time_ms) / static_cast<double>(total_processed_images));
        logger.Info("Images " + std::to_string(total_processed_images) + "   Performance: " + std::to_string(ret.ms_per_image()) + " ms/image");
    } else {
        ret.set_ms_per_image(std::nan(""));
        logger.Info("No images received");
    }

    int64_t indexed_images = 0;
    for (const auto &i: output) {
        if (i.indexed())
            indexed_images++;
    }
    ret.set_indexed_images(indexed_images);

    *ret.mutable_image_output() = {output.begin(), output.end()};
    return ret;
}

JFJochProtoBuf::IndexerStatus JFJochIndexer::GetStatus() {
    std::unique_lock<std::mutex> ul(output_mutex);
    JFJochProtoBuf::IndexerStatus ret;

    int64_t images_indexed = 0;
    for (const auto &i: output) {
        if (i.indexed())
            images_indexed++;
    }
    ret.set_images_analyzed(output.size());
    ret.set_images_indexed(images_indexed);

    int64_t bins = output.size() / bin_size;

    for (int i = 0; i < bins; i++) {
        int64_t indexed = 0;
        int64_t spots = 0;

        for (int j = 0; j < bin_size; j++) {
            spots += output[i * bin_size + j].spot_count();
            if (output[i * bin_size + j].indexed())
                indexed++;
        }

        ret.add_indexing_rate((indexed * 100)/ static_cast<double>(bin_size));
        ret.add_mean_spots(spots / static_cast<double>(bin_size));
    }

    return ret;
}

int64_t JFJochIndexer::Run(const DiffractionExperiment &in_settings) {
    XgandalfWrapper wrapper;
    int64_t total_time_ms = 0;

    std::string s;

    IndexingSettings settings;
    settings.algorithm = IndexingAlgorithm::Xgandalf_fast;
    settings.max_indexing_spots = 0;

    while (true) {
        if (socket.Receive(s, false) >= 0) {
            JFJochProtoBuf::SpotFinderImageOutput list;
            list.ParseFromString(s);

            std::vector<Coord> coord;
            coord.reserve(list.coord().size());
            for (const auto &i: list.coord())
                coord.emplace_back(i);

            settings.unit_cell = list.unit_cell();

            std::vector<UnitCell> indexing_result;
            auto start_time = std::chrono::system_clock::now();
            if (coord.size() >= MIN_SPOTS_TO_INDEX)
                indexing_result = wrapper.Run(settings, coord);
            auto end_time = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            total_time_ms += elapsed.count();

            {
                JFJochProtoBuf::IndexerImageOutput image_output;
                image_output.set_indexed(!indexing_result.empty());
                image_output.set_image_number(list.image_number());
                image_output.set_spot_count(list.coord_size());
                if (!indexing_result.empty())
                    *image_output.mutable_unit_cell() = indexing_result[0];
                AddIndexerImageOutput(image_output);
            }
        } else {
            if (abort) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    return total_time_ms;
}


void JFJochIndexer::AddIndexerImageOutput(const JFJochProtoBuf::IndexerImageOutput &image_output) {
    std::unique_lock<std::mutex> ul(output_mutex);

    int64_t location = image_output.image_number() / image_stride;
    if (location + 1 > output.size())
        output.resize(location+1);
    output[location] = image_output;
}