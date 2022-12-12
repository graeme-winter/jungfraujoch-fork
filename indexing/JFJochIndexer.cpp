// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cmath>
#include <gemmi/symmetry.hpp>

#include "JFJochIndexer.h"
#include "XgandalfWrapper.h"

JFJochIndexer::JFJochIndexer(ZMQContext &context, const JFJochProtoBuf::IndexerInput &input, Logger &in_logger,
                             int64_t nthreads) :
socket(context, ZMQSocketType::Sub), bin_size(input.bin_size()), logger(in_logger) {
    logger.Info("Starting ... ");
    socket.ReceiveWaterMark(250);
    socket.Connect(input.zmq_recv_pub_addr());
    socket.SubscribeAll();

     logger.Info("   ... subscribed to ZeroMQ: " + input.zmq_recv_pub_addr());
    for (int i = 0; i < nthreads; i++)
        processing_threads.emplace_back(std::async(std::launch::async, &JFJochIndexer::Run, this, input));
    logger.Info("   ... started " + std::to_string(nthreads) + " indexing threads");
}

JFJochIndexer::~JFJochIndexer() {
    abort = 1;
}

JFJochProtoBuf::IndexerOutput JFJochIndexer::End() {
    JFJochProtoBuf::IndexerOutput ret;

    if (abort)
        return ret; // Already ended

    abort = 1;

    int64_t total_processing_time_ms = 0;
    for (auto &iter: processing_threads)
        total_processing_time_ms += iter.get();

    int64_t total_processed_images = output_analyzed;

    if (total_processed_images > 0) {
        ret.set_ms_per_image(
                static_cast<double>(total_processing_time_ms) / static_cast<double>(total_processed_images));
        logger.Info("Images " + std::to_string(total_processed_images) + "   Performance: " + std::to_string(ret.ms_per_image()) + " ms/image");
    } else {
        ret.set_ms_per_image(std::nan(""));
        logger.Info("No images received");
    }

    ret.set_indexed_images(output_indexed);

    *ret.mutable_image_output() = {output.begin(), output.end()};
    return ret;
}

JFJochProtoBuf::IndexerStatus JFJochIndexer::GetStatus() {
    std::unique_lock<std::mutex> ul(output_mutex);
    // locking, to ensure that analyzed and indexed are consistent

    JFJochProtoBuf::IndexerStatus ret;

    ret.set_images_analyzed(output_analyzed);
    ret.set_images_indexed(output_indexed);

    return ret;
}

JFJochProtoBuf::IndexerDataProcessingPlots JFJochIndexer::GetPlots() {
    // no need to lock, as indexed_result has built-in lock
    JFJochProtoBuf::IndexerDataProcessingPlots ret;
    indexed_result.GetPlot(*ret.mutable_indexing_rate(), bin_size);
    return ret;
}

int64_t JFJochIndexer::Run(const JFJochProtoBuf::IndexerInput &input) {
    XgandalfWrapper wrapper;
    int64_t total_time_ms = 0;

    std::string s;

    IndexingSettings settings;
    settings.max_indexing_spots = 0;

    if (input.has_unit_cell()) {
        settings.has_unit_cell = true;
        settings.unit_cell = input.unit_cell();
        settings.centering = GetCentering(input.space_group_number());
    } else
        settings.has_unit_cell = false;
    wrapper.Setup(settings);

    while (true) {
        if (socket.Receive(s, false) >= 0) {
            JFJochProtoBuf::SpotFinderImageOutput list;
            list.ParseFromString(s);

            std::vector<Coord> coord;
            coord.reserve(list.coord().size());
            for (const auto &i: list.coord())
                coord.emplace_back(i);

            if (coord.size() >= MIN_SPOTS_TO_INDEX) {
                auto start_time = std::chrono::system_clock::now();

                auto indexing_result = wrapper.Run(coord);
                auto end_time = std::chrono::system_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

                total_time_ms += elapsed.count();

                JFJochProtoBuf::IndexerImageOutput image_output;
                image_output.set_indexed(!indexing_result.empty());
                image_output.set_image_number(list.image_number());
                image_output.set_spot_count(list.coord_size());
                if (!indexing_result.empty())
                    *image_output.mutable_unit_cell() = JFJochProtoBuf::UnitCell(indexing_result[0]);
                AddIndexerImageOutput(image_output);
                indexed_result.AddElement(list.image_number(), 1);
            } else {
                JFJochProtoBuf::IndexerImageOutput image_output;
                image_output.set_indexed(false);
                image_output.set_image_number(list.image_number());
                image_output.set_spot_count(list.coord_size());
                AddIndexerImageOutput(image_output);
                indexed_result.AddElement(list.image_number(), 0);
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

    output_analyzed++;
    if (image_output.indexed())
        output_indexed++;

    output.push_back(image_output);
}

char JFJochIndexer::GetCentering(int64_t space_group) {
    if (space_group == 0)
        return 'P';
    try {
        auto sg = gemmi::get_spacegroup_by_number(static_cast<int>(space_group));
        return sg.centring_type();
    } catch (...) {
        return 'P';
    }
}