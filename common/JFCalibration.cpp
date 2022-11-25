// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFCalibration.h"

#include <bitshuffle/bitshuffle.h>
#include "JFJochCompressor.h"

JFCalibration::JFCalibration(size_t in_nmodules, size_t in_nstorage_cells) :
nmodules(in_nmodules),
nstorage_cells(in_nstorage_cells),
pedestal(in_nmodules * in_nstorage_cells * 3),
mask(in_nmodules * RAW_MODULE_SIZE) {
    if (in_nmodules * in_nstorage_cells == 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Size of JFCalibration cannot be 0");
}

JFCalibration::JFCalibration(const DiffractionExperiment &experiment) :
        JFCalibration(experiment.GetModulesNum(), experiment.GetStorageCellNumber()) {}

size_t JFCalibration::GetModulesNum() const {
    return nmodules;
}

size_t JFCalibration::GetStorageCellNum() const {
    return nstorage_cells;
}

uint32_t &JFCalibration::Mask(size_t id) {
    return mask.at(id);
}

const uint32_t &JFCalibration::Mask(size_t id) const {
    return mask.at(id);
}

JFPedestal &JFCalibration::Pedestal(size_t module, size_t gain_level, size_t storage_cell) {
    if (gain_level >= 3)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Gain level must be in range 0-2");
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Module out of bounds");
    if (storage_cell >= nstorage_cells)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Storage cell " + std::to_string(storage_cell) + " out of bounds");
    return pedestal.at((storage_cell * nmodules + module) * 3 + gain_level);
}

const JFPedestal &JFCalibration::Pedestal(size_t module, size_t gain_level, size_t storage_cell) const {
    if (gain_level >= 3)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Gain level must be in range 0-2");
    if (module >= nmodules)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Module out of bounds");
    if (storage_cell >= nstorage_cells)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Storage cell " + std::to_string(storage_cell) + " out of bounds");
    return pedestal.at((storage_cell * nmodules + module) * 3 + gain_level);
}

std::vector<uint32_t> JFCalibration::CalculateMask(const DiffractionExperiment &experiment, size_t storage_cell) const {
    if (experiment.GetModulesNum() != nmodules)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Mismatch in module size");

    std::vector<uint32_t> tmp(RAW_MODULE_SIZE * nmodules, 0);

    for (int64_t module = 0; module < nmodules; module++) {
        for (int64_t line = 0; line < RAW_MODULE_LINES; line++) {
            for (int64_t col = 0; col < RAW_MODULE_COLS; col++) {
                int64_t pixel = module * RAW_MODULE_SIZE + line * RAW_MODULE_COLS + col;

                if (experiment.GetMaskModuleEdges()) {
                    if ((line == 0)
                        || (line == RAW_MODULE_LINES - 1)
                        || (col == 0)
                        || (col == RAW_MODULE_COLS - 1))
                        tmp[pixel] |= (1 << 30);
                }

                if (experiment.GetMaskChipEdges()) {
                    if ((col == 255) || (col == 256)
                        || (col == 511) || (col == 512)
                        || (col == 767) || (col == 768)
                        || (line == 255) || (line== 256))
                        tmp[pixel] |= (1 << 31);
                }

                tmp[pixel] |= mask[pixel];
                tmp[pixel] |= Pedestal(module, 0, storage_cell).Mask(line * RAW_MODULE_COLS + col);
                tmp[pixel] |= Pedestal(module, 1, storage_cell).Mask(line * RAW_MODULE_COLS + col);
                tmp[pixel] |= Pedestal(module, 2, storage_cell).Mask(line * RAW_MODULE_COLS + col);
            }
        }
    }

    return tmp;
}

std::vector<uint32_t> JFCalibration::CalculateNexusMask(const DiffractionExperiment &experiment, size_t storage_cell) const {
    auto mask_raw = CalculateMask(experiment, storage_cell);

    if (experiment.GetDetectorMode() == DetectorMode::Conversion) {
        std::vector<uint32_t> tmp(experiment.GetPixelsNum(), 1);
        RawToConvertedGeometry<uint32_t, uint32_t>(experiment, tmp.data(), mask_raw.data());
        return tmp;
    } else
        return mask_raw;
}

std::vector<uint8_t> JFCalibration::CalculateOneByteMask(const DiffractionExperiment &experiment, size_t storage_cell) const {
    auto nexus_mask = CalculateNexusMask(experiment, storage_cell);

    std::vector<uint8_t> tmp(experiment.GetPixelsNum());
    for (int i = 0; i < experiment.GetPixelsNum(); i++)
        tmp[i] = (nexus_mask[i] == 0);
    return tmp;
}

int64_t JFCalibration::CountMaskedPixels(size_t module, size_t storage_cell) const {
    int64_t ret = 0;
    for (int i = 0; i < RAW_MODULE_SIZE; i++) {
        if ((mask[RAW_MODULE_SIZE * module+i] != 0)
            || (Pedestal(module, 0, storage_cell).Mask(i) != 0)
            || (Pedestal(module, 1, storage_cell).Mask(i) != 0)
            || (Pedestal(module, 2, storage_cell).Mask(i) != 0))
            ret++;
    }
    return ret;
}

void
JFCalibration::GetModuleStatistics(JFJochProtoBuf::JFCalibrationStatistics &statistics, size_t storage_cell) const {
    for (int i = 0; i < nmodules; i++) {
        auto tmp = statistics.add_module_statistics();
        tmp->set_module_number(i);
        tmp->set_storage_cell_number(storage_cell);
        tmp->set_pedestal_g0_mean(Pedestal(i, 0, storage_cell).Mean());
        tmp->set_pedestal_g1_mean(Pedestal(i, 1, storage_cell).Mean());
        tmp->set_pedestal_g2_mean(Pedestal(i, 2, storage_cell).Mean());

        tmp->set_pedestal_rms_g0_mean(Pedestal(i, 0, storage_cell).MeanRMS());
        tmp->set_pedestal_rms_g1_mean(Pedestal(i, 1, storage_cell).MeanRMS());
        tmp->set_pedestal_rms_g2_mean(Pedestal(i, 2, storage_cell).MeanRMS());

        tmp->set_masked_pixels(CountMaskedPixels(i, storage_cell));

    }
}

JFJochProtoBuf::JFCalibrationStatistics JFCalibration::GetModuleStatistics(size_t storage_cell) const {
    JFJochProtoBuf::JFCalibrationStatistics output;
    GetModuleStatistics(output, storage_cell);
    return output;
}

JFJochProtoBuf::JFCalibrationStatistics JFCalibration::GetModuleStatistics() const {
    JFJochProtoBuf::JFCalibrationStatistics output;
    for (int s = 0; s < nstorage_cells; s++)
        GetModuleStatistics(output, s);
    return output;
}

JFCalibration::JFCalibration(const JFJochProtoBuf::JFCalibration &input) :
        JFCalibration(input.nmodules(), input.nstorage_cells()) {

    if (input.pedestal_size() != pedestal.size())
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError,
                              "Mismatch in size of serialized data");

    for (int i = 0; i < pedestal.size(); i++)
        pedestal[i] = input.pedestal(i);

    if (bshuf_decompress_zstd(input.mask().data(), mask.data(), nmodules * RAW_MODULE_SIZE,
                          sizeof(uint32_t), RAW_MODULE_SIZE) < 0 )
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Decompression error");
}

