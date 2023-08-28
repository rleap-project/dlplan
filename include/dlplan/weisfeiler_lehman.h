/// @brief This module is work in progress and is not ready to be used.

#ifndef DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_
#define DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_

#include <vector>

#include "state_space.h"


namespace dlplan::weisfeiler_lehman {
class Color;

using CompressedColor = int;
using CompressedColors = std::vector<CompressedColor>;
using Colors = std::vector<Color>;


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
