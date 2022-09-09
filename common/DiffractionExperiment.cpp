// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <numeric>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <zstd.h>
#include <arpa/inet.h>

#include <bitshuffle/bitshuffle.h>
#include "GitInfo.h"
#include "DiffractionExperiment.h"
#include "JFJochException.h"
#include <gemmi/symmetry.hpp>

DiffractionExperiment::DiffractionExperiment(const JFJochProtoBuf::JungfraujochSettings &settings) : DiffractionExperiment() {
    Import(settings);
}

DiffractionExperiment& DiffractionExperiment::Import(const JFJochProtoBuf::JungfraujochSettings &settings) {
    if (settings.has_internal())
        internal = settings.internal();
    if (settings.has_frame_count())
        frame_count = settings.frame_count();
    if (settings.has_detector())
        detector = settings.detector();
    if (settings.has_preview())
        preview = settings.preview();
    if (settings.has_pedestal())
        pedestal = settings.pedestal();
    if (settings.has_beamline_metadata())
        beamline_metadata = settings.beamline_metadata();
    if (settings.has_diffraction_geometry())
        diffraction_geometry = settings.diffraction_geometry();
    if (settings.has_timing())
        timing = settings.timing();
    if (settings.has_image_saving())
        image_saving = settings.image_saving();
    if (settings.has_conv())
        conv = settings.conv();
    if (settings.has_compr())
        compr = settings.compr();
    if (settings.has_sample())
        sample = settings.sample();
    if (settings.has_radial_int())
        radial_int = settings.radial_int();
    return *this;
}

DiffractionExperiment::operator JFJochProtoBuf::JungfraujochSettings() const {
    JFJochProtoBuf::JungfraujochSettings settings;
    *settings.mutable_frame_count() = frame_count;
    *settings.mutable_detector() = detector;
    *settings.mutable_preview() = preview;
    *settings.mutable_pedestal() = pedestal;
    *settings.mutable_beamline_metadata() = beamline_metadata;
    *settings.mutable_diffraction_geometry() = diffraction_geometry;
    *settings.mutable_timing() = timing;
    *settings.mutable_image_saving() = image_saving;
    *settings.mutable_conv() = conv;
    *settings.mutable_compr() = compr;
    *settings.mutable_internal() = internal;
    *settings.mutable_sample() = sample;
    *settings.mutable_radial_int() = radial_int;
    return settings;
}

#define check_max(param, val, max) if ((val) > (max)) throw JFJochException(JFJochExceptionCategory::InputParameterAboveMax, param)
#define check_min(param, val, min) if ((val) < (min)) throw JFJochException(JFJochExceptionCategory::InputParameterBelowMin, param)

DiffractionExperiment::DiffractionExperiment() {
    diffraction_geometry.set_photon_energy_kev(WVL_1A_IN_KEV);
    diffraction_geometry.set_detector_distance_mm(100);
    *diffraction_geometry.mutable_rotation_axis() = Coord(1,0,0);
    *diffraction_geometry.mutable_scattering_vector() = Coord(0,0,1);

    diffraction_geometry.mutable_data_stream_modules()->Add(4);
    diffraction_geometry.mutable_data_stream_modules()->Add(4);
    diffraction_geometry.set_horizontal_module_stacking(2);

    beamline_metadata.set_sample_temperature_k(-1);
    beamline_metadata.set_beam_size_x_um(-1);
    beamline_metadata.set_beam_size_y_um(-1);
    beamline_metadata.set_total_flux(-1);
    beamline_metadata.set_transmission(-1);

    beamline_metadata.set_detector_name(DETECTOR_NAME);
    beamline_metadata.set_source_name(SOURCE_NAME);
    beamline_metadata.set_source_name_short(SOURCE_NAME_SHORT);
    beamline_metadata.set_instrument_name(INSTRUMENT_NAME);
    beamline_metadata.set_instrument_name_short(INSTRUMENT_NAME_SHORT);

    compr.set_algorithm(JFJochProtoBuf::CompressionSettings_Compression_BSHUF_LZ4);
    compr.set_block_size(4096);

    image_saving.set_image_per_file(100);
    image_saving.set_file_prefix("test");
    image_saving.set_error_when_overwritting(false);

    timing.set_requested_image_time_us(MIN_FRAME_TIME_HALF_SPEED_IN_US);
    timing.set_frame_time_pedestalg1g2_us(FRAME_TIME_PEDE_G1G2_IN_US);
    timing.set_use_optimal_frame_time(true);
    timing.set_time_resolved_mode(false);

    conv.set_mask_chip_edges(false);
    conv.set_mask_module_edges(false);
    conv.set_upside_down(true);

    preview.set_preview_period_us(1000*1000); // 1s / 1 Hz
    preview.set_spot_finding_period_us(10*1000); // 10 ms / 100 Hz
    preview.set_bkg_estimation_period_us(10*1000); // 10 ms / 100 Hz

    radial_int.set_low_q(0.1);
    radial_int.set_high_q(5.0);
    radial_int.set_q_spacing(0.1);
    radial_int.set_bkg_estimate_low_q(2 * M_PI / 5.0);
    radial_int.set_bkg_estimate_high_q(2 * M_PI / 3.0);

    internal.set_base_ipv4_address(0x0132010a);
    internal.set_base_udp_port(8192);
    internal.set_git_sha1(jfjoch_git_sha1());
    internal.set_git_date(jfjoch_git_date());

    sample.set_run_number(RunNumberNotSet);
    sample.set_space_group_number(1); // P1

    detector.set_storage_cells(1);
    detector.set_storage_cell_start(15);
    detector.set_delay_after_trigger_us(0);
    UpdateGeometry();
}

