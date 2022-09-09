#include "../../include/dlplan/weisfeiler_lehman.h"

#include <numeric>
#include <map>

#include "color.h"

using namespace dlplan::state_space;


namespace dlplan::weisfeiler_lehman {

static std::vector<StateIndices> compute_state_indices_by_goal_distance(
    const Distances& goal_distances) {
    std::map<Distance, StateIndices> goal_distance_to_state_indices;
    for (StateIndex i = 0; i < static_cast<int>(goal_distances.size()); ++i) {
        goal_distance_to_state_indices[goal_distances[i]].push_back(i);
    }
    std::vector<StateIndices> state_indices_by_goal_distance;
    for (const auto pair : goal_distance_to_state_indices) {
        state_indices_by_goal_distance.push_back(pair.second);
    }
    return state_indices_by_goal_distance;
}

/**
 * Compresses each color by assigning a unique index.
 */
static CompressedColors compute_compressed_colors(
    const Colors& colors) {
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

/**
 * Compute the canonical color of a collection of compressed colors.
 */
static Color compute_color_for_compressed_colors(const CompressedColors& compressed_colors)  {
    return Color(compressed_colors.begin(), compressed_colors.end());
}


CompressedColors WeisfeilerLehman::compute_colors_for_state_space(
    const StateSpace& state_space) const {
    // initial coloring 0,..,n where n is number of states.
    CompressedColors old_compressed_colors(state_space.get_num_states());
    std::iota(old_compressed_colors.begin(), old_compressed_colors.end(), 0);
    CompressedColors new_compressed_colors;
    do {
        Colors new_colors;
        state_space.for_each_state_index(
            [&state_space, &old_compressed_colors, &new_colors](int source_state_index){
            Color color;
            state_space.for_each_forward_successor_state_index(
                [&old_compressed_colors, &color](int successor_state_index) {
                color.insert(old_compressed_colors[successor_state_index]);
            }, source_state_index);
            new_colors.push_back(color);
        });
        new_compressed_colors = compute_compressed_colors(new_colors);
    } while (old_compressed_colors == new_compressed_colors);
    return new_compressed_colors;
}

}
