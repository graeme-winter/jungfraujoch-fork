// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <fstream>

#include "JFJochException.h"
#include "JungfrauCalibration.h"
#include "RawToConvertedGeometry.h"
#include <bitshuffle/bitshuffle.h>

template <class T> size_t Compress(const std::vector<T> &input, std::vector<char> &output) {
    if (output.size() < bshuf_compress_zstd_bound(input.size(), sizeof(T), input.size()))
        output.resize(bshuf_compress_zstd_bound(input.size(), sizeof(T), input.size()));

    return bshuf_compress_lz4(input.data(), output.data(), input.size(), sizeof(T), input.size());
}

template <class T> void Decompress(const std::string& string, std::vector<T> &output) {
    if (string.size() > 0)
        bshuf_decompress_lz4(string.data(), output.data(), output.size(), sizeof(T), output.size());
}

JungfrauCalibration::JungfrauCalibration(const DiffractionExperiment &experiment, uint16_t in_data_stream)
: JungfrauCalibration(experiment.GetModulesNum(in_data_stream), in_data_stream) {}

JungfrauCalibration::JungfrauCalibration(uint16_t in_nmodules, uint16_t in_data_stream) :
        nmodules(in_nmodules),
        data_stream(in_data_stream),
        pedestal_frames(3),
        pedestal_time(3)
        {
    mask.resize(in_nmodules * RAW_MODULE_SIZE);
    pedestal[0].resize(RAW_MODULE_SIZE*nmodules);
    pedestal[1].resize(RAW_MODULE_SIZE*nmodules);
    pedestal[2].resize(RAW_MODULE_SIZE*nmodules);
    pedestalFP[0].resize(RAW_MODULE_SIZE*nmodules);
    pedestalFP[1].resize(RAW_MODULE_SIZE*nmodules);
    pedestalFP[2].resize(RAW_MODULE_SIZE*nmodules);
    pedestal_rms[0].resize(RAW_MODULE_SIZE*nmodules, -1);
    pedestal_rms[1].resize(RAW_MODULE_SIZE*nmodules, -1);
    pedestal_rms[2].resize(RAW_MODULE_SIZE*nmodules, -1);
}

JungfrauCalibration::JungfrauCalibration(const JFJochProtoBuf::JFCalibration &calibration) {
    nmodules = calibration.nmodules();
    data_stream = calibration.data_stream();

    mask.resize(RAW_MODULE_SIZE*nmodules);
    pedestal[0].resize(RAW_MODULE_SIZE*nmodules);
    pedestal[1].resize(RAW_MODULE_SIZE*nmodules);
    pedestal[2].resize(RAW_MODULE_SIZE*nmodules);
    pedestal_rms[0].resize(RAW_MODULE_SIZE*nmodules, -1);
    pedestal_rms[1].resize(RAW_MODULE_SIZE*nmodules, -1);
    pedestal_rms[2].resize(RAW_MODULE_SIZE*nmodules, -1);

    Decompress(calibration.pedestal_g0(), pedestal[0]);
    Decompress(calibration.pedestal_g1(), pedestal[1]);
    Decompress(calibration.pedestal_g2(), pedestal[2]);

    Decompress(calibration.pedestal_rms_g0(), pedestal_rms[0]);
    Decompress(calibration.pedestal_rms_g1(), pedestal_rms[1]);
    Decompress(calibration.pedestal_rms_g2(), pedestal_rms[2]);

    Decompress(calibration.mask(), mask);

    pedestal_time = {calibration.pedestal_time().begin(), calibration.pedestal_time().end()};
    pedestal_frames = {calibration.pedestal_frames().begin(), calibration.pedestal_frames().end()};
}

inline uint16_t to_fixed(double val, uint16_t fractional_bits) {
    // If val is result of division by zero, only reasonable value of output is zero (otherwise number could be interpreted improperly)
    uint32_t int_val = std::isfinite(val) ? (std::lround(val * (1<<fractional_bits))) : 0;
    // It is unlikely (but not impossible), that gain value will be lower than the smallest possible
    // Then reciprocal of gain could be more than allowed by data format. Protection is added for this condition
    if (int_val > UINT16_MAX) int_val = UINT16_MAX;
    return int_val;
}

