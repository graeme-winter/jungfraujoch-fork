// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SpotFinder.h"
#include "JFJochException.h"
#include <sstream>

// input      X x Y pixels array
// output     X x Y byte array with 1 or 0

struct CudaStreamWrapper {
    cudaStream_t v;
};

inline void cuda_err(cudaError_t val) {
    if (val != cudaSuccess)
        throw JFJochException(JFJochExceptionCategory::GPUCUDAError, cudaGetErrorString(val));
}

// Determine if pixel could be a spot
// params: spot finding parameters
// val: pixel value
// sum: window sum
// sum2: window sum of squares
// count: window valid pixels count
__device__ __forceinline__ uint8_t pixel_result(const spot_parameters& params, const int64_t val, int64_t sum, int64_t sum2, int64_t count)
{
    sum -= val;
    sum2 -= val * val;
    count -= 1;

    const int64_t var = count * sum2 - (sum * sum);   // This should be divided by ((2*NBX+1) * (2*NBY+1)-1)*((2*NBX+1) * (2*NBY+1))
    const int64_t in_minus_mean = val * count - sum;  // Should be divided by ((2*NBX+1) * (2*NBY+1));

    const int64_t tmp1 = in_minus_mean * in_minus_mean * (count-1);
    const float tmp2 = (var * count) * params.strong_pixel_threshold2;
    const bool strong_pixel = (val >= params.count_threshold) & (in_minus_mean > 0) & (tmp1 > tmp2);

    return strong_pixel ? 1 : 0;
}

