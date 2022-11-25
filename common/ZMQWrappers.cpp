// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ZMQWrappers.h"
#include <cerrno>

ZMQContext::ZMQContext() {
    context = zmq_ctx_new();
}

ZMQContext &ZMQContext::NumThreads(int32_t threads) {
    if (zmq_ctx_set(context, ZMQ_IO_THREADS, threads) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ,
                              "Cannot set number of I/O threads");
    return *this;
}

ZMQContext::~ZMQContext() {
    zmq_ctx_destroy(context);
}

void *ZMQContext::GetContext() const {
    return context;
}

ZMQSocket::ZMQSocket(ZMQContext &context, ZMQSocketType in_socket_type) : socket_type(in_socket_type) {
    socket = zmq_socket(context.GetContext(), static_cast<int>(socket_type));

    if (socket == nullptr)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_socket failed");
}

void ZMQSocket::Bind(const std::string &addr) {
    if (zmq_bind(socket, addr.c_str()) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_bind failed");
}

void ZMQSocket::Connect(const std::string &addr) {
    if (zmq_connect(socket, addr.c_str()) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_connect failed");
}

void ZMQSocket::Disconnect(const std::string &addr) {
    if (zmq_disconnect(socket, addr.c_str()) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_disconnect failed");
}

void ZMQSocket::Send() {
    std::unique_lock<std::mutex> ul(m);
    if (zmq_send(socket, nullptr, 0, 0) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_send() failed: " + std::string(std::strerror(errno)));
}

void ZMQSocket::Send(const void *buf, size_t buf_size, bool blocking, bool multipart) {
    std::unique_lock<std::mutex> ul(m);
    if (zmq_send(socket, buf, buf_size, (blocking ? 0 : ZMQ_DONTWAIT) | (multipart ? ZMQ_SNDMORE : 0)) != buf_size) {
        if (errno != EAGAIN)
            throw JFJochException(JFJochExceptionCategory::ZeroMQ,
                              "zmq_send(buf) failed: " + std::string(std::strerror(errno)));
    }
}


void ZMQSocket::SendTwoPartAtomic(const std::string &metadata, void *image, size_t image_size) {
    std::unique_lock<std::mutex> ul(m);
    if (zmq_send(socket, metadata.c_str(), metadata.size(), ZMQ_SNDMORE) != metadata.size())
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_send(part1) failed: "
        + std::string(std::strerror(errno)));
    if (zmq_send(socket, image, image_size, 0) != image_size)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_send(part2) failed: "
                                                               + std::string(std::strerror(errno)));
}

void ZMQSocket::Send(const std::string &s, bool blocking, bool multipart) {
    Send((void *) s.c_str(), s.size(), blocking, multipart);
}

void ZMQSocket::Send(const int32_t &value) {
    std::unique_lock<std::mutex> ul(m);
    if (zmq_send(socket, &value, sizeof(int32_t), 0) != sizeof(int32_t))
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_send(int) failed: " + std::string(std::strerror(errno)));
}

void ZMQSocket::Send(zmq_msg_t *msg) {
    std::unique_lock<std::mutex> ul(m);
    if (zmq_msg_send(msg, socket, 0) < 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "zmq_msg_send failed");
}

int64_t ZMQSocket::Receive(bool blocking) {
    std::vector<uint8_t> msg;
    return Receive(msg, blocking, true);
}

int64_t ZMQSocket::Receive(std::string &s, bool blocking) {
    std::vector<uint8_t> v;
    int64_t rc = Receive(v, blocking, true);
    if (rc > 0)
        s = std::string(v.begin(), v.end());
    return rc;
}

void ZMQSocket::SetSocketOption(int32_t option_name, int32_t value) {
    if (zmq_setsockopt(socket, option_name, &value, sizeof(value)) != 0)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "Cannot set socket option");

}

ZMQSocket &ZMQSocket::ReceiveTimeout(std::chrono::milliseconds input) {
    SetSocketOption(ZMQ_RCVTIMEO, input.count());
    return *this;
}

ZMQSocket &ZMQSocket::NoReceiveTimeout() {
    SetSocketOption(ZMQ_RCVTIMEO, -1);
    return *this;
}

ZMQSocket &ZMQSocket::ReceiverBufferSize(int32_t bytes) {
    SetSocketOption(ZMQ_RCVBUF, bytes);
    return *this;
}

ZMQSocket &ZMQSocket::SendBufferSize(int32_t bytes) {
    SetSocketOption(ZMQ_SNDBUF, bytes);
    return *this;
}

ZMQSocket &ZMQSocket::SubscribeAll() {
    if (socket_type != ZMQSocketType::Sub)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "subscribe only possible for Sub socket");
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, nullptr, 0);
    return *this;
}

ZMQSocket &ZMQSocket::Subscribe(const std::string &topic) {
    if (socket_type != ZMQSocketType::Sub)
        throw JFJochException(JFJochExceptionCategory::ZeroMQ, "subscribe only possible for Sub socket");
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
    return *this;
}

ZMQSocket::~ZMQSocket() {
    zmq_close(socket);
}

ZMQSocket& ZMQSocket::ReceiveWaterMark(int32_t msgs) {
    SetSocketOption(ZMQ_RCVHWM, msgs);
    return *this;
}

ZMQSocket& ZMQSocket::SendWaterMark(int32_t msgs) {
    SetSocketOption(ZMQ_SNDHWM, msgs);
    return *this;
}

ZMQSocket &ZMQSocket::NoLinger() {
    SetSocketOption(ZMQ_LINGER, 0);
    return *this;
}

ZMQSocket &ZMQSocket::Conflate(bool input) {
    SetSocketOption(ZMQ_CONFLATE, input ? 1 : 0);
    return *this;
}