void
JungfrauCalibration::ImportFPGAInput(const DiffractionExperiment &experiment, const uint16_t *arr, uint16_t data_stream,
                                     uint16_t module, bool import_mask) {
    uint8_t experiment_gain;
    switch (experiment.GetDetectorMode()) {
        case DetectorMode::PedestalG1:
            experiment_gain = 1;
            break;
        case DetectorMode::PedestalG2:
            experiment_gain = 2;
            break;
        default:
            experiment_gain = 0;
            break;
    }

    size_t offset = 0;
    if (data_stream != TASK_NO_DATA_STREAM)
        offset = RAW_MODULE_SIZE * experiment.GetFirstModuleOfDataStream(data_stream);
    offset += module * RAW_MODULE_SIZE;

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal[experiment_gain][offset+i] = arr[i];
        if (import_mask) {
            if (pedestal[experiment_gain][offset + i] > 4*16383)
                mask[offset + i] |= 2 << experiment_gain;
            else
                mask[offset + i] &= ~(2 << experiment_gain);
        }
    }
}

template<class T> void JungfrauCalibration::LoadModulePedestal(const std::vector<T> &vector, uint16_t module) {
    if (module >= nmodules) throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal[0][module * RAW_MODULE_SIZE + i] = to_fixed(vector[i], 2);
        pedestalFP[0][module * RAW_MODULE_SIZE + i] = vector[i];
        pedestal_rms[0][module * RAW_MODULE_SIZE + i] = -1;
    }

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal[1][module * RAW_MODULE_SIZE + i] = to_fixed(vector[i + RAW_MODULE_SIZE], 2);
        pedestalFP[1][module * RAW_MODULE_SIZE + i] = vector[i + RAW_MODULE_SIZE];
        pedestal_rms[1][module * RAW_MODULE_SIZE + i] = -1;
    }

    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        pedestal[2][module * RAW_MODULE_SIZE + i] = to_fixed(vector[i + 2 * RAW_MODULE_SIZE], 2);
        pedestalFP[2][module * RAW_MODULE_SIZE + i] = vector[i + 2 * RAW_MODULE_SIZE];
        pedestal_rms[2][module * RAW_MODULE_SIZE + i] = -1;
    }
}

template<class T> void JungfrauCalibration::LoadMask(const std::vector<T> &vector) {
    for (int i = 0; i < RAW_MODULE_SIZE *nmodules; i++)
        mask[i] = vector[i];
}


static void MaskModuleEdges(size_t nmodules, std::vector<uint32_t> &mask) {
    for (int i = 0; i < nmodules; i++) {
        size_t pixel0 = i * RAW_MODULE_SIZE;
        for (int line = 0; line < RAW_MODULE_LINES; line++) {
            for (int col = 0; col < RAW_MODULE_COLS; col++) {
                if ((col == 0) || (col == 1023) || (line == 0) || (line == 511))
                    mask[pixel0 + line * RAW_MODULE_COLS + col] |= (1<<30);
            }
        }
    }
}

static void MaskChipEdges(size_t nmodules, std::vector<uint32_t> &mask) {
    for (int i = 0; i < nmodules; i++) {
        size_t pixel0 = i * RAW_MODULE_SIZE;
        for (int line = 0; line < RAW_MODULE_LINES; line++) {
            for (int col = 0; col < RAW_MODULE_COLS; col++) {
                if ((col == 255) || (col == 256) || (col == 511) || (col == 512)
                    || (col == 767) || (col == 768) || (line == 255) || (line == 256))
                    mask[pixel0 + line * RAW_MODULE_COLS + col] |= (1<<31);
            }
        }
    }
}

void JungfrauCalibration::GetMaskTransformed(const DiffractionExperiment& experiment, std::vector<uint32_t> &vector) const {
    std::vector<uint32_t> tmp = mask;

    if (experiment.GetMaskModuleEdges())
        MaskModuleEdges(nmodules, tmp);

    if (experiment.GetMaskChipEdges())
        MaskChipEdges(nmodules, tmp);

    vector.resize(experiment.GetPixelsNum());

    for (auto &i : vector)
        i = 1;

    if (experiment.GetDetectorMode() == DetectorMode::Conversion)
        RawToConvertedGeometry<uint32_t, uint32_t>(experiment, vector.data(), tmp.data());
    else
        for (int i = 0; i < nmodules * RAW_MODULE_SIZE; i++ ) vector[i] = tmp[i];
}


std::vector<uint8_t> JungfrauCalibration::GetMaskOneByte(const DiffractionExperiment& experiment) const {
    std::vector<uint32_t> nexus_mask;
    GetMaskTransformed(experiment, nexus_mask);
    std::vector<uint8_t> tmp(experiment.GetPixelsNum(), 1);
    for (int i = 0; i < nexus_mask.size(); i++)
        if (nexus_mask[i] != 0)
            tmp[i] = 0;
    return tmp;
}

