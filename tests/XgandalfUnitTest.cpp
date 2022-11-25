// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <catch2/catch.hpp>
#include "../indexing/XgandalfWrapper.h"
#include "../indexing/JFJochIndexerService.h"
#include "../writer/HDF5Objects.h"
#include "../common/ZMQSpotPublisher.h"

TEST_CASE("CrystalLattice") {
    CrystalLattice l(make_unit_cell(50,60,80, 90, 90, 90));
    REQUIRE(l.vec[0].Length() == Approx(50));
    REQUIRE(l.vec[1].Length() == Approx(60));
    REQUIRE(l.vec[2].Length() == Approx(80));
    REQUIRE(angle_deg(l.vec[0], l.vec[2]) == 90);
    REQUIRE(angle_deg(l.vec[0], l.vec[1]) == 90);
    REQUIRE(angle_deg(l.vec[1], l.vec[2]) == 90);

    l = CrystalLattice(make_unit_cell(30, 40, 70, 90, 95, 90));
    REQUIRE(l.vec[0].Length() == Approx(30));
    REQUIRE(l.vec[1].Length() == Approx(40));
    REQUIRE(l.vec[2].Length() == Approx(70));
    REQUIRE(angle_deg(l.vec[0], l.vec[2]) == 95);
    REQUIRE(angle_deg(l.vec[0], l.vec[1]) == 90);
    REQUIRE(angle_deg(l.vec[1], l.vec[2]) == 90);

    l = CrystalLattice(make_unit_cell(45, 45, 70, 90, 90, 120));
    REQUIRE(l.vec[0].Length() == Approx(45));
    REQUIRE(l.vec[1].Length() == Approx(45));
    REQUIRE(l.vec[2].Length() == Approx(70));
    REQUIRE(angle_deg(l.vec[0], l.vec[2]) == Approx(90));
    REQUIRE(angle_deg(l.vec[0], l.vec[1]) == Approx(120));
    REQUIRE(angle_deg(l.vec[1], l.vec[2]) == Approx(90));
}

TEST_CASE("CrystalLattice_Centering_P") {
    CrystalLattice l(make_unit_cell(50,60,80, 78, 80, 120));
    CrystalLattice ucl = l.Uncenter('P');

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            REQUIRE(l.vec[i][j] == ucl.vec[i][j]);
        }
    }
}

TEST_CASE("CrystalLattice_Centering_C") {
    CrystalLattice l(make_unit_cell(39.20,179.90,139.40, 90, 90, 90));
    CrystalLattice ucl = l.Uncenter('C');
    CHECK(fabs(ucl.vec[0].Length() - 92.01) < 0.1);
    CHECK(fabs(ucl.vec[1].Length() - 92.01) < 0.1);
    CHECK(ucl.vec[2].Length() == Approx(139.4));
    CHECK(fabs(angle_deg(ucl.vec[0], ucl.vec[1])  - 155.4) < 0.1);
    CHECK(angle_deg(ucl.vec[1], ucl.vec[2]) == Approx(90.0));
    CHECK(angle_deg(ucl.vec[0], ucl.vec[2]) == Approx(90.0));
}

TEST_CASE("CrystalLattice_ReciprocalLattice") {
    CrystalLattice l(make_unit_cell(50,60,80, 78, 80, 120));
    CrystalLattice rl = l.ReciprocalLattice();

    REQUIRE(l.vec[0] * rl.vec[0] == Approx(1.0));
    REQUIRE(l.vec[0] * rl.vec[1] < 1e-10);
    REQUIRE(l.vec[0] * rl.vec[2] < 1e-10);

    REQUIRE(l.vec[1] * rl.vec[0] < 1e-10);
    REQUIRE(l.vec[1] * rl.vec[1] == Approx(1.0));
    REQUIRE(l.vec[1] * rl.vec[2] < 1e-10);

    REQUIRE(l.vec[2] * rl.vec[0] < 1e-10);
    REQUIRE(l.vec[2] * rl.vec[1] < 1e-10);
    REQUIRE(l.vec[2] * rl.vec[2] == Approx(1.0));
}

TEST_CASE("Xgandalf","[Indexing]") {
    std::vector<Coord> hkl;
    for (int i = 1; i < 7; i++)
        for (int j = 1; j<6; j++)
            for (int k = 1; k < 4; k++)
                hkl.emplace_back(i,j,k);

    std::vector<JFJochProtoBuf::UnitCell> cells;
    cells.emplace_back(make_unit_cell(30,40,50,90,90,90));
    cells.emplace_back(make_unit_cell(80,80,90,90,90,120));
    cells.emplace_back(make_unit_cell(40,45,80,90,82.5,90));

    for (auto &c: cells) {
        CrystalLattice l(c);
        CrystalLattice recip_l = l.ReciprocalLattice();

        std::vector<Coord> recip;
        recip.reserve(hkl.size());
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec[0] + i.y * recip_l.vec[1] + i.z * recip_l.vec[2]);

        DiffractionExperiment experiment;

        XgandalfWrapper wrapper;
        IndexingSettings settings;
        settings.unit_cell = c;
        settings.centering = 'P';
        settings.max_indexing_spots = 30;
        wrapper.Setup(settings);

        REQUIRE(!wrapper.Run(recip).empty());
    }
}

TEST_CASE("JFJochIndexerService_StartStop","[Indexing]") {
    Logger logger("JFJochIndexerService");
    ZMQContext context;
    JFJochIndexerService service(context, logger);

    JFJochProtoBuf::IndexerInput input;
    JFJochProtoBuf::IndexerOutput output;
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
    JFJochProtoBuf::UnitCell cell;
    cell.set_a(30.0); cell.set_b(40); cell.set_c(50);
    cell.set_alpha(90); cell.set_beta(90); cell.set_gamma(90);

    Logger logger("JFJochIndexerService");
    ZMQContext context;
    JFJochIndexerService service(context, logger);
    service.NumThreads(4);

    DiffractionExperiment experiment;
    experiment.SetUnitCell(cell);
    experiment.ImagesPerTrigger(200).SpotFindingPeriod(std::chrono::milliseconds(10));
    REQUIRE(experiment.GetSpotFindingStride() == 10);

    JFJochProtoBuf::IndexerInput input;
    input = experiment;
    input.set_zmq_recv_pub_addr("inproc://test1");

    REQUIRE(service.Start(nullptr, &input, nullptr).ok());

    ZMQSpotPublisher publisher(context, "inproc://test1");

    CrystalLattice l(cell);
    CrystalLattice recip_l = l.ReciprocalLattice();

    for (int image = 0; image < 20; image++) {
        std::vector<Coord> recip;
        for (const auto &i: hkl)
            recip.emplace_back(i.x * recip_l.vec[0] + i.y * recip_l.vec[1] + i.z * recip_l.vec[2]);

        publisher.PublishReciprocal(experiment, recip, image * experiment.GetSpotFindingStride());
    }

    JFJochProtoBuf::IndexerOutput output;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    REQUIRE(service.Stop(nullptr, nullptr, &output).ok());
    logger.Info(std::to_string(output.ms_per_image()));
    REQUIRE(output.image_output_size() == 20);
    REQUIRE(output.indexed_images() == 20);

    JFJochProtoBuf::IndexerStatus simpl_output;
    REQUIRE(service.GetStatus(nullptr, nullptr, &simpl_output).ok());
    REQUIRE(simpl_output.indexing_rate().y_size() == 2);
    REQUIRE(simpl_output.indexing_rate().y(0) == 1.0);
    REQUIRE(simpl_output.indexing_rate().y(1) == 1.0);
}