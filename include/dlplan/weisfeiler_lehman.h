#ifndef DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_
#define DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_

#include <vector>

#include "state_space.h"


namespace dlplan::weisfeiler_lehman {
class Color;

using CompressedColor = int;
using CompressedColors = std::vector<CompressedColor>;
using Colors = std::vector<Color>;

/**
 * Weisfeiler-Lehman (WL) is an important algorithm in the context of description logics features
 * because the subgraphs G and H of two states s_1,s_2
 * that are isomorphic according to WL, i.e., have the same color,
 * cannot be distinguished by formulas in two-variable first order logic.
 * Hence a description logics that is as most as expressive as two-variable first order logic
 * cannot distinguish G and H as well.
 */
class WeisfeilerLehman {
private:
    CompressedColors compute_compressed_colors(const Colors& colors) const;

public:
    WeisfeilerLehman();

    CompressedColors compute_one_dimensional_state_coloring(
        const state_space::StateSpace& state_space) const;
};

}

#endif