void JungfrauCalibration::LoadMaskTransformed(const DiffractionExperiment &experiment, const std::vector<uint32_t> &conv_mask,
                                         int32_t bit_val) {
    std::vector<uint32_t> raw_mask(experiment.GetModulesNum() * RAW_MODULE_SIZE);
    ConvertedToRawGeometry(experiment, raw_mask.data(), conv_mask.data());
    for (int i = 0; i < experiment.GetModulesNum() * RAW_MODULE_SIZE; i++) {
        if (raw_mask[i] != 0)
            mask[i] |= 1<<bit_val;
        else
            mask[i] &= ~(1<<bit_val);
    }
}

JungfrauCalibration::operator JFJochProtoBuf::JFCalibration() const {
    JFJochProtoBuf::JFCalibration calibration;

    calibration.set_nmodules(nmodules);
    calibration.set_data_stream(data_stream);

    std::vector<char> tmp(nmodules * 8 * RAW_MODULE_SIZE);

    auto pedestal_g0_size = Compress(pedestal[0], tmp);
    calibration.set_pedestal_g0(tmp.data(), pedestal_g0_size);
    auto pedestal_g1_size = Compress(pedestal[1], tmp);
    calibration.set_pedestal_g1(tmp.data(), pedestal_g1_size);
    auto pedestal_g2_size = Compress(pedestal[2], tmp);
    calibration.set_pedestal_g2(tmp.data(), pedestal_g2_size);

    auto pedestal_rms_g0_size = Compress(pedestal_rms[0], tmp);
    calibration.set_pedestal_rms_g0(tmp.data(), pedestal_rms_g0_size);
    auto pedestal_rms_g1_size = Compress(pedestal_rms[1], tmp);
    calibration.set_pedestal_rms_g1(tmp.data(), pedestal_rms_g1_size);
    auto pedestal_rms_g2_size = Compress(pedestal_rms[2], tmp);
    calibration.set_pedestal_rms_g2(tmp.data(), pedestal_rms_g2_size);

    auto mask_size = Compress(mask, tmp);
    calibration.set_mask(tmp.data(), mask_size);

    *calibration.mutable_pedestal_frames() = {pedestal_frames.begin(), pedestal_frames.end()};
    *calibration.mutable_pedestal_time() = {pedestal_time.begin(), pedestal_time.end()};

    for (int i = 0; i < nmodules; i++) {
        auto ptr = calibration.add_module_statistics();
        ptr->set_pedestal_g0_mean(MeanPedestal(0, i));
        ptr->set_pedestal_g1_mean(MeanPedestal(1, i));
        ptr->set_pedestal_g2_mean(MeanPedestal(2, i));

        ptr->set_pedestal_rms_g0_mean(MeanPedestalRMS(0, i));
        ptr->set_pedestal_rms_g1_mean(MeanPedestalRMS(1, i));
        ptr->set_pedestal_rms_g2_mean(MeanPedestalRMS(2, i));
        ptr->set_masked_pixels(CountMaskedPixels(i));
    }

    return calibration;
}

