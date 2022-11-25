// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FastFeedbackIndexerWrapper.h"
#include "CrystalLattice.h"

void FastFeedbackIndexerWrapper::Setup(const IndexingSettings &settings) {
    CrystalLattice lattice(settings.unit_cell);
    auto ucl = lattice.Uncenter(settings.centering);
    auto rl = ucl.ReciprocalLattice();
    for (int i = 0; i < 3; i++) {
        coords(i, 0) = rl.vec[i].x;
        coords(i, 1) = rl.vec[i].y;
        coords(i, 2) = rl.vec[i].z;
    }
    cpers.max_spots = MAX_SPOTS;
    cpers.max_output_cells = 1;
    cpers.max_input_cells = 1;
    crt.num_sample_points = 32*1024;
}

std::vector<JFJochProtoBuf::UnitCell> FastFeedbackIndexerWrapper::Run(const std::vector<Coord> &coord) {
    std::vector<JFJochProtoBuf::UnitCell> ret;

    for (int i = 0; i < std::min<size_t>(MAX_SPOTS, coord.size()); i++) {
        coords(i + 3, 0) = coord[i].x;
        coords(i + 3, 1) = coord[i].y;
        coords(i + 3, 2) = coord[i].z;
    }

    Eigen::MatrixX3f cells(3 * cpers.max_output_cells, 3);  // output cells coordinate container
    Eigen::MatrixX2f scores(cpers.max_output_cells, 2);     // output cell scores container
    fast_feedback::indexer indexer{cpers};          // indexer object

    fast_feedback::memory_pin pin_coords{coords};   // pin input coordinate container
    fast_feedback::memory_pin pin_cells{cells};     // pin output cells coordinate container
    fast_feedback::memory_pin pin_scores(scores);   // pin output cell scores container
    fast_feedback::memory_pin pin_crt{fast_feedback::memory_pin::on(crt)}; // pin runtime config memory

    fast_feedback::input<float> in{&coords(0,0), &coords(0,1), &coords(0,2), 1u, static_cast<unsigned int>(coord.size())};                             // create indexer input object
    fast_feedback::output<float> out{&cells(0,0), &cells(0,1), &cells(0,2), scores.data(), cpers.max_output_cells}; // create indexer output object

    indexer.index(in, out, crt);                    // run indexer

    Coord a_orig(cells(0,0), cells(0,1), cells(0,2));
    Coord b_orig(cells(1,0), cells(1,1), cells(1,2));
    Coord c_orig(cells(2,0), cells(2,1), cells(2,2));

    JFJochProtoBuf::UnitCell cell;
    cell.set_a(a_orig.Length());
    cell.set_b(b_orig.Length());
    cell.set_c(c_orig.Length());
    cell.set_alpha(angle_deg(b_orig, c_orig));
    cell.set_beta(angle_deg(a_orig, c_orig));
    cell.set_gamma(angle_deg(a_orig, b_orig));
    ret.emplace_back(cell);

   /*
    {                                               // refine cells
        using namespace Eigen;
        MatrixX3f spots = coords.bottomRows(coords.rows() - 3);
        for (unsigned j=0u; j<out.n_cells; j++) {
            Matrix3f cell = cells.block(3 * j, 0, 3, 3).inverse();
            MatrixX3f temp = spots * cell;          // coordinates in system <cell>
            MatrixX3f miller = round(temp.array());
            temp -= miller;
            Vector<bool, Dynamic> thresh(temp.cols());
            thresh = temp.array().abs().rowwise().maxCoeff() < lsq_threshold;
            Matrix<bool, Dynamic, 3> sel(thresh.size(), 3);
            sel.colwise() = thresh;
            FullPivHouseholderQR<Eigen::MatrixX3f> qr(sel.select(miller, .0f));
            cell = qr.solve(sel.select(spots, .0f));
            cells.block(3 * j, 0, 3, 3) = cell;
            temp = spots * cell.inverse();
            temp -= miller;
            thresh = temp.array().abs().rowwise().maxCoeff() < score_threshold;
            scores(j, 1) = (float)std::reduce(std::begin(thresh), std::end(thresh), 0u, [](unsigned a, unsigned b)->unsigned {return a + b;}) / (float)spots.rows();
            scores(j, 0) /= -(3.f * spots.rows());
        }
    }
    */
    return ret;
}