// Find pixels that could be spots
// in: image input values
// out: boolean output values
// params: spot finding parameters
//
// The algorithm uses multiple waves (blockDim.y) that run over sections of rows.
// Each wave will write output at the back row and read input at the front row.
// Each wave is split into column output sections (blockDim.x)
// A wave section (block) is responsible for a particular row/column section and
// maintains sum/sum2/count values per column for the output row.
// Every cuda thread is associated with a particular column. The thread maintains
// the sum/sum2/count values in shared memory for it's column. To do this, the input
// pixel values for the hight of the aggregation window are saved in shared memory.
extern __shared__ int32_t shared_mem[];
__global__ void analyze_pixel(const int16_t *in, uint8_t *out, const spot_parameters params)
{
    // assumption: 2 * params.nby + 1 <= params.rows and 2 * params.nbx + 1 <= params.cols
    const int32_t window = 2 * (int)params.nby + 1;                                         // vertical window

    const int32_t writeSize = blockDim.x - 2 * params.nbx;                                  // output columns per block
    const int32_t cmin = blockIdx.x * writeSize;                                            // lowest output column
    const int32_t cmax = min(cmin + writeSize, static_cast<int32_t>(params.cols));     // past highest output column
    const int32_t col = cmin + threadIdx.x - params.nbx;                                    // thread -> column mapping
    const bool data_col = (col >= 0) & (col < static_cast<int32_t>(params.cols));           // read global mem
    const bool result_col = (col >= cmin) & (col < cmax);                                   // write result
    const int32_t nWaves = gridDim.y;                                                       // number of waves
    const int32_t rowsPerWave = (params.lines + nWaves - 1) / nWaves;                       // rows per wave
    const int32_t rmin = blockIdx.y * rowsPerWave;                                          // lowest result row for this wave
    const int32_t rmax = min(rmin + rowsPerWave, static_cast<int32_t>(params.lines));  // past highest result row for this wave

    const int32_t left = max(static_cast<int32_t>(threadIdx.x) - static_cast<int32_t>(params.nbx), 0); // leftmost column touched by this thread
    const int32_t right = min(static_cast<int32_t>(threadIdx.x) + static_cast<int32_t>(params.nbx) + 1, static_cast<int32_t>(params.cols)); // past rightmost column touched by this thread

    int32_t back = rmin;                                                                    // back of wave for writing
    int32_t front = max(back - static_cast<int32_t>(params.nby), 0);                   // front of wave for reading (needs to overtake back initially)

    int32_t* shared_sum = shared_mem;                                                       // shared buffer [blockDim.x]
    int32_t* shared_sum2 = &shared_sum[blockDim.x];                                         // shared buffer [blockDim.x]
    int16_t* shared_count = reinterpret_cast<int16_t*>(&shared_sum2[blockDim.x]);           // shared buffer [blockDim.x]
    int16_t* shared_val = &shared_count[blockDim.x];                                        // shared cyclic buffer [window, blockDim.x]

    int32_t total_sum;                                                                      // totals
    int32_t total_sum2;
    int32_t total_count;

    // initialize sum, sum2, count, val buffers
    const int16_t ini = params.min_viable_number - 1;                                       // value that is NOT counted
    shared_sum[threadIdx.x] = 0;                                                            // shared values without effect on totals
    shared_sum2[threadIdx.x] = 0;
    shared_count[threadIdx.x] = 0;
    for (int i=0; i<window; i++)
        shared_val[i * blockDim.x + threadIdx.x] = ini;

    // wave front up to rmin + nby + 1
    do {
        if (data_col) {     // read at the front end of the wave
            const int16_t val = in[front * params.cols + col];
            shared_val[(front % window) * blockDim.x + threadIdx.x] = val;
            if (val >= params.min_viable_number) {
                shared_sum[threadIdx.x] += val;
                shared_sum2[threadIdx.x] += val * val;
                shared_count[threadIdx.x] += 1;
            }
        }
        front++;
    } while (front < rmin + static_cast<int32_t>(params.nby) + 1);
    // wave front up to rmax
    do {
        __syncthreads();    // make others see the shared values
        if (result_col) {   // write at the back end of the wave
            total_sum = total_sum2 = total_count = 0;
            for (auto j = left; j < right; j++) {
                total_sum += shared_sum[j];
                total_sum2 += shared_sum2[j];
                total_count += shared_count[j];
            }
            out[back * params.cols + col] = pixel_result(params, shared_val[(back % window) * blockDim.x + threadIdx.x], total_sum, total_sum2, total_count);
        }
        back++;
        __syncthreads();    // keep shared values until others have seen them
        if (data_col) {     // read at the front end of the wave
            int16_t cnt = 0;
            int16_t old = shared_val[(front % window) * blockDim.x + threadIdx.x];
            if (old < params.min_viable_number) {
                old = 0;    // no effect value
                cnt = 1;    // bring count to normal
            }
            int16_t val = in[front * params.cols + col];
            shared_val[(front % window) * blockDim.x + threadIdx.x] = val;
            if (val < params.min_viable_number) {
                val = 0;    // no effect value
                cnt -= 1;   // count diff from normal
            }
            shared_sum[threadIdx.x] += val - old;
            shared_sum2[threadIdx.x] += val * val - old * old;
            shared_count[threadIdx.x] += cnt;
        }
        front++;
    } while (front < rmax);
    // wave back up to rmax
    do {
        __syncthreads();    // make others see the shared values
        if (result_col) {   // write at the back end of the wave
            total_sum = total_sum2 = total_count = 0;
            for (auto j = left; j < right; j++) {
                total_sum += shared_sum[j];
                total_sum2 += shared_sum2[j];
                total_count += shared_count[j];
            }
            out[back * params.cols + col] = pixel_result(params, shared_val[(back % window) * blockDim.x + threadIdx.x], total_sum, total_sum2, total_count);
        }
        back++;
        __syncthreads();    // keep shared values until others have seen them
        if (data_col) {     // read at the front end of the wave if possible
            int16_t cnt = -1; // normal count diff
            int16_t old = shared_val[(front % window) * blockDim.x + threadIdx.x];
            if (old < params.min_viable_number) {
                old = 0;    // no effect value
                cnt += 1;   // bring count to normal
            }
            int16_t val = 0;
            if (front < params.lines) {
                val = in[front * params.cols + col];
                if (val < params.min_viable_number)
                    val = 0;    // no effect value
                else
                    cnt += 1;   // count diff from normal
            }
            shared_sum[threadIdx.x] += val - old;
            shared_sum2[threadIdx.x] += val * val - old * old;
            shared_count[threadIdx.x] += cnt;
        }
        front++;
    } while (back < rmax);
}

SpotFinder::SpotFinder(int32_t in_xpixels, int32_t in_ypixels, int32_t gpu_device) :
        xpixels(in_xpixels), ypixels(in_ypixels), gpu_out(nullptr) {

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
    cuda_err(cudaMalloc(&gpu_in, xpixels * ypixels * sizeof(int16_t)));
    cuda_err(cudaMalloc(&gpu_out, xpixels * ypixels * sizeof(char)));
    cuda_err(cudaHostAlloc(&host_in_buffer, xpixels * ypixels * sizeof(int16_t),cudaHostAllocPortable));
    cuda_err(cudaHostAlloc(&host_out, xpixels * ypixels * sizeof(char), cudaHostAllocPortable));
}