std::vector<float> JFCalibration::GetPedestalFP(size_t gain_level, size_t storage_cell) const {
    std::vector<float> ret(nmodules * RAW_MODULE_SIZE);
    for (int m = 0; m < nmodules; m++) {
        auto fp = Pedestal(m, gain_level, storage_cell).GetPedestalFP();
        memcpy(ret.data() + m * RAW_MODULE_SIZE, fp.data(), RAW_MODULE_SIZE * sizeof(float));
    }
    return ret;
}

std::vector<uint16_t> JFCalibration::GetPedestal(size_t gain_level, size_t storage_cell) const {
    std::vector<uint16_t> ret(nmodules * RAW_MODULE_SIZE);
    for (int m = 0; m < nmodules; m++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++)
            ret[m*RAW_MODULE_SIZE + i] = Pedestal(m, gain_level, storage_cell)[i];
    }
    return ret;
}

std::vector<int16_t> JFCalibration::GetPedestalRMS(size_t gain_level, size_t storage_cell) const {
    std::vector<int16_t> ret(nmodules * RAW_MODULE_SIZE);
    for (int m = 0; m < nmodules; m++) {
        for (int i = 0; i < RAW_MODULE_SIZE; i++)
            ret[m*RAW_MODULE_SIZE + i] = Pedestal(m, gain_level, storage_cell).RMS(i);
    }
    return ret;
}

