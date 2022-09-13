#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;
using StateIndices = std::vector<StateIndex>;
using StateIndicesSet = std::unordered_set<StateIndex>;
using StateIndexPairs = std::vector<std::pair<StateIndex, StateIndex>>;
using AdjacencyList = std::vector<StateIndicesSet>;
using Distance = int;
using Distances = std::vector<int>;

const int INF = std::numeric_limits<int>::max();
const int UNDEFINED = -1;

/**
 * Provides access to additional derived knowledge of a StateSpace.
 */
class StateSpaceInformation {
private:
    int m_initial_state_index;
    Distances m_goal_distances;
    StateIndicesSet m_deadend_state_indices;

    StateSpaceInformation(
        int initial_state_index,
        Distances&& goal_distances,
        StateIndicesSet&& deadend_state_indices);
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
    const StateIndicesSet& get_deadend_state_indices_ref() const;
    const Distances& get_goal_distances_ref() const;
};

/**
 * StateSpace stores states, transitions,
 * a single initial state, and a set of goal states.
 *
 * StateSpace makes use of indexing where possible for efficiency reasons.
 *   state_by_index store them in order 0,...,n-1
 *   states_set store them unsorted for efficient hash lookup
 *
 * StateSpace provides special functionality useful for learning:
 *    1. add states and transitions individually,
 *    2. construct a StateSpace as a fragment of another StateSpace,
 *    3. merge a StateSpace into another StateSpace
 * 2. and 3. together are generalizations of 1. to simplify this process.
 */
class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    core::States m_states_by_index;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndicesSet m_goal_state_indices;

    /* Derived information */
    // for backward search
    AdjacencyList m_backward_successor_state_indices;
    // for duplicate checking.
    core::StatesSet m_states;

public:
    /**
     * states must use indexing 0,...,n-1
     */
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        core::StatesSet&& states,
        StateIndex initial_state_index,
        AdjacencyList&& forward_successor_state_indices,
        StateIndicesSet&& goal_state_indices);
    StateSpace(const StateSpace& other);
    /**
     * Creates a copy over same InstanceInfo
     * that contains only a fragment of the states.
     * State indices are remapped to 0,...,m-1
     */
    StateSpace(
        const StateSpace& other,
        const StateIndicesSet& expanded_fragment,
        const StateIndicesSet& generated_fragment);
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
    Distances compute_distances(const StateIndicesSet& state_indices, bool forward) const;

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
    void set_goal_state_indices(const StateIndicesSet& states);

    /**
     * Getters.
     */
    StateSpaceInformation compute_state_space_information() const;
    const core::States& get_states_ref() const;
    const core::State& get_state_ref(StateIndex index) const;
    // Useful to obtain access to index remapping.
    const core::State& get_state_ref(core::State& state) const;
    int get_num_states() const;
    StateIndex get_initial_state_index() const;
    const StateIndicesSet& get_forward_successor_state_indices_ref(StateIndex state_index) const;
    const StateIndicesSet& get_backward_successor_state_indices_ref(StateIndex state_index) const;
    const StateIndicesSet& get_goal_state_indices_ref() const;
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

    /**
     * Prunes the state space and returns the mapping from new to old state indices.
     */
    std::pair<StateSpace, StateIndices> compute_state_space_fragment(const StateIndicesSet& state_indices);
};

}

#endif
