// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUNGFRAUJOCH_ZMQHDF5FILESTATUSPUBLISHER_H
#define JUNGFRAUJOCH_ZMQHDF5FILESTATUSPUBLISHER_H

#include <nlohmann/json.hpp>

#include "../common/ZMQWrappers.h"
#include "../common/DiffractionExperiment.h"

class ZMQHDF5FileStatusPublisher {
    nlohmann::json json_template;
    ZMQSocket socket;
public:
    ZMQHDF5FileStatusPublisher(const DiffractionExperiment &experiment, ZMQContext &zmq_context,
                               const std::string& zmq_addr);
    void Publish(const std::string &filename, size_t max_image_number);
};


#endif //JUNGFRAUJOCH_ZMQHDF5FILESTATUSPUBLISHER_H
