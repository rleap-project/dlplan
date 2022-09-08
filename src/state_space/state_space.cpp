#include "../../include/dlplan/state_space.h"


namespace dlplan::state_space {

void StateSpace::for_each_state_index(std::function<void(int state_index)>&& function) const {
    for (int state_index = 0; state_index < get_num_states(); ++state_index) {
        function(state_index);
    }
}

void StateSpace::for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int source_state_index) const {
    int start = m_forward_successor_state_indices_offsets[source_state_index];
    int end = ((source_state_index + 1) < get_num_states())
        ? m_forward_successor_state_indices_offsets[source_state_index + 1]
        : m_forward_successor_state_indices.size();
    for (; start < end; ++start) {
        function(start);
    }
}

}
