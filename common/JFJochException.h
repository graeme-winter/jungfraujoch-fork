// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SLSEXCEPTION_H
#define SLSEXCEPTION_H

#include <string>
#include <exception>

enum class JFJochExceptionCategory {
JSON,
InputParameterAboveMax,
InputParameterBelowMin,
InputParameterInvalid,
WrongNumber,
WrongUnit,
ArrayOutOfBounds,
HDF5,
IBVerbs,
RESTCommandUnknown,
WrongDAQState,
Detector,
MemAllocFailed,
OpenCAPIError,
GPUCUDAError,
GainFileOpenError,
MockFileOpenError,
Compression,
SharedMemory,
ZeroMQ,
ServiceUndefined,
TriggerError,
FileWriteError,
ZSTDCompressionError,
LogstashConnectionFailure,
TIFFGeneratorError,
gRPCError,
PreviewError,
HardwareParityError,
SpotFinderError,
PCIeError,
CBORError
};

class JFJochException : public std::exception {
    std::string msg;
    JFJochExceptionCategory category;
    static std::string DecodeCategory(JFJochExceptionCategory category) {
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
            case JFJochExceptionCategory::CBORError:
                return "CBOR serialization error";
            default:
                return "";
        }
    }
public:
    JFJochException() = default;

    JFJochException(JFJochExceptionCategory in_val, const std::string &description, int optional = 0) noexcept :
            category(in_val) {
        try {
            msg = DecodeCategory(category) + " (" + description + ")";
        } catch (...) {}
    }

    JFJochException(const JFJochException &e) noexcept {
        try {
            category = e.category;
            msg = e.msg;
        } catch (...) {}
    }

    [[nodiscard]] const char *what() const noexcept override {
            return msg.c_str();
    }
};

#endif //SLSEXCEPTION_H
