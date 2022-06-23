// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SLSEXCEPTION_H
#define SLSEXCEPTION_H

#include <string>
#include <exception>
#include <cstdint>

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
SpotFinderError
};

class JFJochException : public std::exception {
    std::string description;
    std::string msg;
    JFJochExceptionCategory category;
    static std::string DecodeCategory(JFJochExceptionCategory category);
public:
    JFJochException(JFJochExceptionCategory val, const std::string &description, int optional = 0);
    const std::string &GetDescription() const;
    void SetDescription(const std::string& val);
    const char *what() const noexcept ;
};


#endif //SLSEXCEPTION_H
