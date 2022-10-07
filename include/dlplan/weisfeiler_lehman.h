#ifndef DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_
#define DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_

#include "state_space.h"

#include <vector>


/**
 * Forward declarations and usings
 */
namespace dlplan::weisfeiler_lehman {
    class Color;

    using CompressedColor = int;
    using CompressedColors = std::vector<CompressedColor>;
    using Colors = std::vector<Color>;
}


namespace dlplan::weisfeiler_lehman {
/**
 * Weisfeiler-Lehman (WL) is an important algorithm in the context of description logics features
 * because the subgraphs G and H of two states s_1,s_2
 * that are isomorphic according to WL, i.e., have the same color,
 * cannot be distinguished by formulas in two-variable first order logic.
 * Hence a description logics that is as most as expressive as two-variable first order logic
 * cannot distinguish G and H as well.
 */
class WeisfeilerLehman {
public:
    WeisfeilerLehman();
    WeisfeilerLehman(const WeisfeilerLehman& other);
    WeisfeilerLehman& operator=(const WeisfeilerLehman& other);
    WeisfeilerLehman(WeisfeilerLehman&& other);
    WeisfeilerLehman& operator=(WeisfeilerLehman&& other);
    ~WeisfeilerLehman();

    CompressedColors compute_colors_for_state_space(
        const state_space::StateSpace& state_space) const;
};

}

#endif
