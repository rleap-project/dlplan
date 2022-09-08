#include "../../include/dlplan/weisfeiler_lehman.h"

#include <numeric>

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

CompressedColors WeisfeilerLehman::compute_one_dimensional_state_coloring(
    const state_space::StateSpace& state_space) const {
    // initial coloring 0,..,n where n is number of states.
    CompressedColors old_compressed_coloring(state_space.get_num_states());
    std::iota(old_compressed_coloring.begin(), old_compressed_coloring.end(), 0);
    CompressedColors new_compressed_coloring;
    do {
        Colors new_coloring;
        state_space.for_each_state_index(
            [&state_space, &old_compressed_coloring, &new_coloring](int source_state_index){
            Color color;
            state_space.for_each_forward_successor_state_index(
                [&old_compressed_coloring, &color](int successor_state_index) {
                color.insert(old_compressed_coloring[successor_state_index]);
            }, source_state_index);
            new_coloring.push_back(color);
        });
        new_compressed_coloring = compute_compressed_colors(new_coloring);
    } while (old_compressed_coloring == new_compressed_coloring);
    return new_compressed_coloring;
}

}