void JungfrauCalibration::ImportPedestalAndMask(const JFJochProtoBuf::JFCalibration & calibration) {
    if (calibration.nmodules() != nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Inconsistent modules number");

    if (calibration.data_stream() != data_stream)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Inconsistent data_stream number");

    Decompress(calibration.pedestal_g0(), pedestal[0]);
    Decompress(calibration.pedestal_g1(), pedestal[1]);
    Decompress(calibration.pedestal_g2(), pedestal[2]);

    Decompress(calibration.pedestal_rms_g0(), pedestal_rms[0]);
    Decompress(calibration.pedestal_rms_g1(), pedestal_rms[1]);
    Decompress(calibration.pedestal_rms_g2(), pedestal_rms[2]);

    Decompress(calibration.mask(), mask);

    pedestal_time = {calibration.pedestal_time().begin(), calibration.pedestal_time().end()};
    pedestal_frames = {calibration.pedestal_frames().begin(), calibration.pedestal_frames().end()};
}

void JungfrauCalibration::SetPedestalProperties(const DiffractionExperiment &experiment, time_t meas_time) {
    if (experiment.GetDetectorMode() == DetectorMode::PedestalG1) {
        pedestal_frames[1] = experiment.GetPedestalG1Frames();
        if (meas_time > 0) pedestal_time[1] = meas_time;
    } else if (experiment.GetDetectorMode() == DetectorMode::PedestalG2) {
        pedestal_frames[2] = experiment.GetPedestalG2Frames();
        if (meas_time > 0) pedestal_time[2] = meas_time;
    } else if (experiment.GetDetectorMode() == DetectorMode::PedestalG0) {
        // This can be explicit G0 pedestal or pedestal tracking
        pedestal_frames[0] = experiment.GetPedestalG0Frames();
        if (meas_time > 0) pedestal_time[0] = meas_time;
    }
}

void JungfrauCalibration::SetPedestalProperties(uint8_t gain_level, uint32_t frames, time_t meas_time) {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");
    pedestal_frames[gain_level] = frames;
    if (meas_time > 0)
        pedestal_time[gain_level] = meas_time;
    else
        time(&pedestal_time[gain_level]);
}

size_t JungfrauCalibration::GetPedestalFrames(uint8_t gain_level) const {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");
    return pedestal_frames[gain_level];
}

uint32_t JungfrauCalibration::GetPedestalTimestamp(uint8_t gain_level) const{
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");
    return pedestal_time[gain_level];
}

const std::vector<int16_t> & JungfrauCalibration::PedestalRMS(uint8_t gain_level) const {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");

    return pedestal_rms[gain_level];
}

const std::vector<uint16_t> & JungfrauCalibration::Pedestal(uint8_t gain_level) const {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");

    return pedestal[gain_level];
}

const std::vector<float> & JungfrauCalibration::PedestalFP(uint8_t gain_level) const {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");
    if (pedestalFP[gain_level].size() == 0)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Floating-point gain is not transferred via ProtoBuf and not available remotely");
    return pedestalFP[gain_level];
}

const std::vector<uint32_t> & JungfrauCalibration::Mask() const {
    return mask;
}

std::vector<uint16_t> & JungfrauCalibration::Pedestal(uint8_t gain_level) {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");

    return pedestal[gain_level];
}

std::vector<int16_t> & JungfrauCalibration::PedestalRMS(uint8_t gain_level) {
    if (gain_level > 3)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Gain level needs to be in range 0-2");

    return pedestal_rms[gain_level];
}

std::vector<uint32_t> & JungfrauCalibration::Mask() {
    return mask;
}

template void JungfrauCalibration::LoadMask<uint64_t>(const std::vector<uint64_t> &vector);
template void JungfrauCalibration::LoadMask<uint32_t>(const std::vector<uint32_t> &vector);
template void JungfrauCalibration::LoadMask<uint16_t>(const std::vector<uint16_t> &vector);

template void JungfrauCalibration::LoadModulePedestal<double>(const std::vector<double> &vector, uint16_t module);
template void JungfrauCalibration::LoadModulePedestal<float>(const std::vector<float> &vector, uint16_t module);

double JungfrauCalibration::MeanPedestal(uint8_t gain_level, uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");
    double x = 0;
    size_t count = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (mask[module * RAW_MODULE_SIZE + i] == 0) {
            x += pedestal[gain_level][module * RAW_MODULE_SIZE + i];
            count++;
        }
    }
    if (count == 0)
        return std::nan("");
    else
        return x / count / 4.0;
}

double JungfrauCalibration::MeanPedestalRMS(uint8_t gain_level, uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");
    double x = 0;
    size_t count = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if (mask[module * RAW_MODULE_SIZE + i] == 0) {
            x += pedestal_rms[gain_level][module * RAW_MODULE_SIZE + i];
            count++;
        }
    }
    if (x < 0)
        return std::nan("");
    if (count == 0)
        return std::nan("");
    else
        return x / count / PEDESTAL_RMS_MULTIPLIER;
}

size_t JungfrauCalibration::CountMaskedPixels(uint16_t module) const {
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module out of bounds");
    size_t x = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++)
        if (mask[module * RAW_MODULE_SIZE + i] != 0) x++;
    return x;
}

size_t JungfrauCalibration::CountMaskedPixels() const {
    size_t x = 0;
    for (const auto &i: mask)
        if (i != 0) x++;
    return x;
}

size_t JungfrauCalibration::GetDataStream() const {
    return data_stream;
}

uint16_t JungfrauCalibration::GetModulesNum() const {
    return nmodules;
}

double JungfrauCalibration::ComparePedestals(const JungfrauCalibration &c, uint8_t gain_level) const {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < nmodules * RAW_MODULE_SIZE; i++) {
        double diff = Pedestal(gain_level)[i] - c.Pedestal(gain_level)[i];
        sum_sq_diff += diff * diff;
    }
    return sqrt(sum_sq_diff / (nmodules * RAW_MODULE_SIZE)) / 4.0;
}

double JungfrauCalibration::ComparePedestals(const JungfrauCalibration &c, uint8_t gain_level, uint16_t module) const {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        double diff = Pedestal(gain_level)[i + module * RAW_MODULE_SIZE] - c.Pedestal(gain_level)[i + module * RAW_MODULE_SIZE];
        sum_sq_diff += diff * diff;
    }
    return sqrt(sum_sq_diff / (RAW_MODULE_SIZE)) / 4.0;
}