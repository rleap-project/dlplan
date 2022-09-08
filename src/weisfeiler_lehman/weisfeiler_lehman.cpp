#include "../../include/dlplan/weisfeiler_lehman.h"

#include "color.h"


namespace dlplan::weisfeiler_lehman {

CompressedColors WeisfeilerLehman::compute_compressed_colors(
    const Colors& colors) const {
    std::unordered_map<Color, int> color_to_compressed_color;
    CompressedColors compressed_colors;
    compressed_colors.reserve(colors.size());
    for (const auto& color : colors) {
        auto find_result = color_to_compressed_color.insert(
            std::make_pair(color,
            color_to_compressed_color.size()));
        compressed_colors.push_back(find_result.first->second);
    }
    return compressed_colors;
}

CompressedColors WeisfeilerLehman::compute_state_coloring(
    const state_space::StateSpace& state_space) const {

}

}
