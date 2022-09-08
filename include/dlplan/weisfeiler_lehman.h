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
private:
    CompressedColors compute_compressed_colors(const Colors& colors) const;

public:
    WeisfeilerLehman();

    CompressedColors compute_one_dimensional_state_coloring(
        const state_space::StateSpace& state_space) const;
};

}

#endif
