// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../indexing/XgandalfWrapper.h"
#include "../indexing/JFJochIndexerService.h"
#include "../writer/HDF5Objects.h"
#include "../common/ZMQSpotPublisher.h"

TEST_CASE("Xgandalf","[Indexing]") {
    std::vector<Coord> hkl;
    for (int i = 1; i < 7; i++)
        for (int j = 1; j<6; j++)
            for (int k = 1; k < 4; k++)
                hkl.emplace_back(i,j,k);

    std::vector<UnitCell> cells;
    cells.emplace_back(30,40,50,90,90,90);
    cells.emplace_back(80,80,90,90,90,120);
    cells.emplace_back(40,45,80,90,82.5,90);

    for (auto &c: cells) {
        CrystalLattice l(c);
        CrystalLattice recip_l = l.ReciprocalLattice();

        std::vector<Coord> recip;
        recip.reserve(hkl.size());
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec_a + i.y * recip_l.vec_b + i.z * recip_l.vec_c);

        DiffractionExperiment experiment;

        XgandalfWrapper wrapper;
        IndexingSettings settings;
        settings.unit_cell = c;
        settings.max_indexing_spots = 30;
        settings.algorithm = IndexingAlgorithm::Xgandalf_fast;

        REQUIRE(!wrapper.Run(settings, recip).empty());
    }
}

TEST_CASE("JFJochIndexerService_StartStop","[Indexing]") {
    Logger logger("JFJochIndexerService");
    ZMQContext context;
    JFJochIndexerService service(context, logger);

    JFJochProtoBuf::JFJochIndexerInput input;
    JFJochProtoBuf::JFJochIndexerOutput output;
    input.set_bin_size(100);
    input.set_zmq_recv_pub_addr("inproc://#1");

    REQUIRE(service.Start(nullptr, &input, nullptr).ok());
    REQUIRE(service.Stop(nullptr, nullptr, &output).ok());
    logger.Info(std::to_string(output.ms_per_image()));
    REQUIRE(output.image_output_size() == 0);
    REQUIRE(std::isnan(output.ms_per_image()));
}

TEST_CASE("JFJochIndexerService_Operation","[Indexing]") {
    std::vector<Coord> hkl;
    for (int i = 1; i < 7; i++)
        for (int j = 1; j<6; j++)
            for (int k = 1; k < 4; k++)
                hkl.emplace_back(i,j,k);
    UnitCell cell(30,40,50,90,90,90);

    Logger logger("JFJochIndexerService");
    ZMQContext context;
    JFJochIndexerService service(context, logger);
    service.NumThreads(4);

    DiffractionExperiment experiment;
    experiment.SetUnitCell(cell);

    JFJochProtoBuf::JFJochIndexerInput input;
    input.set_bin_size(10);
    input.set_zmq_recv_pub_addr("inproc://test1");
    *input.mutable_jungfraujoch_settings() = experiment;

    REQUIRE(service.Start(nullptr, &input, nullptr).ok());

    ZMQSpotPublisher publisher(context, "inproc://test1");

    CrystalLattice l(cell);
    CrystalLattice recip_l = l.ReciprocalLattice();

    for (int image = 0; image < 20; image++) {
        std::vector<Coord> recip;
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec_a + i.y * recip_l.vec_b + i.z * recip_l.vec_c);

        publisher.PublishReciprocal(experiment, recip, image * experiment.GetSpotFindingStride());
    }

    JFJochProtoBuf::JFJochIndexerOutput output;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    REQUIRE(service.Stop(nullptr, nullptr, &output).ok());
    logger.Info(std::to_string(output.ms_per_image()));
    REQUIRE(output.image_output_size() == 20);
    REQUIRE(output.indexed_images() == 20);

    JFJochProtoBuf::IndexerStatus simpl_output;
    REQUIRE(service.GetStatus(nullptr, nullptr, &simpl_output).ok());
    REQUIRE(simpl_output.indexing_rate_size() == 2);
    REQUIRE(simpl_output.indexing_rate(0) == 100.0);
    REQUIRE(simpl_output.indexing_rate(1) == 100.0);
    REQUIRE(simpl_output.mean_spots(0) == Approx(6*5*3));
    REQUIRE(simpl_output.mean_spots(1) == Approx(6*5*3));
}