#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;
using StateIndices = std::vector<StateIndex>;
using StateIndicesSet = std::unordered_set<StateIndex>;
using AdjacencyMatrix = std::vector<StateIndices>;
using Distance = int;
using Distances = std::vector<int>;

const int INF = std::numeric_limits<int>::max();


class StateSpace {
private:
    // For an efficient implementation we make use of indexing instead of shared_ptrs.
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    core::States m_states_by_index;
    StateIndex m_initial_state_index;
    StateIndices m_forward_successor_state_indices;
    StateIndices m_forward_successor_state_indices_offsets;
    StateIndicesSet m_goal_state_indices;
    // Useful information that we precompute in the constructor.
    StateIndices m_backward_successor_state_indices;
    StateIndices m_backward_successor_state_indices_offsets;
    StateIndicesSet m_deadend_state_indices;
    Distances m_goal_distances;

public:
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        core::States&& states_by_index,
        StateIndex initial_state_index,
        AdjacencyMatrix&& adjacency_matrix,
        StateIndicesSet&& goal_state_indices);
    // TODO: we must update copy semantics to create a new instance info and new states.
    StateSpace(const StateSpace& other);
    StateSpace& operator=(const StateSpace& other);
    StateSpace(StateSpace&& other);
    StateSpace& operator=(StateSpace&& other);
    ~StateSpace();

    template<typename T>
    Distances compute_distances(const T& source_state_indices, bool forward);

    void for_each_state_index(std::function<void(int state_index)>&& function) const;

    void for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int source_state_index) const;

    /**
     * Convenience functions.
     */
    bool is_goal(StateIndex state_index) const;
    bool is_nongoal(StateIndex state_index) const;
    bool is_deadend(StateIndex state_index) const;
    bool is_alive(StateIndex state_index) const;

    /**
     * Getters.
     */
    const core::States& get_states_ref() const;
    const core::State& get_state_ref(int index) const;
    int get_num_states() const;
    const Distances& get_goal_distances_ref() const;
};


class StateSpaceGenerator {
public:
    /**
     * Generates the StateSpace containing the reachable states
     * from given PDDL domain and instance files.
     */
    StateSpace generate_state_space(
        const std::string& domain_file,
        const std::string& instance_file) const;
};

}

#include "state_space.tpp"

#endif
