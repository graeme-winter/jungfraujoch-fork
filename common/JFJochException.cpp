// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochException.h"

JFJochException::JFJochException(JFJochExceptionCategory in_val, const std::string &in_description, int in_optional) :
description(in_description), category(in_val) {
    msg = DecodeCategory(category) + " (" + description + ")";
}

const char * JFJochException::what() const noexcept {

    return msg.c_str();
}

const std::string &JFJochException::GetDescription() const {
    return description;
}

void JFJochException::SetDescription(const std::string &val) {
    description = val;
    msg = DecodeCategory(category) + " (" + description + ")";
}

std::string JFJochException::DecodeCategory(JFJochExceptionCategory category) {
    switch (category) {
        case JFJochExceptionCategory::JSON:
            return "JSON parsing or analysis error";
        case JFJochExceptionCategory::InputParameterAboveMax:
            return "Input parameter above max";
        case JFJochExceptionCategory::InputParameterBelowMin:
            return "Input parameter below min";
        case JFJochExceptionCategory::InputParameterInvalid:
            return "Input parameter invalid";
        case JFJochExceptionCategory::WrongNumber:
            return "Wrong number";
        case JFJochExceptionCategory::WrongUnit:
            return "Wrong unit";
        case JFJochExceptionCategory::ArrayOutOfBounds:
            return "Array out of bounds";
        case JFJochExceptionCategory::HDF5:
            return "HDF5 error";
        case JFJochExceptionCategory::IBVerbs:
            return "Infiniband error";
        case JFJochExceptionCategory::RESTCommandUnknown:
            return "REST command unknown";
        case JFJochExceptionCategory::WrongDAQState:
            return "DAQ state error";
        case JFJochExceptionCategory::Detector:
            return "Detector error";
        case JFJochExceptionCategory::MemAllocFailed:
            return "Memory error";
        case JFJochExceptionCategory::OpenCAPIError:
            return "OpenCAPI error";
        case JFJochExceptionCategory::GPUCUDAError:
            return "CUDA (GPU) error";
        case JFJochExceptionCategory::GainFileOpenError:
            return "Gain file open error";
        case JFJochExceptionCategory::MockFileOpenError:
            return "Mock file open error";
        case JFJochExceptionCategory::Compression:
            return "Compression error";
        case JFJochExceptionCategory::SharedMemory:
            return "Shared memory error";
        case JFJochExceptionCategory::ZeroMQ:
            return "ZeroMQ error";
        case JFJochExceptionCategory::ServiceUndefined:
            return "Service undefined error";
        case JFJochExceptionCategory::TriggerError:
            return "Trigger error";
        case JFJochExceptionCategory::FileWriteError:
            return "File writer error";
        case JFJochExceptionCategory::ZSTDCompressionError:
            return "Zstd compressor error";
        case JFJochExceptionCategory::LogstashConnectionFailure:
            return "Logstash connection error";
        case JFJochExceptionCategory::TIFFGeneratorError:
            return "TIFF writer error";
        case JFJochExceptionCategory::gRPCError:
            return "Remote node (gRPC) error";
        case JFJochExceptionCategory::PreviewError:
            return "Preview error";
        case JFJochExceptionCategory::HardwareParityError:
            return "Parity error (HW)";
        case JFJochExceptionCategory::SpotFinderError:
            return "Spot finder";
        case JFJochExceptionCategory::PCIeError:
            return "PCIe driver error";
        default:
            return "";
    }
}