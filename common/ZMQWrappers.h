// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQWRAPPERS_H
#define JUNGFRAUJOCH_ZMQWRAPPERS_H

#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <zmq.h>
#include "JFJochException.h"

class ZMQContext {
    void *context;
public:
    ZMQContext();
    ZMQContext& NumThreads(int32_t threads);
    ~ZMQContext();
    void *GetContext() const;
};

enum class ZMQSocketType : int {Push = ZMQ_PUSH, Pull = ZMQ_PULL, Req = ZMQ_REQ, Rep = ZMQ_REP,
                                Pub = ZMQ_PUB, Sub = ZMQ_SUB};

class ZMQSocket {
    std::mutex m;
    ZMQSocketType socket_type;
    void *socket;
    void SetSocketOption(int32_t option_name, int32_t value);
public:
    ZMQSocket(ZMQSocket &socket) = delete;
    const ZMQSocket& operator=(ZMQSocket &socket) = delete;
    ZMQSocket(ZMQContext &context, ZMQSocketType socket_type);
    ~ZMQSocket();
    void Connect(const std::string& addr);
    void Disconnect(const std::string& addr);
    void Bind(const std::string& addr);
    ZMQSocket &NoReceiveTimeout();
    ZMQSocket &ReceiveTimeout(std::chrono::milliseconds input);
    ZMQSocket &Subscribe(const std::string &topic);
    ZMQSocket &SubscribeAll();
    ZMQSocket &NoLinger();
    ZMQSocket &Conflate(bool input);
    ZMQSocket &SendBufferSize(int32_t bytes);
    ZMQSocket &ReceiverBufferSize(int32_t bytes);

    int64_t Receive(bool blocking = true);
    int64_t Receive(std::string &j, bool blocking = true);
    template <class T> int64_t Receive(std::vector<T> &vector, bool blocking = true, bool resize = true) {
        std::unique_lock<std::mutex> ul(m);
        zmq_msg_t zmq_msg;
        zmq_msg_init(&zmq_msg);
        int64_t msg_size = zmq_msg_recv(&zmq_msg, socket, blocking ? 0 : ZMQ_DONTWAIT);
        if (msg_size < 0) {
            if (errno == EAGAIN)
                return -1;
            if (errno == EINTR) // Timeout ?
                return -1;
            else
                throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_msg_recv failed "
                                                                       + std::string(strerror(errno)));
        } else if (msg_size == 0) {
            zmq_msg_close (&zmq_msg);
            return 0;
        } else if (msg_size > vector.size() * sizeof(T)) {
            if (resize)
                vector.resize(msg_size / sizeof(T) + ((msg_size % sizeof(T) != 0) ? 1 : 0));
            else {
                zmq_msg_close (&zmq_msg);
                throw JFJochException(JFJochExceptionCategory::ZeroMQ, "Receive buffer too small");
            }
        }

        memcpy(vector.data(), zmq_msg_data(&zmq_msg), msg_size);
        zmq_msg_close (&zmq_msg);
        return msg_size;
    }

    void Send();
    void SendTwoPartAtomic(const std::string &metadata, void *image, size_t image_size);
    void Send(const void *buf, size_t buf_size, bool blocking = true, bool multipart = false);
    template <class T> void Send(const std::vector<T> &buf) {
        Send(buf.data(), buf.size() * sizeof(T));
    }
    void Send(const int32_t &value);
    void Send(const std::string &s, bool blocking = true, bool multipart = false);
    void Send(zmq_msg_t *msg);
    ZMQSocket &SendWaterMark(int32_t msgs);
    ZMQSocket &ReceiveWaterMark(int32_t msgs);
};


#endif //JUNGFRAUJOCH_ZMQWRAPPERS_H
