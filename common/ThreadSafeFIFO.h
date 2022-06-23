// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_THREADSAFEFIFO_H
#define JUNGFRAUJOCH_THREADSAFEFIFO_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <set>

template <class T> class ThreadSafeFIFO {
    std::queue<T> queue;
    std::condition_variable c_empty, c_full;
    std::mutex m;
    const size_t max_size;
public:
    ThreadSafeFIFO(size_t in_max_size = UINT32_MAX) : max_size(in_max_size) {}
    void Clear() {
        std::unique_lock<std::mutex> ul(m);
        queue = {};
    }

    bool Put(T val) {
        std::unique_lock<std::mutex> ul(m);
        if (queue.size() < max_size) {
            queue.push(val);
            c_empty.notify_one();
            return true;
        } else
            return false;
    };

    void PutBlocking(T val) {
        std::unique_lock<std::mutex> ul(m);
        c_full.wait(ul, [&]{return queue.size() < max_size;});
        queue.push(val);
        c_empty.notify_one();
    };

    int Get(T &val) {
        std::unique_lock<std::mutex> ul(m);

        if (queue.empty())
            return 0;
        else {
            val = queue.front();
            queue.pop();
            c_full.notify_one();
            return 1;
        }
    }

    T GetBlocking() {
        std::unique_lock<std::mutex> ul(m);
        c_empty.wait(ul, [&]{return !queue.empty();});
        T tmp = queue.front();
        queue.pop();
        c_full.notify_one();
        return tmp;
    };

    size_t Size() const {
        return queue.size();
    }
};

template <class T> class ThreadSafeSet {
    std::set<T> set;
    std::condition_variable c;
    std::mutex m;
public:
    void Clear() {
        std::unique_lock<std::mutex> ul(m);
        set = {};
    }
    void Put(T val) {
        std::unique_lock<std::mutex> ul(m);
        set.insert(val);
        c.notify_one();
    };

    int Get(T &val) {
        std::unique_lock<std::mutex> ul(m);

        if (set.empty()) return 0;
        else {
            auto iter = set.begin();
            val = *iter;
            set.erase(iter);
            return 1;
        }
    }

    T GetBlocking() {
        std::unique_lock<std::mutex> ul(m);
        c.wait(ul, [&]{return !set.empty();});
        auto iter = set.begin();
        T tmp = *iter;
        set.erase(iter);
        return tmp;
    };

    size_t Size() const {
        return set.size();
    }
};

#endif //JUNGFRAUJOCH_THREADSAFEFIFO_H
