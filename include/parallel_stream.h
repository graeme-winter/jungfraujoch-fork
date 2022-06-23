// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_PARALLEL_STREAM_H
#define JUNGFRAUJOCH_PARALLEL_STREAM_H

#include <cstddef>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace hls {
    template<class T> class stream {
    private:
        mutable std::mutex m;
        std::condition_variable c_empty;
        std::condition_variable c_full;
        std::queue<T> data;
        std::chrono::seconds timeout;
        size_t max_elements;

        void internal_read(T& head) {
            head = data.front();
            data.pop();
            c_full.notify_one();
        }
        void internal_write(const T& tail) {
            data.push(tail);
            c_empty.notify_one();
        }
    public:
        explicit stream(size_t nelem = UINT64_MAX, std::chrono::seconds blocking_timeout = std::chrono::minutes(15))
        : max_elements(nelem), data(), timeout(blocking_timeout) {};

        void write(const T& tail) {
            std::unique_lock<std::mutex> ul(m);
            while (data.size() >= max_elements) {
                if (c_full.wait_for(ul, timeout)
                    == std::cv_status::timeout)
                    throw std::runtime_error("Likely deadlock");
            }
            internal_write(tail);
        }

        bool write_nb(const T& tail) {
            std::unique_lock<std::mutex> ul(m);
            if (data.size() >= max_elements) return false;
            internal_write(tail);
            return true;
        }

        bool read_nb(T& head) {
            std::unique_lock<std::mutex> ul(m);
            if (data.empty()) return false;
            internal_read(head);
            return true;
        }

        void read(T& head) {
            std::unique_lock<std::mutex> ul(m);
            while (data.empty()) {
                if (c_empty.wait_for(ul, timeout)
                    == std::cv_status::timeout)
                    throw std::runtime_error("Likely deadlock");
            }
            internal_read(head);
        }

        bool full() const {
            std::unique_lock<std::mutex> ul(m);
            return (data.size() >= max_elements);
        }

        bool empty() const {
            std::unique_lock<std::mutex> ul(m);
            return data.empty();
        };

        size_t size() const { return data.size(); };
        void operator >> (T& rval) { read(rval); }
        void operator << (const T& rval) { write(rval); }
        T read() { T ret; read(ret); return ret; }
    };
}

#endif //JUNGFRAUJOCH_PARALLEL_STREAM_H