SpotFinder::SpotFinder(const DiffractionExperiment &experiment, int32_t gpu_device) :
        SpotFinder(experiment.GetXPixelsNum(), experiment.GetYPixelsNum(), gpu_device) {}

SpotFinder::~SpotFinder() {
    cudaStreamDestroy(cudastream->v);
    delete(cudastream);

    cudaFreeHost(host_in_buffer);
    cudaFreeHost(host_out);
    cudaFree(gpu_in);
    cudaFree(gpu_out);
}

bool SpotFinder::GPUPresent() {
    int device_count;
    cuda_err(cudaGetDeviceCount(&device_count));

    return (device_count > 0);
}

int16_t *SpotFinder::GetInputBuffer() {
    return host_in_buffer;
}

void SpotFinder::RunSpotFinder(const JFJochProtoBuf::DataProcessingSettings &settings) {
    // Run COLSPOT (GPU version)
    spot_parameters spot_params;
    spot_params.strong_pixel_threshold2 = settings.signal_to_noise_threshold() * settings.signal_to_noise_threshold();
    spot_params.nbx = settings.local_bkg_size();
    spot_params.nby = settings.local_bkg_size();
    spot_params.lines = ypixels;
    spot_params.cols = xpixels;
    spot_params.count_threshold = settings.photon_count_threshold();
    spot_params.min_viable_number = INT16_MIN + 5;

    if (2 * spot_params.nbx + 1 > windowSizeLimit)
        throw JFJochException(JFJochExceptionCategory::SpotFinderError, "nbx exceeds window size limit");
    if (2 * spot_params.nby + 1 > windowSizeLimit)
        throw JFJochException(JFJochExceptionCategory::SpotFinderError, "nby exceeds window size limit");
    if (windowSizeLimit > numberOfCudaThreads)
        throw JFJochException(JFJochExceptionCategory::SpotFinderError, "window size limit exceeds number of cuda threads");
    if (windowSizeLimit > spot_params.cols)
        throw JFJochException(JFJochExceptionCategory::SpotFinderError, "window size limit exceeds number of columns");
    if (windowSizeLimit > spot_params.lines)
        throw JFJochException(JFJochExceptionCategory::SpotFinderError, "window size limit exceeds number of lines");

    cuda_err(cudaMemcpyAsync(gpu_in, host_in_buffer, xpixels * ypixels * sizeof(int16_t),
                             cudaMemcpyHostToDevice, cudastream->v));

    { // call cuda kernel
        const auto nWriters = numberOfCudaThreads - 2 * spot_params.nby;
        const auto nBlocks = (spot_params.cols + nWriters - 1) / nWriters;
        const auto window = 2 * spot_params.nby + 1;
        const auto sharedSize = (2 * sizeof(int32_t) +            // sum, sum2
                                                      (1 + window) * sizeof(int16_t)   // count, val
                                                     ) * numberOfCudaThreads;
        const dim3 blocks(nBlocks, numberOfWaves);

        analyze_pixel<<<blocks, numberOfCudaThreads, sharedSize, cudastream->v>>>((int16_t *) gpu_in, gpu_out, spot_params);
    }

    cuda_err(cudaMemcpyAsync(host_out, gpu_out, xpixels*ypixels*sizeof(char),
                             cudaMemcpyDeviceToHost,cudastream->v));
}

void SpotFinder::GetResults(StrongPixelSet &pixel_set, int64_t image_number) {
    cuda_err(cudaStreamSynchronize(cudastream->v));

    for (int line = 0; line < ypixels; line++) {
        for (int col = 0; col < xpixels; col++) {
            if (host_out[xpixels * line + col] > 0)
                pixel_set.AddStrongPixel(col, line, image_number, host_in_buffer[xpixels * line + col]);
        }
    }
}

void SpotFinder::GetResults(const DiffractionExperiment &experiment,
                            const JFJochProtoBuf::DataProcessingSettings &settings,
                            std::vector<DiffractionSpot> &vec,
                            int64_t image_number) {
    StrongPixelSet pixel_set;
    GetResults(pixel_set, image_number);
    pixel_set.FindSpots(experiment, settings, vec);
}

void SpotFinder::FindSpots(const DiffractionExperiment &experiment,
                           const JFJochProtoBuf::DataProcessingSettings &settings, std::vector<DiffractionSpot> &vec,
                           int64_t image_number) {
    RunSpotFinder(settings);
    GetResults(experiment, settings, vec, image_number);
}

std::atomic<uint16_t> SpotFinder::threadid{0};