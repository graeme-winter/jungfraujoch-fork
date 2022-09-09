// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <iomanip>

#include <bitshuffle/bitshuffle_core.h>
#include "../writer/HDF5Objects.h"
#include "../common/RawToConvertedGeometry.h"
#include "../common/SpotFinder.h"
#include "../indexing/XgandalfWrapper.h"
#include "../common/RadialIntegration.h"

void TestIndexing() {
    const int nexec_std  = 2;
    const int nexec_fast = 20;

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

    IndexingSettings settings;
    settings.algorithm = IndexingAlgorithm::Xgandalf_fast;
    settings.max_indexing_spots = 30;

    for (auto &c: cells) {
        settings.unit_cell = c;

        CrystalLattice l(c);
        CrystalLattice recip_l = l.ReciprocalLattice();

        std::vector<Coord> recip;
        recip.reserve(hkl.size());
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec[0] + i.y * recip_l.vec[1] + i.z * recip_l.vec[2]);

        DiffractionExperiment experiment;
        experiment.SetUnitCell(make_unit_cell(36.9, 78.95, 78.95, 90, 90, 90));

        XgandalfWrapper wrapper;
        settings.algorithm = IndexingAlgorithm::Xgandalf;
        settings.centering = 'P';

        auto start_time = std::chrono::system_clock::now();

        for (int i = 0; i < nexec_std; i++) {

            if (wrapper.Run(settings, recip).empty())
                std::cout << "WARNING! No solution found" << std::endl;
        }

        auto end_time = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "Xgandalf    (std.  ) " << std::setw(5) << (uint32_t)(elapsed.count() / 1000.0 / nexec_std) << " ms/image " << std::endl;

        settings.algorithm = IndexingAlgorithm::Xgandalf_fast;
        start_time = std::chrono::system_clock::now();

        for (int i = 0; i < nexec_fast; i++) {
            if (wrapper.Run(settings, recip).empty())
                std::cout << "WARNING! No solution found" << std::endl;
        }

        end_time = std::chrono::system_clock::now();

        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "Xgandalf    (fast  ) " << std::setw(5) << (uint32_t)std::lround(elapsed.count() / 1000.0 / nexec_fast) << " ms/image" << std::endl;
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
    strstream << std::setw(5) <<   std::lround(elapsed.count() / (1000.0 * (double) nimages)) <<  " ms/image  "
              << std::setw(7) <<  spots.size() << " spots";
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
            integration.Process(image + i * experiment.GetPixelsNum(), experiment.GetPixelsNum());
            //integration.GetResult(result);
        }
    }
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    return std::lround(elapsed.count() / (1000.0 * (double) nimages * nredo));
}

int main(int argc, char **argv) {
    RegisterHDF5Filter();

    if (argc != 2) {
        std::cout << "Usage: ./DataAnalysisPerfTest <JF4M hdf5 file>" << std::endl;
        exit(EXIT_FAILURE);
    }

    HDF5ReadOnlyFile data(argv[1] );

    HDF5DataSet dataset(data, "/entry/data/data");

    HDF5DataSpace file_space(dataset);

    if (file_space.GetNumOfDimensions() != 3) {
        std::cout << "/entry/data/data must be 3D" << std::endl;
        exit(EXIT_FAILURE);
    }

    DiffractionExperiment x;

    if ((file_space.GetDimensions()[1] == 2164) && (file_space.GetDimensions()[2] == 2068)) {
        std::cout << "JF4M with gaps detected (2068 x 2164)" << std::endl;
        x.DataStreamModuleSize(2, {8}, 8, 36);
    } else {
        std::cout << "Unknown geometry - exiting" << std::endl;
        exit(EXIT_FAILURE);
    }
    uint64_t nimages = file_space.GetDimensions()[0];
    uint64_t npixel = file_space.GetDimensions()[1] * file_space.GetDimensions()[2];

    std::cout << "Number of images in the dataset: " << nimages << std::endl;

    x.Mode(DetectorMode::Conversion);

    std::vector<int16_t> image_conv ( nimages * npixel);

    std::vector<hsize_t> start = {0,0,0};
    dataset.ReadVector(image_conv, start, file_space.GetDimensions());

    std::cout << "Images loaded" << std::endl;

    x.BeamX_pxl(1090).BeamY_pxl(1136).DetectorDistance_mm(75).OmegaStart(0)
            .OmegaIncrement(0.088).Wavelength_A(1.0);

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_enable_3d_spot_finding(false);
    settings.set_signal_to_noise_threshold(2.5);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_low_resolution_limit(80.0);
    settings.set_high_resolution_limit(2.0);
    settings.set_local_bkg_size(5);

#ifdef CUDA_SPOT_FINDING
    std::cout << "COLSPOT NBX=NBY=5" << std::endl;
    if (SpotFinder::GPUPresent()) {
        SpotFinder local_peakfinder_gpu(x);
        std::cout << "COLSPOT/GPU (JFjoch) " << std::setw(5) << TestSpotFinder(x, settings, local_peakfinder_gpu,
                                                                               image_conv.data(), nimages) << std::endl;
    }
    settings.set_local_bkg_size(3);
    std::cout << "COLSPOT NBX=NBY=3" << std::endl;
    if (SpotFinder::GPUPresent()) {
        SpotFinder local_peakfinder_gpu(x);
        std::cout << "COLSPOT/GPU (JFjoch) " << std::setw(5) << TestSpotFinder(x, settings, local_peakfinder_gpu,
                                                                               image_conv.data(), nimages) << std::endl;
    }
#endif

    std::cout << "Radial int. (JFjoch) " << std::setw(5) << TestRadialIntegration(x, settings,
                                                                                  image_conv.data(), nimages) << " ms/image" << std::endl;

    std::cout << std::endl;

    TestIndexing();
}
