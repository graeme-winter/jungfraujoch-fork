// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIFFRACTIONEXPERIMENT_H
#define DIFFRACTIONEXPERIMENT_H

#include <chrono>
#include <exception>
#include <mutex>

#include <jfjoch.pb.h>

#include "../compression/CompressionAlgorithmEnum.h"

#include "Coord.h"
#include "Definitions.h"

enum class DetectorMode : int {
    Conversion, Raw, PedestalG0, PedestalG1, PedestalG2
};

inline JFJochProtoBuf::UnitCell make_unit_cell(double a, double b, double c, double alpha, double beta, double gamma) {
    JFJochProtoBuf::UnitCell ret;
    ret.set_a(a);
    ret.set_b(b);
    ret.set_c(c);
    ret.set_alpha(alpha);
    ret.set_beta(beta);
    ret.set_gamma(gamma);
    return ret;
}

class DiffractionExperiment {
    JFJochProtoBuf::DetectorSettings detector;
    JFJochProtoBuf::PreviewSettings preview;
    JFJochProtoBuf::PedestalSettings pedestal;
    JFJochProtoBuf::FrameCountSettings frame_count;
    JFJochProtoBuf::BeamlineMetadataSettings beamline_metadata;
    JFJochProtoBuf::DiffractionGeometrySettings diffraction_geometry;
    JFJochProtoBuf::TimingSettings timing;
    JFJochProtoBuf::ImageSavingSettings image_saving;
    JFJochProtoBuf::ConversionSettings conv;
    JFJochProtoBuf::CompressionSettings compr;
    JFJochProtoBuf::JungfraujochInternalSettings internal;
    JFJochProtoBuf::SampleSettings sample;
    JFJochProtoBuf::RadialIntegrationSettings radial_int;

    std::string GenerateFilePrefix() const;
    int64_t GetGapX() const;
    int64_t GetGapY() const;

    void UpdateDetectorSize();
    void UpdateGeometry();
    void UpdateModulePixel0();
    constexpr static const int64_t max_spot_count = 100;
public:
    constexpr static const int64_t RunNumberNotSet = -1;
    constexpr static const int64_t MaxRunNumber = 99999;
    // Public methods are atomic
    DiffractionExperiment();
    DiffractionExperiment(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x = 0, int64_t gap_y = 0 );
    explicit DiffractionExperiment(const JFJochProtoBuf::JungfraujochSettings &settings);

    // Methods below can be chained together
    DiffractionExperiment& UpsideDown(bool val);
    DiffractionExperiment& DataStreamModuleSize(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x = 0, int64_t gap_y = 0 );
    DiffractionExperiment& Mode(DetectorMode input);
    DiffractionExperiment& Mode_Text(std::string input);

    DiffractionExperiment& Import(const JFJochProtoBuf::JungfraujochSettings &settings);

    DiffractionExperiment& ImagesPerTrigger(int64_t input);
    DiffractionExperiment& NumTriggers(int64_t triggers);

    DiffractionExperiment& PedestalG0Frames(int64_t input);
    DiffractionExperiment& PedestalG1Frames(int64_t input);
    DiffractionExperiment& PedestalG2Frames(int64_t input);
    DiffractionExperiment& FrameTime(std::chrono::microseconds image_time,
                                     std::chrono::microseconds in_count_time = std::chrono::microseconds(0));
    DiffractionExperiment& Summation(int64_t input);
    DiffractionExperiment& TimeResolvedMode(bool input);

    DiffractionExperiment& PedestalG1G2FrameTime(std::chrono::microseconds input);
    DiffractionExperiment& PhotonEnergy_keV(double input);
    DiffractionExperiment& Wavelength_A(double input);
    DiffractionExperiment& BeamX_pxl(double input);
    DiffractionExperiment& BeamY_pxl(double input);
    DiffractionExperiment& DetectorDistance_mm(double input);

