#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;
using StateIndices = std::vector<StateIndex>;
using StateIndicesSet = std::unordered_set<StateIndex>;
using StateIndexPairs = std::vector<std::pair<StateIndex, StateIndex>>;
using AdjacencyList = std::vector<StateIndices>;
using Distance = int;
using Distances = std::vector<int>;

const int INF = std::numeric_limits<int>::max();
const int UNDEFINED = -1;

/**
 * State indices always go from 0,...,n-1
 * state_by_index store them in order 0,...,n-1
 * states store them unsorted for efficient hash lookup
 */
class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    // To obtain states by their index for fast access
    core::States m_states_by_index;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndicesSet m_goal_state_indices;

    /* Derived information */
    // for duplicate checking.
    core::StatesSet m_states_set;
    // for backward search
    AdjacencyList m_backward_successor_state_indices;
    // static information that is useful in general.
    Distances m_goal_distances;
    StateIndicesSet m_deadend_state_indices;

private:
    /**
     * Initializes derived information.
     */
    void initialize_backward_successors();
    void initialize_goal_distances_and_deadends();

    /**
     * Uniquely adds as a state and returns a reference to the stored state.
     */
    const core::State& add_state(const core::State& state);
    /**
     * Adds a transition between source and target state index.
     */
    void add_transition(int source_state_index, int target_state_index);

public:
    /**
     * Creates a state space where input assumes dense state indexing schema.
     */
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        core::States&& states_by_index,
        StateIndex initial_state_index,
        AdjacencyList&& forward_successor_state_indices,
        StateIndicesSet&& goal_state_indices);
    /**
     * Creates an identical copy over same InstanceInfo
     */
    StateSpace(const StateSpace& other);
    /**
     * Creates a copy over same InstanceInfo
     * that contains only a fragment of the states
     * with possible re-labelling of initial and goal states.
     */
    StateSpace(
        const StateSpace& other,
        const core::StatesSet& states_fragment,
        const StateIndicesSet& expanded_fragment,
        const StateIndicesSet& generated_fragment,
        int initial_state_index,
        const StateIndicesSet& goal_state_indices);
    StateSpace& operator=(const StateSpace& other);
    StateSpace(StateSpace&& other);
    StateSpace& operator=(StateSpace&& other);
    ~StateSpace();

    /**
     * Computes the intersection of two state spaces.
     */
    StateSpace& operator&=(const StateSpace& other);
    /**
     * Computes the union of two state spaces.
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
     * Convenience functions.
     */
    bool is_goal(StateIndex state_index) const;
    bool is_nongoal(StateIndex state_index) const;
    bool is_deadend(StateIndex state_index) const;
    bool is_alive(StateIndex state_index) const;
    bool is_solvable() const;
    bool is_trivially_solvable() const;
    void print() const;

    /**
     * Setters.
     */
    void set_initial_state(StateIndex initial_state);
    void set_goal_states(const StateIndicesSet& states);

    /**
     * Getters.
     */
    const core::States& get_states_ref() const;
    const core::State& get_state_ref(int index) const;
    int get_num_states() const;
    StateIndex get_initial_state_index() const;
    const StateIndices& get_forward_successor_state_indices_ref(int state_index) const;
    const StateIndices& get_backward_successor_state_indices_ref(int state_index) const;
    const StateIndicesSet& get_goal_state_indices_ref() const;
    const StateIndicesSet& get_deadend_state_indices_ref() const;
    const Distances& get_goal_distances_ref() const;
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
