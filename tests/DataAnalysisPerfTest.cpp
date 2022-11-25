// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <iomanip>

#include <bitshuffle/bitshuffle_core.h>
#include "../writer/HDF5Objects.h"
#include "../common/RawToConvertedGeometry.h"
#include "../common/SpotFinder.h"
#include "../indexing/XgandalfWrapper.h"
#ifdef CUDA_SPOT_FINDING
#include "../indexing/FastFeedbackIndexerWrapper.h"
#endif
#include "../common/RadialIntegration.h"
#include "../common/Logger.h"

Logger logger{"DataAnalysisPerfTest"};

void TestIndexing() {
    constexpr const int nexec  = 5;

    std::vector<Coord> hkl;
    for (int i = 1; i < 7; i++)
        for (int j = 1; j<6; j++)
            for (int k = 1; k < 4; k++)
                hkl.emplace_back(i,j,k);

    std::vector<JFJochProtoBuf::UnitCell> cells;
    cells.emplace_back(make_unit_cell(30,40,50,90,90,90));
    cells.emplace_back(make_unit_cell(80,80,90,90,90,120));
    cells.emplace_back(make_unit_cell(40,45,80,90,82.5,90));
    cells.emplace_back(make_unit_cell(80,80,150,90,90,90));
    cells.emplace_back(make_unit_cell(30,40,50,90,90,90));

    std::map<std::string, std::unique_ptr<IndexerWrapper>> indexer;
    indexer["Xgandalf"] = std::make_unique<XgandalfWrapper>(false);
    indexer["Xgandalf_fast"] = std::make_unique<XgandalfWrapper>(true);
#ifdef CUDA_SPOT_FINDING
    indexer["fast_feedback"] = std::make_unique<FastFeedbackIndexerWrapper>();
#endif
    IndexingSettings settings;
    settings.max_indexing_spots = 30;
    settings.centering = 'P';

    for (auto &c: cells) {
        settings.unit_cell = c;

        CrystalLattice l(c);
        CrystalLattice recip_l = l.ReciprocalLattice();

        std::vector<Coord> recip;
        recip.reserve(hkl.size());
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec[0] + i.y * recip_l.vec[1] + i.z * recip_l.vec[2]);

        for (const auto& [x,y]: indexer) {
            auto start_time = std::chrono::system_clock::now();
            y->Setup(settings);
            for (int i = 0; i < nexec; i++) {
                if (y->Run(recip).empty())
                    logger.Warning("WARNING! No solution found");
            }

            auto end_time = std::chrono::system_clock::now();

            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            logger.Info("{:20s} {:8.1f} ms/image", x, elapsed.count() / (1000.0 * nexec));
        }
    }
}

#ifdef CUDA_SPOT_FINDING
auto TestSpotFinder(const DiffractionExperiment &experiment, const JFJochProtoBuf::DataProcessingSettings &settings,
                    SpotFinder &spot_finder, int16_t* image, size_t nimages) {

    std::vector<int16_t> spot_finder_buffer(experiment.GetPixelsNum());
    std::vector<DiffractionSpot> spots;

    spot_finder.SetInputBuffer(spot_finder_buffer.data());

    auto start_time = std::chrono::system_clock::now();
    for (int i = 0; i < nimages; i++) {
        memcpy(spot_finder_buffer.data(), image + i * experiment.GetPixelsNum(),
               experiment.GetPixelsNum() * sizeof(int16_t));
        spot_finder.RunSpotFinder(settings);
        spot_finder.GetResults(experiment, settings, spots, i);
    }

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::ostringstream strstream;
    logger.Info("{:20s} {:8.1f} ms/image   {:5d} spots", "Spot finding",
                elapsed.count() / (1000.0 * (double) nimages), spots.size());

    return strstream.str();
}
#endif

auto TestRadialIntegration(const DiffractionExperiment &experiment, const JFJochProtoBuf::DataProcessingSettings &settings,
                           int16_t* image, size_t nimages) {

    uint32_t nredo = 20;
    RadialIntegrationMapping mapping(experiment);
    RadialIntegration integration(mapping);
    std::vector<float> result;

    auto start_time = std::chrono::system_clock::now();
    for (int redo = 0; redo < nredo; redo++) {
        for (int i = 0; i < nimages; i++) {
            integration.ProcessOneImage(image + i * experiment.GetPixelsNum(), experiment.GetPixelsNum());
            //integration.GetResult(result);
        }
    }
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    return elapsed.count() / (1000.0 * (double) nimages * nredo);
}

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    if (argc != 2) {
        logger.Info("Usage: ./DataAnalysisPerfTest <JF4M hdf5 file>");
        exit(EXIT_FAILURE);
    }

    HDF5ReadOnlyFile data(argv[1] );

    HDF5DataSet dataset(data, "/entry/data/data");

    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        logger.Error("/entry/data/data must be 3D");
        exit(EXIT_FAILURE);
    }

    DiffractionExperiment x;

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        logger.Info("JF4M with gaps detected (2068 x 2164)");
        x.DataStreamModuleSize(2, {8}, 8, 36);
    } else {
        logger.Error("Unknown geometry - exiting");
        exit(EXIT_FAILURE);
    }
    uint64_t nimages = file_space.GetDimensions()[0];
    uint64_t npixel = file_space.GetDimensions()[1] * file_space.GetDimensions()[2];

    logger.Info("Number of images in the dataset: {}", nimages);

    x.Mode(DetectorMode::Conversion);

    std::vector<int16_t> image_conv ( nimages * npixel);

    std::vector<hsize_t> start = {0,0,0};
    dataset.ReadVector(image_conv, start, file_space.GetDimensions());

    logger.Info("Images loaded");

    x.BeamX_pxl(1090).BeamY_pxl(1136).DetectorDistance_mm(75).Wavelength_A(1.0);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_signal_to_noise_threshold(2.5);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.0);
    settings.set_local_bkg_size(5);

#ifdef CUDA_SPOT_FINDING
    logger.Info("COLSPOT NBX=NBY=5");
    if (SpotFinder::GPUPresent()) {
        SpotFinder local_peakfinder_gpu(x.GetXPixelsNum(), x.GetYPixelsNum());
        TestSpotFinder(x, settings, local_peakfinder_gpu,image_conv.data(), nimages);
    }
    settings.set_local_bkg_size(3);
    logger.Info("COLSPOT NBX=NBY=3");
    if (SpotFinder::GPUPresent()) {
        SpotFinder local_peakfinder_gpu(x.GetXPixelsNum(), x.GetYPixelsNum());
        TestSpotFinder(x, settings, local_peakfinder_gpu,image_conv.data(), nimages);
    }
#endif

    logger.Info("{:20s} {:8.1f} ms/image", "Radial int. (JFjoch)", TestRadialIntegration(x, settings,
                                                                                        image_conv.data(), nimages));

    TestIndexing();
}
