// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_STATUSVECTOR_H
#define JUNGFRAUJOCH_STATUSVECTOR_H

#include <vector>
#include <map>
#include <mutex>

#include <jfjoch.pb.h>

#include "JFJochException.h"


template <class T> class StatusVector {
    mutable std::mutex m;
    std::map<uint32_t, T > content;
    uint32_t max_id = 0;
public:
    void AddElement(uint32_t id, T val) {
        std::unique_lock<std::mutex> ul(m);

        content[id] = val;
        if (id > max_id)
            max_id = id;
    }

    [[nodiscard]] std::vector<float> GetStatus(int32_t bin_size) const {
        std::unique_lock<std::mutex> ul(m);

        if (bin_size <= 0)
            throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                                  "Bin number must be greater than zero");

        std::vector<float> ret;
        if (!content.empty()) {
            for (int bin = 0; bin < max_id / bin_size + ((max_id % bin_size > 0) ? 1 : 0); bin++) {
                double sum = 0;
                int64_t count = 0;

                auto it_start = content.lower_bound(bin * bin_size);
                auto it_end = content.upper_bound((bin + 1) * bin_size);
                if ((it_start != content.end()) && (it_start->first < (bin + 1) * bin_size)) {
                    for (auto it = it_start; it != it_end; it++) {
                        sum += it->second;
                        count++;
                    }
                }
                if (count > 0)
                    ret.push_back(static_cast<float>(sum / static_cast<double>(count)));
                else
                    ret.push_back(0.0);
            }
        }
        return ret;
    }

    void GetPlot(JFJochProtoBuf::Plot& plot, int32_t bin_size) const {
        // GetStatus has mutex, no need to lock again
        auto status = GetStatus(bin_size);
        if (status.size() == 1) {
            plot.add_x(max_id / 2.0);
            plot.add_y(status[0]);
        } else if (!status.empty()) {
            *plot.mutable_y() = {status.begin(), status.end()};

            for (int i = 0; i < status.size(); i++)
                plot.add_x(bin_size * (i + 0.5));
        }
    }
};


#endif //JUNGFRAUJOCH_STATUSVECTOR_H
