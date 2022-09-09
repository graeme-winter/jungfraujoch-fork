// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PedestalCalcGPU.h"
#include "JFJochException.h"
#include "PedestalCalcCPU.h"

struct CudaStreamWrapper {
    cudaStream_t v;
};

inline void cuda_err(cudaError_t val) {
    if (val != cudaSuccess)
        throw JFJochException(JFJochExceptionCategory::GPUCUDAError, cudaGetErrorString(val));
}

PedestalCalcGPU::PedestalCalcGPU(const DiffractionExperiment & experiment, int64_t in_lines, int32_t gpu_device) :
        PedestalCalc(experiment, in_lines) {

    int device_count;
    cuda_err(cudaGetDeviceCount(&device_count));

    if (device_count == 0)
        throw JFJochException(JFJochExceptionCategory::GPUCUDAError, "No CUDA devices found");

    if (gpu_device < 0)
        gpu_device = threadid++;

    if (device_count > 1)
        cuda_err(cudaSetDevice(gpu_device % device_count));

    int deviceId;
    cuda_err(cudaGetDevice(&deviceId));
    cudaDeviceGetAttribute(&numberOfSMs, cudaDevAttrMultiProcessorCount, deviceId);

    cudastream = new(CudaStreamWrapper);
    cuda_err(cudaStreamCreate(&cudastream->v));

    cuda_err(cudaMallocManaged(&currPedestal, lines * RAW_MODULE_COLS * sizeof(double)));
    cuda_err(cudaMallocManaged(&currPedestalSquare, lines * RAW_MODULE_COLS * sizeof(double)));
    cuda_err(cudaMallocManaged(&wrongCount, lines * RAW_MODULE_COLS * sizeof(uint32_t)));
    cuda_err(cudaMalloc(&gpu_image, lines * RAW_MODULE_COLS * sizeof(uint16_t)));

    for (int i = 0; i < lines * RAW_MODULE_COLS; i++) {
        currPedestal[i] = 0;
        currPedestalSquare[i] = 0;
        wrongCount[i] = 0;
    }
}

PedestalCalcGPU::~PedestalCalcGPU() {

    cudaStreamDestroy(cudastream->v);
    delete(cudastream);

    cudaFree(currPedestal);
    cudaFree(currPedestalSquare);
    cudaFree(wrongCount);
    cudaFree(gpu_image);
}

template <unsigned int GAIN_BIT> __global__
void analyze_pixel_add(const uint16_t *raw_image, double *currPedestal, double *currPedestalSquare, uint32_t *wrongCount,
                  uint32_t lines) {

    uint32_t pixel = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t line = pixel / RAW_MODULE_COLS;

    if (line < lines) {
        float adc = raw_image[pixel] & 0x3FFF;
        uint16_t gain = raw_image[pixel] & 0xc000;

        if (gain != GAIN_BIT)
            wrongCount[pixel]++;

        currPedestal[pixel] += adc;
        currPedestalSquare[pixel] += adc * adc;

    }
}

template <unsigned int GAIN_BIT, unsigned int WINDOW_SIZE> __global__
void analyze_pixel_avg(const uint16_t *raw_image, double *currPedestal, double *currPedestalSquare, uint32_t *wrongCount,
                       uint32_t lines) {

    uint32_t pixel = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t line = pixel / RAW_MODULE_COLS;

    if (line < lines) {
        float adc = (raw_image[pixel] & 0x3FFF);
        uint16_t gain = raw_image[pixel] & 0xc000;

        if (gain != GAIN_BIT)
            wrongCount[pixel]++;
        else {
            // Don't include wrong gains into moving average
            currPedestal[pixel] += adc - currPedestal[pixel] / WINDOW_SIZE;
            currPedestalSquare[pixel] += adc * adc - currPedestalSquare[pixel] / WINDOW_SIZE;
        }
    }
}

void PedestalCalcGPU::AnalyzeImage(const uint16_t *raw_image) {
    cuda_err(cudaMemcpy(gpu_image, raw_image, lines * RAW_MODULE_COLS * sizeof(uint16_t), cudaMemcpyHostToDevice));
    size_t nThreads = 256;
    size_t nBlocks  = ((lines * RAW_MODULE_COLS) / (nThreads * numberOfSMs) + 1) * numberOfSMs;

    switch (gain_level) {
        case 0:
            if (image_number < window_size)
                analyze_pixel_add<0><<<nBlocks, nThreads, 0, cudastream->v>>>
                    (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            else
                analyze_pixel_avg<0, PEDESTAL_WINDOW_SIZE><<<nBlocks, nThreads, 0, cudastream->v>>>
                        (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            break;
        case 1:
            if (image_number < window_size)
                analyze_pixel_add<0x4000><<<nBlocks, nThreads, 0, cudastream->v>>>
                        (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            else
                analyze_pixel_avg<0x4000, PEDESTAL_WINDOW_SIZE><<<nBlocks, nThreads, 0, cudastream->v>>>
                        (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            break;
        case 2:
            if (image_number < window_size)
                analyze_pixel_add<0xc000><<<nBlocks, nThreads, 0, cudastream->v>>>
                        (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            else
                analyze_pixel_avg<0xc000, PEDESTAL_WINDOW_SIZE><<<nBlocks, nThreads, 0, cudastream->v>>>
                        (gpu_image, currPedestal, currPedestalSquare, wrongCount, lines);
            break;
        default:
            break;
    }
    cuda_err(cudaStreamSynchronize(cudastream->v));
    image_number++;
}

std::atomic<uint16_t> PedestalCalcGPU::threadid{0};

bool PedestalCalcGPU::GPUPresent() {
    int device_count;
    cuda_err(cudaGetDeviceCount(&device_count));

    return (device_count > 0);
}

std::unique_ptr<PedestalCalc> MakePedestalCalc(const DiffractionExperiment& experiment, int64_t lines, int32_t thread_id) {
    if (PedestalCalcGPU::GPUPresent())
        return std::make_unique<PedestalCalcGPU>(experiment, lines, thread_id);
    else
        return std::make_unique<PedestalCalcCPU>(experiment, lines);
}