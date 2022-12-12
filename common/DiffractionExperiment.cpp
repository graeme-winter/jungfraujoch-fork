// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <numeric>
#include <cmath>
#include <unistd.h>
#include <zstd.h>

#include "NetworkAddressConvert.h"
#include "JFJochCompressor.h" // For ZSTD_USE_JFJOCH_RLE
#include "GitInfo.h"
#include "DiffractionExperiment.h"
#include "JFJochException.h"
#include "../compression/MaxCompressedSize.h"

#define check_max(param, val, max) if ((val) > (max)) throw JFJochException(JFJochExceptionCategory::InputParameterAboveMax, param)
#define check_min(param, val, min) if ((val) < (min)) throw JFJochException(JFJochExceptionCategory::InputParameterBelowMin, param)

DiffractionExperiment::DiffractionExperiment(const JFJochProtoBuf::JungfraujochSettings &settings) : DiffractionExperiment() {
    Import(settings);
}

DiffractionExperiment& DiffractionExperiment::Import(const JFJochProtoBuf::JungfraujochSettings &settings) {
    internal = settings.internal();
    dataset = settings.dataset();
    return *this;
}

DiffractionExperiment::operator JFJochProtoBuf::JungfraujochSettings() const {
    JFJochProtoBuf::JungfraujochSettings settings;
    *settings.mutable_dataset() = dataset;
    *settings.mutable_internal() = internal;
    return settings;
}

DiffractionExperiment::DiffractionExperiment() : DiffractionExperiment(2, {4,4}, 0, 0, true)
{}

DiffractionExperiment::DiffractionExperiment(int64_t horizontal_stacking, const std::vector<int64_t> &vec, int64_t gap_x,
                                             int64_t gap_y, bool mirror_y_in_conversion) {
    check_min("Gap X",gap_x, 0);
    check_min("Gap Y",gap_y, 0);
    check_min("Horizontal module stacking",horizontal_stacking, 1);

    if (vec.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "At least one data stream with one module needs to be defined");

    for (const auto& val: vec) {
        if (val == 0)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Cannot define data stream with zero modules");
    }

    dataset.set_photon_energy_kev(WVL_1A_IN_KEV);
    dataset.set_detector_distance_mm(100);
    *dataset.mutable_scattering_vector() = Coord(0,0,1);

    dataset.set_data_file_count(1);
    dataset.set_file_prefix("test");

    dataset.set_ntrigger(1);
    dataset.set_images_per_trigger(1);
    dataset.set_summation(1);
    dataset.set_space_group_number(0); // not set

    dataset.set_compression(JFJochProtoBuf::BSHUF_LZ4);

    *internal.mutable_data_stream_modules() = {vec.begin(), vec.end()};

    internal.set_frame_time_us(MIN_FRAME_TIME_HALF_SPEED_IN_US);
    internal.set_count_time_us(MIN_FRAME_TIME_HALF_SPEED_IN_US - READOUT_TIME_IN_US);
    internal.set_frame_time_pedestalg1g2_us(FRAME_TIME_PEDE_G1G2_IN_US);

    internal.set_mask_chip_edges(false);
    internal.set_mask_module_edges(false);
    internal.set_type(JFJochProtoBuf::JUNGFRAU);

    internal.set_preview_period_us(1000*1000); // 1s / 1 Hz
    internal.set_spot_finding_period_us(10*1000); // 10 ms / 100 Hz
    internal.set_bkg_estimation_period_us(10*1000); // 10 ms / 100 Hz

    internal.set_low_q(0.1);
    internal.set_high_q(5.0);
    internal.set_q_spacing(0.1);

    internal.set_base_ipv4_address(0x0132010a);
    internal.set_base_udp_port(8192);
    internal.set_git_sha1(jfjoch_git_sha1());
    internal.set_git_date(jfjoch_git_date());

    internal.set_storage_cells(1);
    internal.set_storage_cell_start(15);

    CalculateGeometry(horizontal_stacking, vec, gap_x, gap_y, mirror_y_in_conversion);
    Mode(DetectorMode::Conversion);
}

void DiffractionExperiment::CalculateGeometry(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x,
                                              int64_t gap_y, bool mirror_y_in_conversion) {
    auto nmodules = GetModulesNum();

    const auto cgeom = internal.mutable_conv_geometry();

    cgeom->set_width_pxl(horizontal_stacking * CONVERTED_MODULE_COLS + (horizontal_stacking - 1) * gap_x);
    int64_t conv_lines = nmodules / horizontal_stacking + (nmodules % horizontal_stacking > 0 ? 1 : 0);
    cgeom->set_height_pxl(conv_lines * CONVERTED_MODULE_LINES + (conv_lines - 1) * gap_y);
    cgeom->set_mirror_modules_in_y(mirror_y_in_conversion);

    for (int module = 0; module < nmodules; module++) {
        int64_t module_x = module % horizontal_stacking;
        int64_t module_y = module / horizontal_stacking;

        int64_t pixel_x = module_x * (CONVERTED_MODULE_COLS + gap_x);
        int64_t pixel_y = module_y * (CONVERTED_MODULE_LINES + gap_y);

        if (mirror_y_in_conversion)
            pixel_y = cgeom->height_pxl() - pixel_y - 1;

        cgeom->add_pixel0_of_module(pixel_y * cgeom->width_pxl() + pixel_x);
    }

    const auto rgeom = internal.mutable_raw_geometry();
    rgeom->set_width_pxl(RAW_MODULE_COLS);
    rgeom->set_height_pxl(nmodules * RAW_MODULE_LINES);
    rgeom->set_mirror_modules_in_y(false);
    for (int module = 0; module < nmodules; module++)
        rgeom->add_pixel0_of_module( module * RAW_MODULE_SIZE);
}