JFCalibration::operator JFJochProtoBuf::JFCalibration() const {
    JFJochProtoBuf::JFCalibration output;

    output.set_nmodules(nmodules);
    output.set_nstorage_cells(nstorage_cells);

    for (const auto & i : pedestal) {
        auto tmp = output.add_pedestal();
        *tmp = i;
    }

    std::vector<char> tmp(bshuf_compress_zstd_bound(RAW_MODULE_SIZE * nmodules, sizeof(uint32_t),
                                                    RAW_MODULE_SIZE));

    auto size = bshuf_compress_zstd(mask.data(), tmp.data(), RAW_MODULE_SIZE * nmodules,
                                    sizeof(uint32_t),RAW_MODULE_SIZE, 0);
    if (size < 0)
        throw JFJochException(JFJochExceptionCategory::ZSTDCompressionError, "Compression error");

    output.set_mask(tmp.data(), size);

    return output;
}

JFCalibration::operator JFJochProtoBuf::JFCalibrationStatistics() const {
    return GetModuleStatistics();
}

template <class T>
std::vector<uint8_t> CompressCalibration(const std::vector<T> &c) {
    std::vector<uint8_t> compressed_output(bshuf_compress_lz4_bound(c.size(), sizeof(T),0) + 12);
    JFJochBitShuffleCompressor compressor(CompressionAlgorithm::BSHUF_LZ4, 0, 0);
    compressor.Compress(compressed_output.data(), c);
    return compressed_output;
}

void JFCalibration::Export(const DiffractionExperiment& experiment, JFJochProtoBuf::WriterMetadataInput &request) const {
    auto compressed_mask = CompressCalibration(CalculateNexusMask(experiment));
    request.mutable_calibration_metadata()->set_mask_bshuf_lz4(compressed_mask.data(), compressed_mask.size());
    for (int i = 0; i < experiment.GetStorageCellNumber(); i++) {
        std::string suffix;
        if (experiment.GetStorageCellNumber() > 1)
            suffix = "_sc" + std::to_string(i);
        for (int gain = 0; gain < 3; gain++) {
            auto entry = request.mutable_calibration_metadata()->add_calibration();
            auto compressed_pedestal = CompressCalibration(GetPedestal(gain, i));
            entry->set_compressed_bshuf_lz4(compressed_pedestal.data(), compressed_pedestal.size());
            entry->set_name("pedestalG" + std::to_string(gain) + suffix);
            entry->set_units("ADU");
        }
        for (int gain = 0; gain < 3; gain++) {
            auto entry = request.mutable_calibration_metadata()->add_calibration();
            auto compressed_pedestal = CompressCalibration(GetPedestalRMS(gain, i));
            entry->set_compressed_bshuf_lz4(compressed_pedestal.data(), compressed_pedestal.size());
            entry->set_name("pedestalG" + std::to_string(gain) + "_RMS" + suffix);
            entry->set_units("0.5 ADU");
        }
    }
}