    DiffractionExperiment& ScatteringVector(Coord input);
    DiffractionExperiment& FilePrefix(std::string input);
    DiffractionExperiment& ImagesPerFile(int64_t input);
    DiffractionExperiment& Compression(JFJochProtoBuf::Compression input, int64_t level = 0);
    DiffractionExperiment& Compression_Text(std::string input);
    DiffractionExperiment& CompressionBlockSize(int64_t input);
    DiffractionExperiment& PreviewPeriod(std::chrono::microseconds input);
    DiffractionExperiment& SpotFindingPeriod(std::chrono::microseconds input);
    DiffractionExperiment& BackgroundEstimationPeriod(std::chrono::microseconds input);

    DiffractionExperiment& UseInternalPacketGenerator(bool input);
    DiffractionExperiment& DetectorName(std::string input);
    DiffractionExperiment& BaseIPv4Address(std::string input);
    DiffractionExperiment& BaseUDPPort(int64_t input);
    DiffractionExperiment& BaseIPv4Address(int64_t input);
    DiffractionExperiment& IncrementRunNumber();
    DiffractionExperiment& MaskModuleEdges(bool input);
    DiffractionExperiment& MaskChipEdges(bool input);
    DiffractionExperiment& SetUnitCell(const JFJochProtoBuf::UnitCell &cell);
    DiffractionExperiment& SetUnitCell();

    DiffractionExperiment& LowResForBkgEstimation_A(double input);
    DiffractionExperiment& HighResForBkgEstimation_A(double input);
    DiffractionExperiment& LowResForRadialInt_A(double input);
    DiffractionExperiment& HighResForRadialInt_A(double input);
    DiffractionExperiment& LowQForRadialInt_recipA(double input);
    DiffractionExperiment& HighQForRadialInt_recipA(double input);
    DiffractionExperiment& QSpacingForRadialInt_recipA(double input);

    DiffractionExperiment& SpaceGroup(const std::string &name);
    DiffractionExperiment& SpaceGroupNumber(int64_t input);
    DiffractionExperiment& RunNumber(int64_t input = RunNumberNotSet);
    DiffractionExperiment& StorageCells(int64_t input);
    DiffractionExperiment& StorageCellStart(int64_t input = 15);
    DiffractionExperiment& DetectorDelayAfterTrigger(std::chrono::microseconds input);
    DiffractionExperiment& DetectorType(JFJochProtoBuf::DetectorType type);
    DiffractionExperiment& SoftTrigger(bool input);

    operator JFJochProtoBuf::JungfraujochSettings() const;
    operator JFJochProtoBuf::DetectorInput() const;
    operator JFJochProtoBuf::IndexerInput() const;
    operator JFJochProtoBuf::WriterInput() const;

    void FillWriterMetadata(JFJochProtoBuf::WriterMetadataInput& input) const;
    JFJochProtoBuf::DetectorConfig DetectorConfig(const JFJochProtoBuf::ReceiverNetworkConfig& net_config) const;

    DetectorMode GetDetectorMode() const;
    std::string GetDetectorModeTxt() const;

    int64_t GetPixelDepth() const;
    bool IsPixelSigned() const;
    int64_t GetOverflow() const;
    int64_t GetUnderflow() const;
    static int64_t GetOverflow(int64_t summation);
    static int64_t GetUnderflow(int64_t summation);

    int64_t GetNumTriggers() const;
    int64_t GetImageNum() const;
    int64_t GetImageNumPerTrigger() const;

    int64_t GetPedestalG0Frames() const;
    int64_t GetPedestalG1Frames() const;
    int64_t GetPedestalG2Frames() const;

    int64_t GetFrameNum() const;
    int64_t GetFrameNumPerTrigger() const;

    std::chrono::microseconds GetFrameTime() const;
    std::chrono::microseconds GetImageTime() const;

    int64_t GetSummation() const;
    std::chrono::microseconds GetImageCountTime() const;
    std::chrono::microseconds GetFrameCountTime() const;

    double GetPhotonEnergy_keV() const;
    double GetWavelength_A() const;
    double GetBeamX_pxl() const;
    double GetBeamY_pxl() const;
    double GetDetectorDistance_mm() const;

    Coord GetScatteringVector() const;

    std::string GetFilePrefix() const;
    int64_t GetImagesPerFile() const;
    std::pair<int64_t,int64_t> GetImageLocationInFile(int64_t image_number) const; // file_number + image_number
    int64_t GetImagesInFile(int64_t file_num, int64_t actual_images = -1) const;
    int64_t GetFilesNum(int64_t actual_images = -1) const;