// setter functions

DiffractionExperiment &DiffractionExperiment::Mode(DetectorMode input) {
    switch (input) {
        case DetectorMode::Conversion:
            internal.set_mode(JFJochProtoBuf::CONVERSION);
            break;
        case DetectorMode::Raw:
            internal.set_mode(JFJochProtoBuf::RAW);
            break;
        case DetectorMode::PedestalG0:
            internal.set_mode(JFJochProtoBuf::PEDESTAL_G0);
            break;
        case DetectorMode::PedestalG1:
            internal.set_mode(JFJochProtoBuf::PEDESTAL_G1);
            break;
        case DetectorMode::PedestalG2:
            internal.set_mode(JFJochProtoBuf::PEDESTAL_G2);
            break;
    }
    if (input == DetectorMode::Conversion)
        *internal.mutable_geometry() = internal.conv_geometry();
    else
        *internal.mutable_geometry() = internal.raw_geometry();

    return *this;
}

DiffractionExperiment &DiffractionExperiment::ImagesPerTrigger(int64_t input) {
    check_min("Total number of images", input, 0);
    dataset.set_images_per_trigger(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::NumTriggers(int64_t input) {
    check_max("Total number of triggers", input, INT16_MAX);
    check_min("Total number of triggers", input, 1);
    dataset.set_ntrigger(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::FrameTime(std::chrono::microseconds in_frame_time,
                                                        std::chrono::microseconds in_count_time) {
    check_min("Frame time (us)", in_frame_time.count(), MIN_FRAME_TIME_FULL_SPEED_IN_US);
    check_max("Frame time (us)", in_frame_time.count(), MAX_FRAME_TIME);
    check_max("Count time (us)", in_count_time.count(), in_frame_time.count() - READOUT_TIME_IN_US);
    if (in_count_time.count() != 0) {
        check_min("Count time (us)", in_count_time.count(), MIN_COUNT_TIME_IN_US);
    }

    internal.set_frame_time_us(in_frame_time.count());
    if (in_count_time.count() == 0)
        internal.set_count_time_us(in_frame_time.count() - READOUT_TIME_IN_US);
    else
        internal.set_count_time_us(in_count_time.count());

    return *this;
}

DiffractionExperiment &DiffractionExperiment::Summation(int64_t input) {
    check_min("Summation", input, 1);
    check_max("Summation", input, MAX_SUMMATION);
    dataset.set_summation(input);
    return *this;
}

DiffractionExperiment & DiffractionExperiment::ImageTimeUs(std::chrono::microseconds image_time) {
    check_min("Image time (us)", image_time.count(), MIN_FRAME_TIME_FULL_SPEED_IN_US); // Exclude negative numbers and zero
    if ( image_time.count() % GetFrameTime().count() != 0)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Image time must be multiple of frame time");
    Summation(image_time.count() / GetFrameTime().count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG1G2FrameTime(std::chrono::microseconds input) {
    check_min("Pedestal G1G2 frame time (us) ", input.count(), MIN_FRAME_TIME_FULL_SPEED_IN_US);
    internal.set_frame_time_pedestalg1g2_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG0Frames(int64_t input) {
    check_min("Pedestal G0 frames", input, 0);
    // Max?
    internal.set_pedestal_g0_frames(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG1Frames(int64_t input) {
    check_min("Pedestal G1 frames", input, 0);
    internal.set_pedestal_g1_frames(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG2Frames(int64_t input) {
    check_min("Pedestal G2 frames", input, 0);
    internal.set_pedestal_g2_frames(input);
    return *this;
}


DiffractionExperiment &DiffractionExperiment::PhotonEnergy_keV(double input) {
    check_min("Energy (keV)", input, MIN_ENERGY);
    check_max("Energy (keV)", input, MAX_ENERGY);
    dataset.set_photon_energy_kev(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamX_pxl(double input) {
    dataset.set_beam_x_pxl(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamY_pxl(double input) {
    dataset.set_beam_y_pxl(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DetectorDistance_mm(double input) {
    check_min("Detector distance (mm)", input, 1);
    dataset.set_detector_distance_mm(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ScatteringVector(Coord input) {
    *dataset.mutable_scattering_vector() = input.Normalize();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ScatteringVector() {
    dataset.clear_scattering_vector();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::FilePrefix(std::string input) {
    // File prefix with front slash is not allowed for security reasons
    if (input.front() == '/')
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Path cannot start with slash");
    if (input.substr(0,3) == "../")
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Path cannot start with ../");
    if (input.find("/../") != std::string::npos)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Path cannot contain /../");

    if ((input.find("_master.h5") == input.length() - 10) && (input.length() > 10))
        dataset.set_file_prefix(input.substr(0, input.length() - 10));
    else
        dataset.set_file_prefix(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DataFileCount(int64_t input) {
    check_min("File count", input, 1);
    check_max("File count", input, 1000);
    dataset.set_data_file_count(input);
    return *this;
}
DiffractionExperiment &DiffractionExperiment::UseInternalPacketGenerator(bool input) {
    internal.set_internal_fpga_packet_generator(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BaseIPv4Address(std::string input) {
    BaseIPv4Address(IPv4AddressFromStr(input)); // IPv4 address is passed as low endian to FPGA
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BaseIPv4Address(int64_t input) {
    check_min("IPv4 address", input, 1);
    check_max("IPv4 address", input, (1L<<33) - 1);
    check_max("IPv4 address last 2-bytes", input >> 24, 128);
    // not really a good idea to allow last part of address close to end of address space
    internal.set_base_ipv4_address(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BaseUDPPort(int64_t input) {
    check_min("UDP port", input, 1024);
    check_max("UDP port", input, 65535);
    if (input % 64 != 0)
        throw JFJochException(JFJochExceptionCategory::WrongNumber, "Base UDP port must be multiple of 64");
    internal.set_base_udp_port(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::MaskModuleEdges(bool input) {
    internal.set_mask_module_edges(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::Compression(JFJochProtoBuf::Compression input) {
    switch (input) {
        case JFJochProtoBuf::BSHUF_ZSTD:
            dataset.set_compression(JFJochProtoBuf::BSHUF_ZSTD);
            break;
        case JFJochProtoBuf::BSHUF_ZSTD_RLE:
            dataset.set_compression(JFJochProtoBuf::BSHUF_ZSTD_RLE);
            break;
        case JFJochProtoBuf::BSHUF_LZ4:
            dataset.set_compression(JFJochProtoBuf::BSHUF_LZ4);
            break;
        default:
            dataset.set_compression(JFJochProtoBuf::NO_COMPRESSION);
            break;
    }
    return *this;
}

DiffractionExperiment &DiffractionExperiment::MaskChipEdges(bool input) {
    internal.set_mask_chip_edges(input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::LowResForRadialInt_A(double input) {
    check_min("Low Resolution for radial integration", input, 0.1);
    check_max("Low Resolution for radial integration", input, 500.0);
    internal.set_low_q(2 * M_PI / input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::HighResForRadialInt_A(double input) {
    check_min("High Resolution for radial integration", input, 0.1);
    check_max("High Resolution for radial integration", input, 500.0);
    internal.set_high_q(2 * M_PI / input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::LowQForRadialInt_recipA(double input) {
    check_min("Low Q for radial integration", input, 0.001);
    check_max("Low Q for radial integration", input, 10.0);
    internal.set_low_q(input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::HighQForRadialInt_recipA(double input) {
    check_min("High Q for radial integration", input, 0.001);
    check_max("High Q for radial integration", input, 10.0);
    internal.set_high_q(input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::QSpacingForRadialInt_recipA(double input) {
    check_min("Q spacing for radial integration", input, 0.01);
    internal.set_q_spacing(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DetectorType(JFJochProtoBuf::DetectorType type) {
    internal.set_type(type);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SetUnitCell(const JFJochProtoBuf::UnitCell &cell) {
    *dataset.mutable_unit_cell() = cell;
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SetUnitCell() {
    dataset.clear_unit_cell();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PreviewPeriod(std::chrono::microseconds input) {
    check_min("Preview image generation period", input.count(), 0);
    internal.set_preview_period_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SpotFindingPeriod(std::chrono::microseconds input) {
    check_min("Spot finding image generation period", input.count(), 0);
    internal.set_spot_finding_period_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BackgroundEstimationPeriod(std::chrono::microseconds input) {
    check_min("Background estimation generation period", input.count(), 0);
    internal.set_bkg_estimation_period_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SpaceGroupNumber(int64_t input) {
    check_min("Space group number", input, 0);
    check_max("Space group number", input, 230);
    dataset.set_space_group_number(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::StorageCells(int64_t input) {
    check_min("Storage cell number", input, 1);
    check_max("Storage cell number", input, 16);
    internal.set_storage_cells(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::StorageCellStart(int64_t input) {
    check_min("Start storage cell", input, 0);
    check_max("Start storage cell", input, 15);
    internal.set_storage_cell_start(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SampleName(std::string input) {
    dataset.set_sample_name(input);
    return *this;
}

// getter functions

int64_t DiffractionExperiment::GetNumTriggers() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            return dataset.ntrigger();
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
        default:
            return 1;
    }
}

DetectorMode DiffractionExperiment::GetDetectorMode() const {
    switch (internal.mode()) {
        case JFJochProtoBuf::RAW:
            return DetectorMode::Raw;
          case JFJochProtoBuf::PEDESTAL_G0:
            return DetectorMode::PedestalG0;
        case JFJochProtoBuf::PEDESTAL_G1:
            return DetectorMode::PedestalG1;
        case JFJochProtoBuf::PEDESTAL_G2:
            return DetectorMode::PedestalG2;
        default:
        case JFJochProtoBuf::CONVERSION:
            return DetectorMode::Conversion;
    }
}

int64_t DiffractionExperiment::GetSummation() const {
    if (GetDetectorMode() == DetectorMode::Conversion) {
        if (dataset.has_image_time_us())
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                                  "Internally DiffractionExperiment shouldn't use image_time_us");
        return dataset.summation();
    } else
        return 1;
}

std::chrono::microseconds DiffractionExperiment::GetFrameTime() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return std::chrono::microseconds(internal.frame_time_pedestalg1g2_us());
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
        case DetectorMode::PedestalG0:
        default:
            return std::chrono::microseconds(internal.frame_time_us());
    }
}

std::chrono::microseconds DiffractionExperiment::GetImageTime() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return std::chrono::microseconds(internal.frame_time_pedestalg1g2_us());
        case DetectorMode::Conversion:
            return GetFrameTime() * GetSummation();
        case DetectorMode::Raw:
        case DetectorMode::PedestalG0:
        default:
            return GetFrameTime();
    }
}

int64_t DiffractionExperiment::GetImageNum() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
                return GetImageNumPerTrigger() * GetNumTriggers();
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
        default:
             return 0;
    }
}

int64_t DiffractionExperiment::GetImageNumPerTrigger() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            if (GetStorageCellNumber() > 1)
                return GetStorageCellNumber();
            return GetFrameNumPerTrigger() / GetSummation();
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
        default:
            return 0;
    }
}

int64_t DiffractionExperiment::GetFrameNum() const {
    return GetFrameNumPerTrigger() * GetNumTriggers();
}

int64_t DiffractionExperiment::GetFrameNumPerTrigger() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            if (GetStorageCellNumber() > 1)
                return GetStorageCellNumber();
            else
                return dataset.images_per_trigger() * GetSummation();
        case DetectorMode::PedestalG0:
            return internal.pedestal_g0_frames() * GetStorageCellNumber();
        case DetectorMode::PedestalG1:
            return internal.pedestal_g1_frames() * GetStorageCellNumber();
        case DetectorMode::PedestalG2:
            return internal.pedestal_g2_frames() * GetStorageCellNumber();
        default:
            return 0;
    }
}

std::chrono::microseconds DiffractionExperiment::GetFrameCountTime() const {
    return std::chrono::microseconds(internal.count_time_us());
}

std::chrono::microseconds DiffractionExperiment::GetImageCountTime() const {
    return GetFrameCountTime() * GetSummation();
}

int64_t DiffractionExperiment::GetPedestalG0Frames() const {
    return internal.pedestal_g0_frames();
}

int64_t DiffractionExperiment::GetPedestalG1Frames() const {
    return internal.pedestal_g1_frames();
}

int64_t DiffractionExperiment::GetPedestalG2Frames() const {
    return internal.pedestal_g2_frames();
}

double DiffractionExperiment::GetPhotonEnergy_keV() const {
    return dataset.photon_energy_kev();
}

double DiffractionExperiment::GetWavelength_A() const {
    return WVL_1A_IN_KEV / dataset.photon_energy_kev();
}

double DiffractionExperiment::GetBeamX_pxl() const {
    return dataset.beam_x_pxl();
}

double DiffractionExperiment::GetBeamY_pxl() const {
    return dataset.beam_y_pxl();
}

double DiffractionExperiment::GetDetectorDistance_mm() const {
    return dataset.detector_distance_mm();
}

Coord DiffractionExperiment::GetScatteringVector() const {
    if (dataset.has_scattering_vector())
        return (Coord)dataset.scattering_vector() * (dataset.photon_energy_kev() / WVL_1A_IN_KEV);
    else
        return {0,0,dataset.photon_energy_kev() / WVL_1A_IN_KEV};
}

std::string DiffractionExperiment::GetFilePrefix() const {
    return dataset.file_prefix();
}

int64_t DiffractionExperiment::GetDataFileCount() const {
    return dataset.data_file_count();
}

JFJochProtoBuf::Compression DiffractionExperiment::GetCompressionAlgorithm() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
            return dataset.compression();
        default:
            // Compression not supported for raw data and pedestal
            return JFJochProtoBuf::NO_COMPRESSION;
    }
}

CompressionAlgorithm DiffractionExperiment::GetCompressionAlgorithmEnum() const {
    switch (GetCompressionAlgorithm()) {
        case JFJochProtoBuf::BSHUF_LZ4:
            return CompressionAlgorithm::BSHUF_LZ4;
        case JFJochProtoBuf::BSHUF_ZSTD:
            return CompressionAlgorithm::BSHUF_ZSTD;
        case JFJochProtoBuf::BSHUF_ZSTD_RLE:
            return CompressionAlgorithm::BSHUF_ZSTD_RLE;
        default:
            return CompressionAlgorithm::NO_COMPRESSION;
    }
}

int64_t DiffractionExperiment::GetPixelDepth() const {
    if (GetSummation() == 1) return 2;
    else return 4;
}

bool DiffractionExperiment::IsPixelSigned() const {
    return (GetDetectorMode() == DetectorMode::Conversion) && (GetDetectorType() == JFJochProtoBuf::JUNGFRAU);
}

int64_t DiffractionExperiment::GetDataStreamsNum() const {
    return internal.data_stream_modules_size();
}

int64_t DiffractionExperiment::GetModulesNum(uint16_t data_stream) const {
    if (data_stream == TASK_NO_DATA_STREAM)
        return std::accumulate(internal.data_stream_modules().begin(), internal.data_stream_modules().end(), 0);
    if (data_stream >= GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non exisiting data stream");
    return internal.data_stream_modules(data_stream);
}

int64_t DiffractionExperiment::GetFirstModuleOfDataStream(uint16_t data_stream) const {
    if (data_stream == TASK_NO_DATA_STREAM)
        return 0;
    if (data_stream >= GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non exisiting data stream");

    int64_t val = 0;
    for (int i = 0; i < data_stream; i++) val += GetModulesNum(i);
    return val;
}

int64_t DiffractionExperiment::GetMaxCompressedSize() const {
    return MaxCompressedSize(GetCompressionAlgorithmEnum(),GetPixelsNum(), GetPixelDepth());
}

int64_t DiffractionExperiment::GetPixelsNum() const {
    return GetXPixelsNum() * GetYPixelsNum();
}

int64_t DiffractionExperiment::GetXPixelsNum() const {
    return internal.geometry().width_pxl();
}

int64_t DiffractionExperiment::GetYPixelsNum() const {
    return internal.geometry().height_pxl();
}

int64_t DiffractionExperiment::GetPixel0OfModule(uint16_t module) const {
    if (module >= internal.geometry().pixel0_of_module_size())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module number out of bounds");

    return internal.geometry().pixel0_of_module(module);
}

bool DiffractionExperiment::IsUpsideDown() const {
    return internal.geometry().mirror_modules_in_y();
}

int64_t DiffractionExperiment::GetOverflow() const {
    int64_t summation_rnd_pow2 = std::pow(2, std::ceil(std::log2(GetSummation())));
    return  summation_rnd_pow2 * INT16_MAX - 4L;
}

int64_t DiffractionExperiment::GetUnderflow() const {
    int64_t summation_rnd_pow2 = std::pow(2, std::ceil(std::log2(GetSummation())));
    return summation_rnd_pow2 * INT16_MIN + 4L;
}

std::chrono::microseconds DiffractionExperiment::GetPreviewPeriod() const {
    return std::chrono::microseconds(internal.preview_period_us());
}

std::chrono::microseconds DiffractionExperiment::GetSpotFindingPeriod() const {
    return std::chrono::microseconds(internal.spot_finding_period_us());
}

std::chrono::microseconds DiffractionExperiment::GetBackgroundEstimationPeriod() const {
    return std::chrono::microseconds(internal.bkg_estimation_period_us());
}

int64_t DiffractionExperiment::GetPreviewStride() const {
    return GetPreviewStride(GetPreviewPeriod());
}

int64_t DiffractionExperiment::GetSpotFindingStride() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
            return CalculateStride(GetImageTime(), GetSpotFindingPeriod());
        default:
            return 0;
    }
}

int64_t DiffractionExperiment::GetBackgroundEstimationStride() const {
    return GetPreviewStride(GetBackgroundEstimationPeriod());
}

int64_t DiffractionExperiment::GetSpotFindingBin() const {
    if (GetSpotFindingPeriod().count() <= 0)
        return 100;
    else if (GetSpotFindingPeriod().count() >= 1000*1000)
        return 1;
    else
        return 1000*1000/(GetSpotFindingPeriod().count()); // 1 bin = 1 second
}

int64_t DiffractionExperiment::GetPreviewStride(std::chrono::microseconds period) const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
            return CalculateStride(GetImageTime(), period);
        default:
            return 0;
    }
}

bool DiffractionExperiment::IsUsingInternalPacketGen() const {
    return internal.internal_fpga_packet_generator();
}

int32_t DiffractionExperiment::GetSrcIPv4Address(uint16_t half_module) const {
    if (half_module >= 2 * GetModulesNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non existing module");
    return internal.base_ipv4_address() + (((uint64_t)half_module + GetDataStreamsNum() + 2) << 24);
}
int32_t DiffractionExperiment::GetDestIPv4Address(uint16_t data_stream) const {
    if (data_stream >= GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non existing data stream");
    return internal.base_ipv4_address() + ((uint64_t)data_stream << 24);
}

uint16_t DiffractionExperiment::GetDestUDPPort(uint16_t data_stream, uint16_t module) const {
    if (data_stream >= GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non existing data stream");
    if (module >= GetModulesNum(data_stream))
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non existing module");
    return internal.base_udp_port() + 2 * module;
}

bool DiffractionExperiment::CheckGitSha1Consistent() const {
    return (internal.git_sha1() == jfjoch_git_sha1());
}

std::string DiffractionExperiment::CheckGitSha1Msg() const {
    if (internal.git_sha1() == jfjoch_git_sha1())
        return "";
    else {
        return "Local component git repo is rev. " + jfjoch_git_sha1().substr(0,6) +  " (" + jfjoch_git_date() +") remote component repo is rev. "
               + internal.git_sha1().substr(0,6) + " (" + internal.git_date() + ")";
    }
}

bool DiffractionExperiment::GetMaskModuleEdges() const {
    return internal.mask_module_edges();
}

bool DiffractionExperiment::GetMaskChipEdges() const {
    return internal.mask_chip_edges();
}

JFJochProtoBuf::UnitCell DiffractionExperiment::GetUnitCell() const {
    return dataset.unit_cell();
}

bool DiffractionExperiment::HasUnitCell() const {
    return dataset.has_unit_cell();
}

double DiffractionExperiment::ResToPxl(double resolution) const {
    if (resolution == 0)
        return INFINITY;

    double sin_theta = GetWavelength_A() / (2 * resolution);
    double theta = asin(sin_theta);
    double tan_2theta = tan(2 * theta);
    return tan_2theta * GetDetectorDistance_mm() / PIXEL_SIZE_IN_MM;
}

Coord DiffractionExperiment::LabCoord(double detector_x, double detector_y) const {
    // Assumes planar detector, 90 deg towards beam
    return {(detector_x - GetBeamX_pxl()) * PIXEL_SIZE_IN_MM ,
            (detector_y - GetBeamY_pxl()) * PIXEL_SIZE_IN_MM ,
            GetDetectorDistance_mm()};
}

double DiffractionExperiment::PxlToRes(double detector_x, double detector_y) const {
    auto lab = LabCoord(detector_x, detector_y);

    double beam_path = lab.Length();
    if (beam_path == GetDetectorDistance_mm()) return std::numeric_limits<double>::infinity();

    double cos_2theta = GetDetectorDistance_mm() / beam_path;
    // cos(2theta) = cos(theta)^2 - sin(theta)^2
    // cos(2theta) = 1 - 2*sin(theta)^2
    // Technically two solutions for two theta, but it makes sense only to take positive one in this case
    double sin_theta = sqrt((1-cos_2theta)/2);
    return GetWavelength_A() / (2 * sin_theta);
}

int64_t DiffractionExperiment::GetSpaceGroupNumber() const {
    return dataset.space_group_number();
}

int64_t DiffractionExperiment::GetStorageCellNumber() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            if (internal.storage_cells() > 1)
                return 2;
            else
                return 1;
        default:
            return internal.storage_cells();
    }
}

int64_t DiffractionExperiment::GetStorageCellStart() const {
    return internal.storage_cell_start();
}

double DiffractionExperiment::GetLowQForRadialInt_recipA() const {
    return internal.low_q();
}
double DiffractionExperiment::GetHighQForRadialInt_recipA() const {
    return internal.high_q();
}

double DiffractionExperiment::GetQSpacingForRadialInt_recipA() const {
    return internal.q_spacing();
}

JFJochProtoBuf::DetectorType DiffractionExperiment::GetDetectorType() const {
    return internal.type();
}

int64_t DiffractionExperiment::GetMaxSpotCount() const {
    return max_spot_count;
}

std::string DiffractionExperiment::GetSampleName() const {
    return dataset.sample_name();
}

// Create ProtoBuf structures

DiffractionExperiment::operator JFJochProtoBuf::DetectorInput() const {
    JFJochProtoBuf::DetectorInput ret;

    ret.set_modules_num(GetModulesNum());
    ret.set_mode(internal.mode());

    if (GetNumTriggers() == 1) {
        ret.set_num_frames(GetFrameNumPerTrigger() + DELAY_FRAMES_STOP_AND_QUIT);
        ret.set_num_triggers(1);
    } else {
        // More than 1 trigger - detector needs one trigger or few more trigger
        if (GetStorageCellNumber() > 1)
            ret.set_num_frames(1);
        else
            ret.set_num_frames(GetFrameNumPerTrigger());

        if (GetFrameNumPerTrigger() < DELAY_FRAMES_STOP_AND_QUIT)
            ret.set_num_triggers(GetNumTriggers() + DELAY_FRAMES_STOP_AND_QUIT);
        else
            ret.set_num_triggers(GetNumTriggers() + 1);
    }
    ret.set_storage_cell_start(GetStorageCellStart());
    ret.set_storage_cell_number(GetStorageCellNumber());
    ret.set_storage_cell_delay(5);

    if (GetStorageCellNumber() > 1) {
        ret.set_period_us((GetFrameTime().count() +10) * GetStorageCellNumber());
    } else
        ret.set_period_us(GetFrameTime().count());

    ret.set_count_time_us(GetFrameCountTime().count());

    return ret;
}

DiffractionExperiment::operator JFJochProtoBuf::IndexerInput() const {
    JFJochProtoBuf::IndexerInput ret;
    ret.set_bin_size(GetSpotFindingBin());

    if (HasUnitCell()) {
        *ret.mutable_unit_cell() = GetUnitCell();
        ret.set_space_group_number(GetSpaceGroupNumber());
    }

    return ret;
}

DiffractionExperiment::operator JFJochProtoBuf::WriterInput() const {
    JFJochProtoBuf::WriterInput ret;

    ret.set_file_prefix(GetFilePrefix());
    ret.set_file_count(GetDataFileCount());

    ret.set_width(GetXPixelsNum());
    ret.set_height(GetYPixelsNum());
    ret.set_pixel_depth_bytes(GetPixelDepth());
    ret.set_signed_pxl(IsPixelSigned());
    ret.set_compression(GetCompressionAlgorithm());
    ret.set_max_spot_count(GetMaxSpotCount());
    if (GetDetectorMode() == DetectorMode::Conversion)
        ret.set_image_units("photon");
    else
        ret.set_image_units("ADU");
    return ret;
}

JFJochProtoBuf::DetectorConfig DiffractionExperiment::DetectorConfig(const JFJochProtoBuf::ReceiverNetworkConfig &net_config) const {
    JFJochProtoBuf::DetectorConfig ret;
    if (net_config.fpga_mac_addr_size() < GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds,
                              "Number of FPGA boards in the receiver is less then necessary");
    int i = 0;
    for (int d = 0; d < GetDataStreamsNum(); d++) {
        for (int module = 0; module < GetModulesNum(d); module++) {
            auto mod_cfg = ret.add_modules();
            mod_cfg->set_udp_dest_port_1(GetDestUDPPort(d,module));
            mod_cfg->set_udp_dest_port_2(GetDestUDPPort(d,module) + 1);
            mod_cfg->set_ipv4_src_addr_1(IPv4AddressToStr(GetSrcIPv4Address(i * 2)));
            mod_cfg->set_ipv4_src_addr_2(IPv4AddressToStr(GetSrcIPv4Address(i * 2 + 1)));
            mod_cfg->set_ipv4_dest_addr_1(IPv4AddressToStr(GetDestIPv4Address(d)));
            mod_cfg->set_ipv4_dest_addr_2(IPv4AddressToStr(GetDestIPv4Address(d)));
            mod_cfg->set_mac_addr_dest_1(net_config.fpga_mac_addr(d));
            mod_cfg->set_mac_addr_dest_2(net_config.fpga_mac_addr(d));
            i++;
        }
    }
    return ret;
}

void DiffractionExperiment::FillWriterMetadata(JFJochProtoBuf::WriterMetadataInput &input) const {
    // general metadata
    input.set_image_number(GetImageNum());
    input.set_images_per_trigger(GetImageNumPerTrigger());
    input.set_file_prefix(GetFilePrefix());
    input.set_file_count(GetDataFileCount());

    input.mutable_beam_metadata()->set_energy_kev(GetPhotonEnergy_keV());

    auto det = input.mutable_detector_metadata();
    det->set_type(GetDetectorType());
    det->set_width_pxl(GetXPixelsNum());
    det->set_height_pxl(GetYPixelsNum());
    det->set_pixel_depth_byte(GetPixelDepth());
    det->set_signed_pxl(IsPixelSigned());
    det->set_compression(GetCompressionAlgorithm());
    det->set_image_time_us(GetImageTime().count());
    det->set_count_time_us(GetImageCountTime().count());
    det->set_overload(GetOverflow());
    if (IsPixelSigned())
        det->set_underload(GetUnderflow());
    det->set_ntrigger(GetNumTriggers());
    det->set_storage_cell_number(GetStorageCellNumber());

    det->set_sensor_material(SENSOR_MATERIAL);
    det->set_pixel_size_mm(PIXEL_SIZE_IN_MM);
    det->set_sensor_thickness_um(SENSOR_THICKNESS_IN_UM);

    auto geom = input.mutable_geometry_metadata();
    geom->set_beam_x_pxl(GetBeamX_pxl());
    geom->set_beam_y_pxl(GetBeamY_pxl());
    geom->set_distance_mm(GetDetectorDistance_mm());

    auto sample = input.mutable_sample_metadata();
    sample->set_name(GetSampleName());
    if (GetSpaceGroupNumber() > 0)
        sample->set_space_group(GetSpaceGroupNumber());
    if (HasUnitCell())
        *sample->mutable_unit_cell() = GetUnitCell();
}

void DiffractionExperiment::LoadDatasetSettings(const JFJochProtoBuf::DatasetSettings &settings) {
    // Save DatasetSettings - if something goes wrong, restore old settings
    auto tmp = dataset;

    try {
        dataset = JFJochProtoBuf::DatasetSettings();
        ImagesPerTrigger(settings.images_per_trigger());
        NumTriggers(settings.ntrigger());
        if (settings.has_image_time_us())
            ImageTimeUs(std::chrono::microseconds(settings.image_time_us()));
        else
            Summation(settings.summation());
        BeamX_pxl(settings.beam_x_pxl());
        BeamY_pxl(settings.beam_y_pxl());
        DetectorDistance_mm(settings.detector_distance_mm());
        PhotonEnergy_keV(settings.photon_energy_kev());
        FilePrefix(settings.file_prefix());
        DataFileCount(settings.data_file_count());
        if (settings.has_unit_cell())
            SetUnitCell(settings.unit_cell());
        else
            SetUnitCell();
        SpaceGroupNumber(settings.space_group_number());
        SampleName(settings.sample_name());
        if (settings.has_scattering_vector())
            ScatteringVector(settings.scattering_vector());
        Compression(settings.compression());

    } catch (...) {
        dataset = tmp;
        throw;
    }
}

void DiffractionExperiment::LoadDetectorSettings(const JFJochProtoBuf::DetectorSettings &settings) {
    auto tmp = internal;
    try {
        if (settings.has_count_time_us())
            FrameTime(std::chrono::microseconds(settings.frame_time_us()),
                      std::chrono::microseconds(settings.count_time_us()));
        else
            FrameTime(std::chrono::microseconds(settings.frame_time_us()));

        if (settings.use_storage_cells())
            StorageCells(16);
        else
            StorageCells(1);

        UseInternalPacketGenerator(settings.use_internal_packet_generator());

        if (settings.collect_raw_data())
            Mode(DetectorMode::Raw);
        else
            Mode(DetectorMode::Conversion);

        if (settings.has_pedestal_g0_frames())
            PedestalG0Frames(settings.pedestal_g0_frames());

        if (settings.has_pedestal_g1_frames())
            PedestalG1Frames(settings.pedestal_g1_frames());

        if (settings.has_pedestal_g2_frames())
            PedestalG2Frames(settings.pedestal_g2_frames());
    } catch (...) {
        internal = tmp;
        throw;
    }
}

JFJochProtoBuf::DetectorSettings DiffractionExperiment::GetDetectorSettings() const {
    JFJochProtoBuf::DetectorSettings ret;
    ret.set_frame_time_us(GetFrameTime().count());
    ret.set_count_time_us(GetFrameCountTime().count());
    ret.set_collect_raw_data(GetDetectorMode() != DetectorMode::Conversion);
    ret.set_use_internal_packet_generator(IsUsingInternalPacketGen());
    ret.set_use_storage_cells(GetStorageCellNumber() > 1);
    ret.set_pedestal_g0_frames(GetPedestalG0Frames());
    ret.set_pedestal_g1_frames(GetPedestalG1Frames());
    ret.set_pedestal_g2_frames(GetPedestalG2Frames());
    return ret;
}

void DiffractionExperiment::CheckDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings &settings) {
    check_min("Signal to noise threshold", settings.signal_to_noise_threshold(), 1);
    check_min("Photon count threshold", settings.photon_count_threshold(), 0);
    check_min("Minimum pixels per spot", settings.min_pix_per_spot(), 1);
    check_min("Maximum pixels per spot", settings.max_pix_per_spot(), settings.min_pix_per_spot() + 1);
    check_min("Local background size", settings.local_bkg_size(), 2);
    check_max("Local background size", settings.local_bkg_size(), 7);
    if (settings.has_high_resolution_limit()) {
        check_min("Spot finding high resolution limit", settings.high_resolution_limit(), 0.5);
        check_max("Spot finding high resolution limit", settings.high_resolution_limit(), 50.0);
        if (settings.has_low_resolution_limit()) {
            check_min("Spot finding low resolution limit", settings.low_resolution_limit(),
                      settings.high_resolution_limit());
        }
    } else if (settings.has_low_resolution_limit()) {
        check_min("Spot finding low resolution limit", settings.low_resolution_limit(), 1.0);
        check_max("Spot finding low resolution limit", settings.low_resolution_limit(), 50.0);
    }
    check_min("Background estimate lowQ", settings.bkg_estimate_low_q(), 0.0);
    check_min("Background estimate highQ", settings.bkg_estimate_high_q(), settings.bkg_estimate_low_q());

}

JFJochProtoBuf::DataProcessingSettings DiffractionExperiment::DefaultDataProcessingSettings() {
    JFJochProtoBuf::DataProcessingSettings ret;
    ret.set_local_bkg_size(5);
    ret.set_signal_to_noise_threshold(3);
    ret.set_photon_count_threshold(16);
    ret.set_min_pix_per_spot(1);
    ret.set_max_pix_per_spot(50);
    ret.set_bkg_estimate_low_q(2 * M_PI / 5.0);
    ret.set_bkg_estimate_high_q(2 * M_PI / 3.0);
    return ret;
}