// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AcquisitionOfflineCounters.h"
#include <bitset>

void AcquisitionOfflineCounters::Reset(const DiffractionExperiment &x, uint16_t data_stream) {
    std::unique_lock<std::mutex> ul(m);

    auto max_entries = x.GetFrameNum() * x.GetModulesNum(data_stream);
    completion.clear();
    completion.reserve(max_entries);
}

void AcquisitionOfflineCounters::Add(Completion c) {
    std::unique_lock<std::mutex> ul(m);
    completion.push_back(c);
}

void AcquisitionOfflineCounters::FillStatistics(const DiffractionExperiment &x, uint16_t data_stream,
                                                JFJochProtoBuf::AcquisitionDeviceStatistics& statistics) const {
    std::unique_lock<std::mutex> ul(m);

    if (x.GetFrameNum() == 0)
        return;

    auto nmodules = x.GetModulesNum(data_stream);
    auto max_entries = x.GetFrameNum() * x.GetModulesNum(data_stream);

    int masks_per_module;
    bool packet_8kB;
    int64_t expected_packets_per_module;
    if (x.GetDetectorType() == JFJochProtoBuf::JUNGFRAU) {
        masks_per_module = 2;
        packet_8kB = true;
        expected_packets_per_module = 128;
    } else {
        masks_per_module = 4;
        packet_8kB = false;
        expected_packets_per_module = 256;
    }

    std::vector<uint32_t> timestamp(max_entries, UINT32_MAX);
    std::vector<uint32_t> debug(max_entries, UINT32_MAX);
    std::vector<uint64_t> bunchid(max_entries, UINT64_MAX);
    std::vector<uint64_t> packet_mask(masks_per_module * max_entries, 0);
    std::vector<uint64_t> packet_count(max_entries, 0);
    uint64_t total_packets = 0;

    for (const auto &c: completion) {
        size_t i = c.frame_number * nmodules + c.module;
        timestamp[i]    = c.timestamp;
        debug[i]        = c.debug;
        bunchid[i]      = c.bunchid;
        packet_count[i] = (packet_8kB ? (c.packet_count / 2) : c.packet_count);
        total_packets += packet_count[i];

        for (int j = 0; j < masks_per_module; j++)
            packet_mask[i * masks_per_module + j] = c.packet_mask[j];
    }

    size_t expected_images = x.GetImageNum();
    if ((x.GetDetectorMode() == DetectorMode::PedestalG0) ||
        (x.GetDetectorMode() == DetectorMode::PedestalG1) ||
        (x.GetDetectorMode() == DetectorMode::PedestalG2))
        expected_images = x.GetFrameNum();

    uint32_t expected_packets = max_entries * expected_packets_per_module;
    statistics.set_nmodules(x.GetModulesNum(data_stream));
    statistics.set_mask_entries_per_module(masks_per_module);
    statistics.set_packets_expected_per_module(expected_packets_per_module);
    statistics.set_packets_expected(expected_packets);

    if ((expected_images == 0) || (total_packets == expected_packets))
        statistics.set_efficiency(1.0);
    else
        statistics.set_efficiency(static_cast<double>(total_packets) / static_cast<double>(expected_packets));

    *statistics.mutable_bunchid() = {bunchid.begin(), bunchid.end()};
    *statistics.mutable_detector_debug() = {debug.begin(), debug.end()};
    *statistics.mutable_timestamp() = {timestamp.begin(), timestamp.end()};
    *statistics.mutable_packet_mask() = {packet_mask.begin(), packet_mask.end()};
    *statistics.mutable_packets_received_per_module() = {packet_count.begin(), packet_count.end()};
    statistics.set_good_packets(total_packets);
}