    JFJochProtoBuf::Compression GetCompressionAlgorithm() const;
    CompressionAlgorithm GetCompressionAlgorithmEnum() const;
    std::string GetCompressionAlgorithmText() const;
    int64_t GetCompressionBlockSize() const;
    int64_t GetCompressionLevel() const;

    int64_t GetMaxCompressedSizeGeneral(int64_t pixels_number, uint16_t pixel_depth) const;
    int64_t GetMaxCompressedSize() const;

    int64_t GetDataStreamsNum() const;
    int64_t GetModulesNum(uint16_t data_stream = TASK_NO_DATA_STREAM) const;
    int64_t GetFirstModuleOfDataStream(uint16_t data_stream) const;

    int64_t GetPixelsNum() const;
    int64_t GetYPixelsNum() const;
    int64_t GetXPixelsNum() const; // X pixels must be the same for all modules
    int64_t GetLine0OfModule(uint16_t module) const;
    int64_t GetCol0OfModule(uint16_t module) const;
    int64_t GetPixel0OfModule(uint16_t module) const;
    bool IsUpsideDown() const;
    int64_t GetHorizontalStacking() const;

    Coord GetModulePosition(uint16_t module) const;
    Coord GetModuleFastAxis(uint16_t module) const;
    Coord GetModuleSlowAxis(uint16_t module) const;

    std::string GenerateDataFilename(int64_t file_id) const;
    std::string GenerateMasterFilename() const;

    std::chrono::microseconds GetPreviewPeriod() const;
    int64_t GetPreviewStride() const;
    std::chrono::microseconds GetSpotFindingPeriod() const;
    int64_t GetSpotFindingStride() const;
    std::chrono::microseconds GetBackgroundEstimationPeriod() const;
    int64_t GetBackgroundEstimationStride() const;

    int64_t GetSpotFindingBin() const;

    int64_t GetPreviewStride(std::chrono::microseconds period) const;
    bool IsUsingInternalPacketGen() const;

    std::string GetDetectorName() const;

    static std::string IPv4AddressToStr(int32_t addr);
    static std::string MacAddressToStr(uint64_t addr);
    static uint64_t MacAddressFromStr(const std::string& addr);

    int32_t GetSrcIPv4Address(uint16_t half_module) const;
    int32_t GetDestIPv4Address(uint16_t data_stream) const;
    uint16_t GetDestUDPPort(uint16_t data_stream, uint16_t module) const;
    bool GetTimeResolvedMode() const;

    bool CheckGitSha1Consistent() const;
    std::string CheckGitSha1Msg() const;

    bool GetMaskModuleEdges() const;
    bool GetMaskChipEdges() const;
    JFJochProtoBuf::UnitCell GetUnitCell() const;
    bool HasUnitCell() const;

    double ResToPxl(double resolution) const;
    Coord LabCoord(double detector_x, double detector_y) const;
    double PxlToRes(double detector_x, double detector_y) const;

    double GetLowQLimitForBkg_recipA() const;
    double GetHighQLimitForBkg_recipA() const;

    double GetLowQForRadialInt_recipA() const;
    double GetHighQForRadialInt_recipA() const;
    double GetQSpacingForRadialInt_recipA() const;

    std::string GetSpaceGroupName() const;
    int64_t GetSpaceGroupNumber() const;
    char GetCentering() const;

    int64_t GetRunNumber() const;
    int64_t GetStorageCellNumber() const;
    int64_t GetStorageCellStart() const;

    std::chrono::microseconds GetDetectorDelayAfterTrigger() const;
    JFJochProtoBuf::DetectorType GetDetectorType() const;
    int64_t GetMaxSpotCount() const;
    bool GetSoftTrigger() const;
};

inline int64_t CalculateStride(const std::chrono::microseconds &frame_time, const std::chrono::microseconds &preview_time) {
    if ((preview_time.count() <= 0) || (frame_time.count() <= 0))
        return 0;
    else if (preview_time < frame_time)
        return 1;
    else
        return preview_time / frame_time;
}

#endif //DIFFRACTIONEXPERIMENT_H
