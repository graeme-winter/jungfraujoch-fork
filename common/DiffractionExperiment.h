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
    JFJochProtoBuf::DatasetSettings dataset;
    JFJochProtoBuf::InternalSettings internal;

    void CalculateGeometry(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x, int64_t gap_y,
                           bool mirror_y_in_conversion);
    constexpr static const int64_t max_spot_count = 100;
public:
    // Public methods are atomic
    DiffractionExperiment();
    DiffractionExperiment(int64_t horizontal_stacking, const std::vector<int64_t> &v, int64_t gap_x = 0, int64_t gap_y = 0,
                          bool mirror_y_in_conversion = true);
    explicit DiffractionExperiment(const JFJochProtoBuf::JungfraujochSettings &settings);

    // Methods below can be chained together
    DiffractionExperiment& Mode(DetectorMode input);

    DiffractionExperiment& Import(const JFJochProtoBuf::JungfraujochSettings &settings);

    DiffractionExperiment& ImagesPerTrigger(int64_t input);
    DiffractionExperiment& NumTriggers(int64_t triggers);

    DiffractionExperiment& PedestalG0Frames(int64_t input);
    DiffractionExperiment& PedestalG1Frames(int64_t input);
    DiffractionExperiment& PedestalG2Frames(int64_t input);
    DiffractionExperiment& FrameTime(std::chrono::microseconds frame_time,
                                     std::chrono::microseconds in_count_time = std::chrono::microseconds(0));
    DiffractionExperiment& Summation(int64_t input);
    DiffractionExperiment& ImageTimeUs(std::chrono::microseconds image_time);

    DiffractionExperiment& PedestalG1G2FrameTime(std::chrono::microseconds input);
    DiffractionExperiment& PhotonEnergy_keV(double input);
    DiffractionExperiment& BeamX_pxl(double input);
    DiffractionExperiment& BeamY_pxl(double input);
    DiffractionExperiment& DetectorDistance_mm(double input);

    DiffractionExperiment& ScatteringVector(Coord input);
    DiffractionExperiment& ScatteringVector();
    DiffractionExperiment& FilePrefix(std::string input);
    DiffractionExperiment& DataFileCount(int64_t input);

    DiffractionExperiment& Compression(JFJochProtoBuf::Compression input);
    DiffractionExperiment& PreviewPeriod(std::chrono::microseconds input);
    DiffractionExperiment& SpotFindingPeriod(std::chrono::microseconds input);
    DiffractionExperiment& BackgroundEstimationPeriod(std::chrono::microseconds input);

    DiffractionExperiment& UseInternalPacketGenerator(bool input);
    DiffractionExperiment& BaseIPv4Address(std::string input);
    DiffractionExperiment& BaseUDPPort(int64_t input);
    DiffractionExperiment& BaseIPv4Address(int64_t input);
    DiffractionExperiment& MaskModuleEdges(bool input);
    DiffractionExperiment& MaskChipEdges(bool input);
    DiffractionExperiment& SetUnitCell(const JFJochProtoBuf::UnitCell &cell);
    DiffractionExperiment& SetUnitCell();

    DiffractionExperiment& LowResForRadialInt_A(double input);
    DiffractionExperiment& HighResForRadialInt_A(double input);
    DiffractionExperiment& LowQForRadialInt_recipA(double input);
    DiffractionExperiment& HighQForRadialInt_recipA(double input);
    DiffractionExperiment& QSpacingForRadialInt_recipA(double input);

    DiffractionExperiment& SpaceGroupNumber(int64_t input);
    DiffractionExperiment& StorageCells(int64_t input);
    DiffractionExperiment& StorageCellStart(int64_t input = 15);
    DiffractionExperiment& DetectorType(JFJochProtoBuf::DetectorType type);
    DiffractionExperiment& SampleName(std::string input);

    operator JFJochProtoBuf::JungfraujochSettings() const;
    operator JFJochProtoBuf::DetectorInput() const;
    operator JFJochProtoBuf::IndexerInput() const;
    operator JFJochProtoBuf::WriterInput() const;

    void FillWriterMetadata(JFJochProtoBuf::WriterMetadataInput& input) const;
    JFJochProtoBuf::DetectorConfig DetectorConfig(const JFJochProtoBuf::ReceiverNetworkConfig& net_config) const;
    void LoadDatasetSettings(const JFJochProtoBuf::DatasetSettings &settings);
    void LoadDetectorSettings(const JFJochProtoBuf::DetectorSettings &settings);
    JFJochProtoBuf::DetectorSettings GetDetectorSettings() const;

    static void CheckDataProcessingSettings(const JFJochProtoBuf::DataProcessingSettings& settings);
    static JFJochProtoBuf::DataProcessingSettings DefaultDataProcessingSettings();
    DetectorMode GetDetectorMode() const;

    int64_t GetPixelDepth() const;
    bool IsPixelSigned() const;
    int64_t GetOverflow() const;
    int64_t GetUnderflow() const;

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
    int64_t GetDataFileCount() const;

    JFJochProtoBuf::Compression GetCompressionAlgorithm() const;
    CompressionAlgorithm GetCompressionAlgorithmEnum() const;

    int64_t GetMaxCompressedSize() const;

    int64_t GetDataStreamsNum() const;
    int64_t GetModulesNum(uint16_t data_stream = TASK_NO_DATA_STREAM) const;
    int64_t GetFirstModuleOfDataStream(uint16_t data_stream) const;

    int64_t GetPixelsNum() const;
    int64_t GetYPixelsNum() const;
    int64_t GetXPixelsNum() const; // X pixels must be the same for all modules
    int64_t GetPixel0OfModule(uint16_t module) const;
    bool IsUpsideDown() const;

    std::chrono::microseconds GetPreviewPeriod() const;
    int64_t GetPreviewStride() const;
    std::chrono::microseconds GetSpotFindingPeriod() const;
    int64_t GetSpotFindingStride() const;
    std::chrono::microseconds GetBackgroundEstimationPeriod() const;
    int64_t GetBackgroundEstimationStride() const;

    int64_t GetSpotFindingBin() const;

    int64_t GetPreviewStride(std::chrono::microseconds period) const;
    bool IsUsingInternalPacketGen() const;

    int32_t GetSrcIPv4Address(uint16_t half_module) const;
    int32_t GetDestIPv4Address(uint16_t data_stream) const;
    uint16_t GetDestUDPPort(uint16_t data_stream, uint16_t module) const;

    bool CheckGitSha1Consistent() const;
    std::string CheckGitSha1Msg() const;

    bool GetMaskModuleEdges() const;
    bool GetMaskChipEdges() const;
    JFJochProtoBuf::UnitCell GetUnitCell() const;
    bool HasUnitCell() const;

    double ResToPxl(double resolution) const;
    Coord LabCoord(double detector_x, double detector_y) const;
    double PxlToRes(double detector_x, double detector_y) const;

    double GetLowQForRadialInt_recipA() const;
    double GetHighQForRadialInt_recipA() const;
    double GetQSpacingForRadialInt_recipA() const;

    int64_t GetSpaceGroupNumber() const;

    int64_t GetStorageCellNumber() const;
    int64_t GetStorageCellStart() const;

    JFJochProtoBuf::DetectorType GetDetectorType() const;
    int64_t GetMaxSpotCount() const;
    std::string GetSampleName() const;
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