DiffractionExperiment::DiffractionExperiment(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x,
                                             int64_t gap_y) : DiffractionExperiment() {
    DataStreamModuleSize(horizontal_stacking, v, gap_x, gap_y);
}

DiffractionExperiment &DiffractionExperiment::Mode(DetectorMode input) {
    switch (input) {
        case DetectorMode::Conversion:
            conv.set_mode(JFJochProtoBuf::CONVERSION);
            break;
        case DetectorMode::Raw:
            conv.set_mode(JFJochProtoBuf::RAW);
            break;
        case DetectorMode::PedestalG0:
            conv.set_mode(JFJochProtoBuf::PEDESTAL_G0);
            break;
        case DetectorMode::PedestalG1:
            conv.set_mode(JFJochProtoBuf::PEDESTAL_G1);
            break;
        case DetectorMode::PedestalG2:
            conv.set_mode(JFJochProtoBuf::PEDESTAL_G2);
            break;
    }
    UpdateGeometry();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ImagesPerTrigger(int64_t input) {
    check_min("Total number of images", input, 0);
    frame_count.set_images_per_trigger(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::NumTriggers(int64_t input) {
    check_max("Total number of triggers", input, INT32_MAX);
    check_min("Total number of triggers", input, 0);
    frame_count.set_ntrigger(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ImageTime(std::chrono::microseconds input) {
    check_min("Image time (us)", input.count(), MIN_FRAME_TIME_FULL_SPEED_IN_US);
    timing.set_requested_image_time_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::CountTime(std::chrono::microseconds input) {
    check_max("Count time (us)", input.count(), 1500);
    timing.set_count_time_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::OptimizeFrameTime(bool input) {
    timing.set_use_optimal_frame_time(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG1G2FrameTime(std::chrono::microseconds input) {
    check_min("Pedestal G1G2 frame time (us) ", input.count(), MIN_FRAME_TIME_FULL_SPEED_IN_US);
    timing.set_frame_time_pedestalg1g2_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG0Frames(int64_t input) {
    check_min("Pedestal G0 frames", input, 0);
    // Max?
    pedestal.set_g0_frames(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG1Frames(int64_t input) {
    check_min("Pedestal G1 frames", input, 0);
    pedestal.set_g1_frames(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalG2Frames(int64_t input) {
    check_min("Pedestal G2 frames", input, 0);
    pedestal.set_g2_frames(input);
    return *this;
}


DiffractionExperiment &DiffractionExperiment::PhotonEnergy_keV(double input) {
    check_min("Energy (keV)", input, MIN_ENERGY);
    check_max("Energy (keV)", input, MAX_ENERGY);
    diffraction_geometry.set_photon_energy_kev(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::Wavelength_A(double input) {
    PhotonEnergy_keV(WVL_1A_IN_KEV / input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamX_pxl(double input) {
    diffraction_geometry.set_beam_x_pxl(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamY_pxl(double input) {
    diffraction_geometry.set_beam_y_pxl(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DetectorDistance_mm(double input) {
    check_min("Detector distance (mm)", input, 0);
    diffraction_geometry.set_detector_distance_mm(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::Transmission(double input) {
    check_max("Transmission", input, 1.0);
    beamline_metadata.set_transmission(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::TotalFlux(double input) {
    beamline_metadata.set_total_flux(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::OmegaStart(double input) {
    diffraction_geometry.set_omega_start_deg(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::OmegaIncrement(double input) {
    diffraction_geometry.set_omega_angle_per_image_deg(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamSizeX_um(double input) {
    beamline_metadata.set_beam_size_x_um(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BeamSizeY_um(double input) {
    beamline_metadata.set_beam_size_y_um(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SampleTemperature(double input) {
    beamline_metadata.set_sample_temperature_k(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ScatteringVector(Coord input) {
    *diffraction_geometry.mutable_scattering_vector() = input.Normalize();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::RotationAxis(Coord input) {
    *diffraction_geometry.mutable_rotation_axis() = input.Normalize();
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
        image_saving.set_file_prefix(input.substr(0, input.length() - 10));
    else
        image_saving.set_file_prefix(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::TrackingID(std::string input) {
    image_saving.set_tracking_id(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ImagesPerFile(int64_t input) {
    check_min("Images per file", input, 0);
    image_saving.set_image_per_file(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::UseInternalPacketGenerator(bool input) {
    detector.set_internal_fpga_packet_generator(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DetectorName(std::string input) {
    beamline_metadata.set_detector_name(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SourceName(std::string input) {
    beamline_metadata.set_source_name(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SourceNameShort(std::string input) {
    beamline_metadata.set_source_name_short(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::InstrumentName(std::string input) {
    beamline_metadata.set_instrument_name(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::InstrumentNameShort(std::string input) {
    beamline_metadata.set_instrument_name_short(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::PedestalSaved(bool input) {
    pedestal.set_saved(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ForceFullSpeed(bool input) {
    detector.set_force_full_speed(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BaseIPv4Address(std::string input) {
    in_addr ipv4_addr{};
    if (inet_pton(AF_INET, input.c_str(), &ipv4_addr) != 1)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Need proper IPv4 address");
    BaseIPv4Address(ipv4_addr.s_addr); // IPv4 address is passed as low endian to FPGA
    return *this;
}

std::string DiffractionExperiment::IPv4AddressToStr(int32_t addr) {
    char tmp[256];
    if (inet_ntop(AF_INET, &addr, tmp, 255) != tmp)
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Error in exporting IPv4 address");
    return {tmp};
}

std::string DiffractionExperiment::MacAddressToStr(uint64_t addr) {
    uint64_t mac_address_network_order = addr;
    std::ostringstream ss;
    for (int i = 0; i < 6; i++) {
        ss << std::setfill('0') << std::setw(2) << std::hex << ((mac_address_network_order >> (i * 8)) & 0xFF);
        if (i != 5) ss << ":";
    }
    return ss.str();
}

uint64_t DiffractionExperiment::MacAddressFromStr(const std::string &addr) {
    uint64_t ret = 0;
    uint32_t values[6];
    char end_char;
    if( sscanf( addr.c_str(), "%x:%x:%x:%x:%x:%x%c",
                     &values[0], &values[1], &values[2],
                     &values[3], &values[4], &values[5], &end_char ) != 6 )
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "MAC address invalid");

    for (int i = 0; i < 6; i++) {
        if (values[i] > 255)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "MAC address out of range");
        ret |= (uint64_t)values[i] << (i*8);
    }
    return ret;
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

DiffractionExperiment &DiffractionExperiment::TimeResolvedMode(bool input) {
    timing.set_time_resolved_mode(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::IncrementMeasurementSequenceNumber() {
    auto tmp = (image_saving.measurement_sequence_num() + 1) % INT32_MAX;
    image_saving.set_measurement_sequence_num(tmp);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::MeasurementSequenceNumber(int64_t input) {
    check_min("Sequence number", input, 0);
    check_max("Sequence number", input, INT32_MAX-1);
    image_saving.set_measurement_sequence_num(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::ErrorWhenOverwriting(bool input) {
    image_saving.set_error_when_overwritting(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::MaskModuleEdges(bool input) {
    conv.set_mask_module_edges(input);
    return *this;
}

int64_t DiffractionExperiment::GetNumTriggers() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            if (GetTimeResolvedMode() && (frame_count.ntrigger() < 1))
                return 1;
            else
                return frame_count.ntrigger();
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
        default:
            return 0;
    }
}

DetectorMode DiffractionExperiment::GetDetectorMode() const {
    switch (conv.mode()) {
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

bool DiffractionExperiment::IsDetectorFullSpeed() const {
    // Either detector is forced to operate in full speed
    // or image time is so short, that it can be only satisfied in full speed mode
    return ((timing.requested_image_time_us() < MIN_FRAME_TIME_HALF_SPEED_IN_US) || detector.force_full_speed());
}

int64_t DiffractionExperiment::GetFrameToReqImageRatio() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return 1;
        case DetectorMode::PedestalG0:
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
        default:
            if (!timing.use_optimal_frame_time()) return 1;
            if (IsDetectorFullSpeed()) return (timing.requested_image_time_us() / MIN_FRAME_TIME_FULL_SPEED_IN_US);
            else return (timing.requested_image_time_us() / MIN_FRAME_TIME_HALF_SPEED_IN_US);
    }
}

int64_t DiffractionExperiment::GetSummation() const {
    if ((GetDetectorMode() == DetectorMode::Conversion) && (GetStorageCellNumber() == 1))
        // Only conversion mode allows for summation, but this option can be overridden by setting an option
        return GetFrameToReqImageRatio();
    else
        return 1;
}

std::chrono::microseconds DiffractionExperiment::GetFrameTime() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return std::chrono::microseconds(timing.frame_time_pedestalg1g2_us());
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
        case DetectorMode::PedestalG0:
        default:
            return std::chrono::microseconds(timing.requested_image_time_us() / GetFrameToReqImageRatio());
    }
}

std::chrono::microseconds DiffractionExperiment::GetImageTime() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return std::chrono::microseconds(timing.frame_time_pedestalg1g2_us());
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
        case DetectorMode::PedestalG0:
        default:
            return GetFrameTime() * GetSummation();
    }
}

int64_t DiffractionExperiment::GetImageNum() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            if (GetNumTriggers() == 0)
                return GetImageNumPerTrigger() ;
            else
                return GetImageNumPerTrigger() * GetNumTriggers();
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
        default:
            if (pedestal.saved()) return GetFrameNum();
                // No frames saved for pedestal runs
            else return 0;
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
            if (pedestal.saved())
                return GetFrameNumPerTrigger();
            else
                return 0;
        default:
            return 0;
    }
}

int64_t DiffractionExperiment::GetFrameNum() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
        {
            int64_t nframes = 0;
            if (frame_count.ntrigger() == 0)
                nframes += GetFrameNumPerTrigger();
            else {
                nframes += GetFrameNumPerTrigger() * frame_count.ntrigger();
            }
            return nframes;
        }
        case DetectorMode::PedestalG0:
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            return GetFrameNumPerTrigger();
        default:
            return 0;
    }
}

int64_t DiffractionExperiment::GetFrameNumPerTrigger() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
        case DetectorMode::Raw:
            if (GetStorageCellNumber() > 1)
                return GetStorageCellNumber();
            else
                return frame_count.images_per_trigger() * GetFrameToReqImageRatio();
        case DetectorMode::PedestalG0:
            return pedestal.g0_frames() * GetStorageCellNumber();
        case DetectorMode::PedestalG1:
            return pedestal.g1_frames() * GetStorageCellNumber();
        case DetectorMode::PedestalG2:
            return pedestal.g2_frames() * GetStorageCellNumber();
        default:
            return 0;
    }
}

std::chrono::microseconds DiffractionExperiment::GetFrameCountTime() const {
    if ((timing.count_time_us() > 0) && (timing.count_time_us() - GetFrameTime().count() < READOUT_TIME_IN_US))
        return std::chrono::microseconds(timing.count_time_us());
    else if (IsDetectorFullSpeed())
        return std::chrono::microseconds((timing.requested_image_time_us() /
                                          (timing.requested_image_time_us() / MIN_FRAME_TIME_FULL_SPEED_IN_US))
                                         - READOUT_TIME_IN_US);
    else
        return std::chrono::microseconds((timing.requested_image_time_us() /
                                          (timing.requested_image_time_us() / MIN_FRAME_TIME_HALF_SPEED_IN_US))
                                         - READOUT_TIME_IN_US);
}

std::chrono::microseconds DiffractionExperiment::GetImageCountTime() const {
    return GetFrameCountTime() * GetSummation();
}

int64_t DiffractionExperiment::GetPedestalG0Frames() const {
    return pedestal.g0_frames();
}

int64_t DiffractionExperiment::GetPedestalG1Frames() const {
    return pedestal.g1_frames();
}

int64_t DiffractionExperiment::GetPedestalG2Frames() const {
    return pedestal.g2_frames();
}

double DiffractionExperiment::GetPhotonEnergy_keV() const {
    return diffraction_geometry.photon_energy_kev();
}

double DiffractionExperiment::GetWavelength_A() const {
    return WVL_1A_IN_KEV / diffraction_geometry.photon_energy_kev();
}

double DiffractionExperiment::GetBeamX_pxl() const {
    return diffraction_geometry.beam_x_pxl();
}

double DiffractionExperiment::GetBeamY_pxl() const {
    return diffraction_geometry.beam_y_pxl();
}

double DiffractionExperiment::GetDetectorDistance_mm() const {
    return diffraction_geometry.detector_distance_mm();
}

double DiffractionExperiment::GetTransmission() const {
    return beamline_metadata.transmission();
}

double DiffractionExperiment::GetTotalFlux() const {
    return beamline_metadata.total_flux();
}

double DiffractionExperiment::GetOmegaStart() const {
    return diffraction_geometry.omega_start_deg();
}

double DiffractionExperiment::GetOmegaIncrement() const {
    return diffraction_geometry.omega_angle_per_image_deg();
}

double DiffractionExperiment::GetOmega(double image) const {
    return GetOmegaStart() + GetOmegaIncrement() * image;
}

double DiffractionExperiment::GetBeamSizeX_um() const {
    return beamline_metadata.beam_size_x_um();
}

double DiffractionExperiment::GetBeamSizeY_um() const {
    return beamline_metadata.beam_size_y_um();
}

std::vector<double> DiffractionExperiment::GetBeamSize() const {
    return {GetBeamSizeX_um(), GetBeamSizeY_um()};
}

double DiffractionExperiment::GetSampleTemperature() const {
    return beamline_metadata.sample_temperature_k();
}

Coord DiffractionExperiment::GetRotationAxis() const {
    return diffraction_geometry.rotation_axis();
}

Coord DiffractionExperiment::GetScatteringVector() const {
    return (Coord)diffraction_geometry.scattering_vector() * (diffraction_geometry.photon_energy_kev() / WVL_1A_IN_KEV);
}

std::string DiffractionExperiment::GetTrackingID() const {
    return image_saving.tracking_id();
}

std::string DiffractionExperiment::GetFilePrefix() const {
    return image_saving.file_prefix();
}

int64_t DiffractionExperiment::GetFileForImage(int64_t image_number) const {
    if (GetTimeResolvedMode())
        return image_number % GetImageNumPerTrigger();
    if (image_saving.image_per_file() == 0)
        return 0;
    else
        return image_number / GetImagesPerFile();
}

int64_t DiffractionExperiment::GetImagesPerFile() const {
    if (GetTimeResolvedMode())
        return GetNumTriggers();
    if (image_saving.image_per_file() == 0)
        return GetImageNum();
    else
        return image_saving.image_per_file();
}

CompressionAlgorithm DiffractionExperiment::GetCompressionAlgorithm() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
            switch (compr.algorithm()) {
                case JFJochProtoBuf::CompressionSettings_Compression_BSHUF_LZ4:
                    return CompressionAlgorithm::BSHUF_LZ4;
                case JFJochProtoBuf::CompressionSettings_Compression_BSHUF_ZSTD:
                    return CompressionAlgorithm::BSHUF_ZSTD;
                default:
                case JFJochProtoBuf::CompressionSettings_Compression_NONE:
                    return CompressionAlgorithm::None;
            }
        default:
            // Compression not supported for raw data and pedestal
            return CompressionAlgorithm::None;
    }
}

std::string DiffractionExperiment::GetCompressionAlgorithmText() const {
    std::string algorithm;
    switch (GetCompressionAlgorithm()) {
        case CompressionAlgorithm::BSHUF_LZ4:
            algorithm = "bslz4";
            if (compr.level() != 0)
                algorithm += ":" + std::to_string(compr.level());
            break;
        case CompressionAlgorithm::BSHUF_ZSTD:
            algorithm = "bszstd";
            if (compr.level() == ZSTD_USE_JFJOCH_RLE)
                algorithm += "_rle";
            else if (compr.level() != 0)
                algorithm += ":" + std::to_string(compr.level());
            break;
        case CompressionAlgorithm::None:
        default:
            algorithm = "off";
            break;

    }
    return algorithm;
}

int64_t DiffractionExperiment::GetCompressionBlockSize() const {
    return compr.block_size();
}

int64_t DiffractionExperiment::GetCompressionLevel() const {
    return compr.level();
}

DiffractionExperiment &DiffractionExperiment::Compression_Text(std::string input) {
    auto colon_position = input.find(':');
    std::string prefix = input.substr(0, colon_position);
    int64_t in_compression_level = 0;
    if (colon_position != std::string::npos)
        in_compression_level = std::stoi(input.substr(colon_position + 1));

    if ((prefix == "off") || (prefix == "none"))
        Compression(CompressionAlgorithm::None, 0);
    else if (prefix == "bslz4") // Allows for shorter names, used by Dectris EIGER
        Compression(CompressionAlgorithm::BSHUF_LZ4, in_compression_level);
    else if (prefix == "bszstd")
        Compression(CompressionAlgorithm::BSHUF_ZSTD, in_compression_level);
    else if (prefix == "bszstd_rle")
        Compression(CompressionAlgorithm::BSHUF_ZSTD, ZSTD_USE_JFJOCH_RLE);
    else throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                               "Compression algorithm not supported");
    return *this;
}

DiffractionExperiment &DiffractionExperiment::Compression(CompressionAlgorithm input, int64_t in_compression_level) {
    switch (input) {
        case CompressionAlgorithm::BSHUF_ZSTD:
            if (in_compression_level != ZSTD_USE_JFJOCH_RLE) {
                check_min("Zstd compression level", in_compression_level, ZSTD_minCLevel());
                check_max("Zstd compression level", in_compression_level, ZSTD_maxCLevel());
            }
            compr.set_algorithm(JFJochProtoBuf::CompressionSettings_Compression_BSHUF_ZSTD);
            compr.set_level(in_compression_level);
            break;
        case CompressionAlgorithm::BSHUF_LZ4:
            compr.set_level(in_compression_level);
            compr.set_algorithm(JFJochProtoBuf::CompressionSettings_Compression_BSHUF_LZ4);
            break;
        default:
            compr.set_algorithm(JFJochProtoBuf::CompressionSettings_Compression_NONE);
            compr.set_level(0);
            break;
    }
    return *this;
}

DiffractionExperiment &DiffractionExperiment::CompressionBlockSize(int64_t in_block_size) {
    check_min("Compression block size", in_block_size, 8);

    if (in_block_size % 8 > 0)
        throw JFJochException(JFJochExceptionCategory::Compression,
                              "Bitshuffle block size must be multiple of 8");

    compr.set_block_size(in_block_size);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::MaskChipEdges(bool input) {
    conv.set_mask_chip_edges(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::Mode_Text(std::string input) {
    if (input == "pedestalG0")
        return Mode(DetectorMode::PedestalG0);
    else if (input == "pedestalG1")
        return Mode(DetectorMode::PedestalG1);
    else if (input == "pedestalG2")
        return Mode(DetectorMode::PedestalG2);
    else if (input == "raw")
        return Mode(DetectorMode::Raw);
    else if (input == "conversion")
        return Mode(DetectorMode::Conversion);
    else
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Operation mode not supported");
}

std::string DiffractionExperiment::GetDetectorModeTxt() const {
    switch (GetDetectorMode()) {
        case DetectorMode::Raw:
            return "raw";
        case DetectorMode::Conversion:
            return "conversion";
        case DetectorMode::PedestalG0:
            return "pedestalG0";
        case DetectorMode::PedestalG1:
            return "pedestalG1";
        case DetectorMode::PedestalG2:
            return "pedestalG2";
        default:
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,"Mode unknown");
    }
}


DiffractionExperiment &DiffractionExperiment::UpsideDown(bool val) {
    conv.set_upside_down(val);
    UpdateGeometry();
    return *this;
}

DiffractionExperiment &DiffractionExperiment::DataStreamModuleSize(int64_t horizontal_stacking, const std::vector<int64_t> &vec, int64_t in_gap_x, int64_t in_gap_y) {
    check_min("Gap X",in_gap_x, 0);
    check_min("Gap Y",in_gap_y, 0);
    check_min("Horizontal module stacking",horizontal_stacking, 1);

    if (vec.empty())
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "At least one data stream with one module needs to be defined");
    if ((vec.size() > 1) && (in_gap_y % 2 == 1))
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Horizontal gap must be even number of pixels");
    for (const auto& val: vec) {
        if (val == 0)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Cannot define data stream with zero modules");
        if (val % horizontal_stacking != 0)
            throw JFJochException(JFJochExceptionCategory::InputParameterInvalid, "Number of modules needs to be multiply of horizontal stacking");
        // check_max("Number of modules", val, MAX_MODULES_FPGA);
    }

    diffraction_geometry.set_gap_x_pxl(in_gap_x);
    diffraction_geometry.set_gap_y_pxl(in_gap_y);
    diffraction_geometry.set_horizontal_module_stacking(horizontal_stacking);

    *diffraction_geometry.mutable_data_stream_modules() = {vec.begin(), vec.end()};
    UpdateGeometry();
    return *this;
}

void DiffractionExperiment::UpdateModulePixel0() {
    diffraction_geometry.clear_first_pixel_of_module();
    for (int module = 0; module < GetModulesNum(); module++) {
        switch (GetDetectorMode()) {
            case DetectorMode::Conversion: {
                int64_t module_x = module % GetHorizontalStacking();
                int64_t module_y = module / GetHorizontalStacking();

                int64_t pixel_x = module_x * (CONVERTED_MODULE_COLS + GetGapX());
                int64_t pixel_y = module_y * (CONVERTED_MODULE_LINES + GetGapY());

                if (conv.upside_down())
                    pixel_y = GetYPixelsNum() - pixel_y - 1;

                diffraction_geometry.add_first_pixel_of_module(pixel_y * GetXPixelsNum() + pixel_x);
            }
                break;
            default:
                diffraction_geometry.add_first_pixel_of_module( module * RAW_MODULE_SIZE);
                break;
        }
    }
}

void DiffractionExperiment::UpdateDetectorSize() {
    switch (GetDetectorMode()) {
        case DetectorMode::Conversion:
            diffraction_geometry.set_detector_size_x_pxl(GetHorizontalStacking() * CONVERTED_MODULE_COLS
                                                         +  (GetHorizontalStacking() - 1) * GetGapX());
            diffraction_geometry.set_detector_size_y_pxl((GetModulesNum() / GetHorizontalStacking()) * CONVERTED_MODULE_LINES
                                                         + (GetModulesNum() / GetHorizontalStacking() - 1) * GetGapY());
            break;
        default:
            diffraction_geometry.set_detector_size_x_pxl(RAW_MODULE_COLS);
            diffraction_geometry.set_detector_size_y_pxl(GetModulesNum() * RAW_MODULE_LINES);
            break;
    }
}

void DiffractionExperiment::UpdateGeometry() {
    UpdateDetectorSize();
    UpdateModulePixel0();
}

int64_t DiffractionExperiment::GetPixelDepth() const {
    if (GetSummation() == 1) return 2;
    else return 4;
}

int64_t DiffractionExperiment::GetDataStreamsNum() const {
    return diffraction_geometry.data_stream_modules_size();
}

int64_t DiffractionExperiment::GetModulesNum(uint16_t data_stream) const {
    if (data_stream == TASK_NO_DATA_STREAM)
        return std::accumulate(diffraction_geometry.data_stream_modules().begin(), diffraction_geometry.data_stream_modules().end(), 0);
    if (data_stream >= GetDataStreamsNum())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Non exisiting data stream");
    return diffraction_geometry.data_stream_modules(data_stream);
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

int64_t DiffractionExperiment::GetMaxCompressedSizeGeneral(int64_t pixels_number, uint16_t pixel_depth) const {
    switch (GetCompressionAlgorithm()) {
        case CompressionAlgorithm::BSHUF_LZ4:
            return bshuf_compress_lz4_bound(pixels_number, pixel_depth,
                                            GetCompressionBlockSize()) + 12;
        case CompressionAlgorithm::BSHUF_ZSTD:
            return bshuf_compress_zstd_bound(pixels_number, pixel_depth,
                                             GetCompressionBlockSize()) + 12;
        default:
            return pixels_number * pixel_depth;
    }
}

int64_t DiffractionExperiment::GetMaxCompressedSize() const {
    return GetMaxCompressedSizeGeneral(GetPixelsNum(), GetPixelDepth());
}

int64_t DiffractionExperiment::GetPixelsNum() const {
    return GetXPixelsNum() * GetYPixelsNum();
}

int64_t DiffractionExperiment::GetXPixelsNum() const {
    return diffraction_geometry.detector_size_x_pxl();
}

int64_t DiffractionExperiment::GetYPixelsNum() const {
    return diffraction_geometry.detector_size_y_pxl();
}

int64_t DiffractionExperiment::GetCol0OfModule(uint16_t module) const {
    return GetPixel0OfModule(module) % GetXPixelsNum();
}

int64_t DiffractionExperiment::GetLine0OfModule(uint16_t module) const {
    return GetPixel0OfModule(module) / GetXPixelsNum();
}

int64_t DiffractionExperiment::GetPixel0OfModule(uint16_t module) const {
    if (module >= diffraction_geometry.first_pixel_of_module_size())
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Module number out of bounds");

    return diffraction_geometry.first_pixel_of_module(module);
}

int64_t DiffractionExperiment::GetFilesNum(int64_t actual_images) const {
    if (actual_images < 0)
        actual_images = GetImageNum();
    if (GetTimeResolvedMode())
        return GetImageNumPerTrigger();
    if (GetImagesPerFile() == 0)
        return 1;
    else
        return actual_images / GetImagesPerFile() + (actual_images % GetImagesPerFile() != 0);
}

int64_t DiffractionExperiment::GetImagesInFile(int64_t file_num, int64_t actual_images) const {
    if (actual_images < 0)
        actual_images = GetImageNum();
    if ((file_num < 0) || (file_num >= GetFilesNum(actual_images)))
        throw JFJochException(JFJochExceptionCategory::ArrayOutOfBounds, "Invalid file number");
    if (GetTimeResolvedMode())
        return actual_images / GetFilesNum();
    if (file_num < GetFilesNum(actual_images) - 1)
        return GetImagesPerFile();
    else
        return actual_images - (GetFilesNum(actual_images) - 1) * GetImagesPerFile();
}

double DiffractionExperiment::GetRotationSpeed() const {
    return GetOmegaIncrement() / (GetImageTime().count() * 1e-6);
}

std::string DiffractionExperiment::GenerateFilePrefix() const {
    //Private, no need to lock
    std::string filename;
    filename += image_saving.file_prefix();

    if (GetRunNumber() != RunNumberNotSet) {
        char tmp[64];
        snprintf(tmp, 63, "_%05ld", GetRunNumber());
        filename += std::string(tmp);
    }

    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG0:
            filename += "_pedestalG0";
            break;
        case DetectorMode::PedestalG1:
            filename += "_pedestalG1";
            break;
        case DetectorMode::PedestalG2:
            filename += "_pedestalG2";
            break;
        default:
            break;
    }
    return filename;
}

std::string DiffractionExperiment::GenerateMasterFilename() const {
    return GenerateFilePrefix() + "_master.h5";
}

bool DiffractionExperiment::IsUpsideDown() const {
    if (GetDetectorMode() == DetectorMode::Conversion)
        return conv.upside_down();
    else
        return false;
}

int64_t DiffractionExperiment::GetOverflow() const {
    return GetOverflow(GetSummation());
}

int64_t DiffractionExperiment::GetUnderflow() const {
    return GetUnderflow(GetSummation());
}

int64_t DiffractionExperiment::GetOverflow(int64_t in_summation) {
    int64_t summation_rnd_pow2 = std::pow(2, std::ceil(std::log2(in_summation)));
    return  summation_rnd_pow2 * INT16_MAX - 4L;
}

int64_t DiffractionExperiment::GetUnderflow(int64_t in_summation) {
    int64_t summation_rnd_pow2 = std::pow(2, std::ceil(std::log2(in_summation)));
    return summation_rnd_pow2 * INT16_MIN + 4L;
}

std::string DiffractionExperiment::GenerateDataFilename(int64_t file_id) const {

    std::string ret = GenerateFilePrefix() + "_data";
    char buff[32];
    if (GetImagesPerFile() != 0) {
        snprintf(buff,31,"_%06ld", file_id+1);
        ret += std::string(buff);
    }
    return ret + ".h5";
}

Coord DiffractionExperiment::GetModulePosition(uint16_t module) const {
    // This is 4M specific
    return {double(module % GetHorizontalStacking()) * (CONVERTED_MODULE_COLS + GetGapX()),
             double(module / GetHorizontalStacking()) * (CONVERTED_MODULE_LINES + GetGapY()),
                 0};
}

Coord DiffractionExperiment::GetModuleFastAxis(uint16_t module) const {
    return Coord{-1,0,0};
}

Coord DiffractionExperiment::GetModuleSlowAxis(uint16_t module) const {
    return Coord{0,-1,0};
}

int64_t DiffractionExperiment::GetHorizontalStacking() const {
    return diffraction_geometry.horizontal_module_stacking();
}

int64_t DiffractionExperiment::GetGapX() const {
    return diffraction_geometry.gap_x_pxl();
}

int64_t DiffractionExperiment::GetGapY() const {
    return diffraction_geometry.gap_y_pxl();
}

DiffractionExperiment &DiffractionExperiment::PreviewPeriod(std::chrono::microseconds input) {
    check_min("Preview image generation period", input.count(), 0);
    preview.set_preview_period_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SpotFindingPeriod(std::chrono::microseconds input) {
    check_min("Spot finding image generation period", input.count(), 0);
    preview.set_spot_finding_period_us(input.count());
    return *this;
}

DiffractionExperiment &DiffractionExperiment::BackgroundEstimationPeriod(std::chrono::microseconds input) {
    check_min("Background estimation generation period", input.count(), 0);
    preview.set_bkg_estimation_period_us(input.count());
    return *this;
}

std::chrono::microseconds DiffractionExperiment::GetPreviewPeriod() const {
    return std::chrono::microseconds(preview.preview_period_us());
}

std::chrono::microseconds DiffractionExperiment::GetSpotFindingPeriod() const {
    return std::chrono::microseconds(preview.spot_finding_period_us());
}

std::chrono::microseconds DiffractionExperiment::GetBackgroundEstimationPeriod() const {
    return std::chrono::microseconds(preview.bkg_estimation_period_us());
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
    return detector.internal_fpga_packet_generator();
}

std::string DiffractionExperiment::GetDetectorName() const {
    return beamline_metadata.detector_name();
}

std::string DiffractionExperiment::GetInstrumentName() const {
    return beamline_metadata.instrument_name();
}

std::string DiffractionExperiment::GetInstrumentNameShort() const {
    return beamline_metadata.instrument_name_short();
}

std::string DiffractionExperiment::GetSourceName() const {
    return beamline_metadata.source_name();
}

std::string DiffractionExperiment::GetSourceNameShort() const {
    return beamline_metadata.source_name_short();
}

bool DiffractionExperiment::IsPedestalSaved() const {
    return pedestal.saved();
}

bool DiffractionExperiment::IsPedestalChanging() const {
    return (GetDetectorMode() == DetectorMode::PedestalG0)
           || (GetDetectorMode() == DetectorMode::PedestalG1)
           || (GetDetectorMode() == DetectorMode::PedestalG2);
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

bool DiffractionExperiment::GetTimeResolvedMode() const {
    return (GetStorageCellNumber() > 1) || timing.time_resolved_mode();
}

int64_t DiffractionExperiment::GetMeasurementSequenceNumber() const {
    return image_saving.measurement_sequence_num();
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

bool DiffractionExperiment::GetErrorWhenOverwriting() const {
    return image_saving.error_when_overwritting();
}

bool DiffractionExperiment::GetMaskModuleEdges() const {
    return conv.mask_module_edges();
}

bool DiffractionExperiment::GetMaskChipEdges() const {
    return conv.mask_chip_edges();
}

DiffractionExperiment &DiffractionExperiment::SetUnitCell(const JFJochProtoBuf::UnitCell &cell) {
    *sample.mutable_unit_cell() = cell;
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SetUnitCell() {
    sample.clear_unit_cell();
    return *this;
}

JFJochProtoBuf::UnitCell DiffractionExperiment::GetUnitCell() const {
    return sample.unit_cell();
}

bool DiffractionExperiment::HasUnitCell() const {
    return sample.has_unit_cell();
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

DiffractionExperiment &DiffractionExperiment::SampleName(const std::string &input) {
    sample.set_name(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SpaceGroupNumber(int64_t input) {
    try {
        auto sg = gemmi::get_spacegroup_by_number(input);
        sample.set_space_group_number(input);
    } catch (...) {
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Invalid space group number " + std::to_string(input));
    }
    return *this;
}

DiffractionExperiment &DiffractionExperiment::SpaceGroup(const std::string &input) {
    try {
        auto sg = gemmi::get_spacegroup_by_name(input);
        sample.set_space_group_number(sg.number);
    } catch (...) {
        throw JFJochException(JFJochExceptionCategory::InputParameterInvalid,
                              "Invalid space group number " + input);
    }
    return *this;
}

std::string DiffractionExperiment::GetSampleName() const {
    return sample.name();
}

int64_t DiffractionExperiment::GetSpaceGroupNumber() const {
    return sample.space_group_number();
}

std::string DiffractionExperiment::GetSpaceGroupName() const {
    auto sg = gemmi::get_spacegroup_by_number(GetSpaceGroupNumber());
    return sg.short_name();
}

char DiffractionExperiment::GetCentering() const {
    auto sg = gemmi::get_spacegroup_by_number(GetSpaceGroupNumber());
    return sg.centring_type();
}

DiffractionExperiment &DiffractionExperiment::RunNumber(int64_t input) {
    check_min("Run number", input, -1); // RunNumberNotSet
    check_max("Run number", input, 99999); // Using 5 digits to save run number
    sample.set_run_number(input);
    return *this;
}

int64_t DiffractionExperiment::GetRunNumber() const {
    return sample.run_number();
}

DiffractionExperiment &DiffractionExperiment::StorageCells(int64_t input) {
    check_min("Storage cell number", input, 1);
    check_max("Storage cell number", input, 16);
    detector.set_storage_cells(input);
    return *this;
}

DiffractionExperiment &DiffractionExperiment::StorageCellStart(int64_t input) {
    check_min("Start storage cell", input, 0);
    check_max("Start storage cell", input, 15);
    detector.set_storage_cell_start(input);
    return *this;
}
int64_t DiffractionExperiment::GetStorageCellNumber() const {
    switch (GetDetectorMode()) {
        case DetectorMode::PedestalG1:
        case DetectorMode::PedestalG2:
            if (detector.storage_cells() > 1)
                return 2;
            else
                return 1;
        default:
            return detector.storage_cells();
    }
}

int64_t DiffractionExperiment::GetStorageCellStart() const {
    return detector.storage_cell_start();
}

DiffractionExperiment &DiffractionExperiment::DetectorDelayAfterTrigger(std::chrono::microseconds input) {
    check_min("Input delay (us)", input.count(), 0);
    detector.set_delay_after_trigger_us(input.count());
    return *this;
}

std::chrono::microseconds DiffractionExperiment::GetDetectorDelayAfterTrigger() const {
    return std::chrono::microseconds(detector.delay_after_trigger_us());
}

double DiffractionExperiment::GetLowQLimitForBkg_recipA() const {
    return radial_int.bkg_estimate_low_q();
}

double DiffractionExperiment::GetHighQLimitForBkg_recipA() const {
    return radial_int.bkg_estimate_high_q();
}

double DiffractionExperiment::GetLowQForRadialInt_recipA() const {
    return radial_int.low_q();
}
double DiffractionExperiment::GetHighQForRadialInt_recipA() const {
    return radial_int.high_q();
}

double DiffractionExperiment::GetQSpacingForRadialInt_recipA() const {
    return radial_int.q_spacing();
}

DiffractionExperiment& DiffractionExperiment::LowResForBkgEstimation_A(double input) {
    check_min("Low Resolution for background estimation", input, 0.1);
    check_max("Low Resolution for background estimation", input, 500.0);
    radial_int.set_bkg_estimate_low_q(2 * M_PI / input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::HighResForBkgEstimation_A(double input) {
    check_min("High Resolution for background estimation", input, 0.1);
    check_max("High Resolution for background estimation", input, 500.0);
    radial_int.set_bkg_estimate_high_q(2 * M_PI / input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::LowResForRadialInt_A(double input) {
    check_min("Low Resolution for radial integration", input, 0.1);
    check_max("Low Resolution for radial integration", input, 500.0);
    radial_int.set_low_q(2 * M_PI / input);
    return *this;
}
DiffractionExperiment& DiffractionExperiment::HighResForRadialInt_A(double input) {
    check_min("High Resolution for radial integration", input, 0.1);
    check_max("High Resolution for radial integration", input, 500.0);
    radial_int.set_high_q(2 * M_PI / input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::LowQForRadialInt_recipA(double input) {
    check_min("Low Q for radial integration", input, 0.001);
    check_max("Low Q for radial integration", input, 10.0);
    radial_int.set_low_q(input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::HighQForRadialInt_recipA(double input) {
    check_min("High Q for radial integration", input, 0.001);
    check_max("High Q for radial integration", input, 10.0);
    radial_int.set_high_q(input);
    return *this;
}

DiffractionExperiment& DiffractionExperiment::QSpacingForRadialInt_recipA(double input) {
    check_min("Q spacing for radial integration", input, 0.01);
    radial_int.set_q_spacing(input);
    return *this;
}