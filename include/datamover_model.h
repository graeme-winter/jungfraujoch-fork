// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_DATAMOVER_MODEL_H
#define JUNGFRAUJOCH_DATAMOVER_MODEL_H

#include "../fpga/hls/hls_jfjoch.h"
#include <thread>

enum class Direction {Input, Output};

template <int N> class Datamover {
    volatile int stop_signal;
    Direction dir;
    char *offset;
    hls::stream<axis_datamover_ctrl> control;
    hls::stream<ap_axiu<N,1,1,1> > data;
    std::thread datamover_thread;

    size_t bytes_transferred;

    void Move() {
        axis_datamover_ctrl command = control.read();

        uint64_t addr = command.data(31+64,32);
        uint32_t bytes = command.data(22,0);
        if (bytes % (N/8) != 0)
            throw std::runtime_error("AXI datamover transfer must be aligned to " + std::to_string(N/8) +  " bytes");
        if (addr % (N/8) != 0)
            throw std::runtime_error("AXI datamover addr must be aligned to " + std::to_string(N/8) +  " bytes");
        if (bytes == 0)
            throw std::runtime_error("AXI datamover transfer cannot be 0");

        uint32_t xfers = bytes / (N/8);
        auto host_mem = (ap_uint<N> *) (offset + addr);
        ap_axiu<N,1,1,1> data_packet;
        if (dir == Direction::Input) {
            for (uint64_t i = 0; i < xfers; i++) {
                data_packet.data = host_mem[i];
                for (int j = 0; j < N / 8; j++)
                    data_packet.keep[j] = 1;
                data_packet.last = ((i == xfers - 1) ? 1 : 0);
                data << data_packet;
                bytes_transferred += N/8;
            }
        } else {
            for (uint64_t i = 0; i < xfers; i++) {
                data >> data_packet;
                host_mem[i] = data_packet.data;
                for (int j = 0; j < N / 8; j++) {
                    if (data_packet.keep[j] != 1)
                        throw std::runtime_error("TKEEP set low for bit " + std::to_string(i));
                }
                if (data_packet.last != ((i == xfers - 1) ? 1 : 0))
                    throw std::runtime_error("TLAST packet flag set incorrectly for packet " + std::to_string(i) + " Expected xfers: " + std::to_string(xfers));
                bytes_transferred += N/8;
            }
        }
    }
public:
    void Run() {
        while (!stop_signal || !control.empty()) {
            while (!stop_signal && control.empty())
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            // All commands that arrive till stop signal is executed, are executed
            while (!control.empty()) {
                Move();
            }
        }
    };

    size_t Stop() noexcept { stop_signal = true; datamover_thread.join(); return bytes_transferred; }

    Datamover(Direction in_dir, char * in_offset = nullptr, uint64_t data_fifo_size = INT32_MAX)
                  : dir(in_dir), offset(in_offset), stop_signal(0), bytes_transferred(0), data(data_fifo_size) {
       datamover_thread = std::thread([this] {this->Run(); });
    };

    bool IsIdle() const {
        return control.empty();
    }

    hls::stream<axis_datamover_ctrl>& GetCtrlStream() { return control; }
    hls::stream<ap_axiu<N,1,1,1> >& GetDataStream() { return data; }
    ~Datamover() { Stop(); }
};
#endif //JUNGFRAUJOCH_DATAMOVER_MODEL_H
