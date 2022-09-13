#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;
using StateIndices = std::unordered_set<StateIndex>;
using AdjacencyList = std::unordered_map<StateIndex, StateIndices>;
using Distance = int;
using Distances = std::unordered_map<StateIndex, Distance>;

const int INF = std::numeric_limits<int>::max();
const int UNDEFINED = -1;

/**
 * Provides access to additional derived knowledge of a StateSpace.
 */
class StateSpaceInformation {
private:
    int m_initial_state_index;
    Distances m_goal_distances;
    StateIndices m_deadend_state_indices;

    StateSpaceInformation(
        int initial_state_index,
        Distances&& goal_distances,
        StateIndices&& deadend_state_indices);
    friend class StateSpace;

public:
    /**
     * Convenience functions.
     */
    bool is_goal(StateIndex state_index) const;
    bool is_nongoal(StateIndex state_index) const;
    bool is_deadend(StateIndex state_index) const;
    bool is_alive(StateIndex state_index) const;
    bool is_solvable() const;
    bool is_trivially_solvable() const;

    /**
     * Getters.
     */
    int get_initial_state() const;
    const StateIndices& get_deadend_state_indices_ref() const;
    const Distances& get_goal_distances_ref() const;
};

/**
 * StateSpace stores states, transitions,
 * a single initial state, and a set of goal states.
 *
 * We use sparse indexing which makes it easier
 * to use the same indexing when incrementally
 * adding states from an existing state space.
 */
class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    std::unordered_map<StateIndex, core::State> m_state_index_to_state;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndices m_goal_state_indices;

    /* Derived information */
    // for backward search
    AdjacencyList m_backward_successor_state_indices;

public:
    /**
     * states must use indexing 0,...,n-1
     */
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        std::unordered_map<StateIndex, core::State>&& state_index_to_state,
        StateIndex initial_state_index,
        AdjacencyList&& forward_successor_state_indices,
        StateIndices&& goal_state_indices);
    StateSpace(const StateSpace& other);
    /**
     * Creates a copy over same InstanceInfo
     * that contains only a fragment of the states.
     * State indices are remapped to 0,...,m-1
     */
    StateSpace(
        const StateSpace& other,
        const StateIndices& expanded_fragment,
        const StateIndices& generated_fragment);
    StateSpace& operator=(const StateSpace& other);
    StateSpace(StateSpace&& other);
    StateSpace& operator=(StateSpace&& other);
    ~StateSpace();

    /**
     * Merges other into this and returns a reference to this.
     * State indices are remapped to 0,...,m-1
     */
    StateSpace& operator|=(const StateSpace& other);

    /**
     * Run BrFs to compute distances.
     */
    Distances compute_distances(const StateIndices& state_indices, bool forward) const;

    /**
     * For more readable iterations.
     */
    void for_each_state_index(std::function<void(int state_index)>&& function) const;
    void for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const;
    void for_each_backward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const;

    /**
     * Uniquely adds as a state and returns a reference to the stored state.
     */
    const core::State& add_state(const core::State& state);
    /**
     * Adds a transition between source and target state index.
     * Throws an exception if indices are out of bounds.
     */
    void add_transition(int source_state_index, int target_state_index);

    /**
     * Pretty printing.
     */
    void print() const;

    /**
     * Setters.
     */
    void set_initial_state_index(StateIndex initial_state);
    void set_goal_state_indices(const StateIndices& states);

    /**
     * Getters.
     */
    StateSpaceInformation compute_state_space_information() const;
    const std::unordered_map<StateIndex, core::State>& get_state_index_to_state_ref() const;
    const core::State& get_state_ref(StateIndex index) const;
    int get_num_states() const;
    StateIndex get_initial_state_index() const;
    const StateIndices& get_forward_successor_state_indices_ref(StateIndex state_index) const;
    const StateIndices& get_backward_successor_state_indices_ref(StateIndex state_index) const;
    const StateIndices& get_goal_state_indices_ref() const;
    std::shared_ptr<const core::InstanceInfo> get_instance_info() const;
};


class StateSpaceGenerator {
public:
    /**
     * Generates the StateSpace containing the reachable states
     * from given PDDL domain and instance files.
     * Multi instance case: pass a fully initialized VocabularyInfo.
     * Single instance case: VocabularyInfo is constructed inside the function.
     */
    StateSpace generate_state_space(
        const std::string& domain_file,
        const std::string& instance_file,
        std::shared_ptr<const core::VocabularyInfo> vocabulary_info=nullptr) const;
};

}

#endif
