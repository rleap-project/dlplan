#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"

#include <unordered_map>
#include <unordered_set>


/**
 * Forward declarations and usings
 */
namespace dlplan::state_space {
    using StateIndex = int;
    using StateIndices = std::vector<StateIndex>;
    using StateIndicesSet = std::unordered_set<StateIndex>;
    using AdjacencyList = std::unordered_map<StateIndex, StateIndicesSet>;
    using Distance = int;
    using Distances = std::unordered_map<StateIndex, Distance>;
    using StateMapping = std::unordered_map<StateIndex, core::State>;

    const int UNDEFINED = -1;
}


namespace dlplan::state_space {
/**
 * StateSpace stores states, transitions,
 * a single initial state, and a set of goal states.
 *
 * We use sparse indexing which makes it easier
 * to use the same indexing when incrementally
 * adding states from an existing state space
 * and for back reference to the original state space.
 */
class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    StateMapping m_states;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndicesSet m_goal_state_indices;
    /* Derived information */
    // for backward search
    AdjacencyList m_backward_successor_state_indices;

public:
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        StateMapping&& index_to_state,
        StateIndex initial_state_index,
        AdjacencyList&& forward_successor_state_indices,
        StateIndicesSet&& goal_state_indices);
    StateSpace(const StateSpace& other);
    /**
     * Creates a copy over same InstanceInfo
     * that contains only a fragment of the states.
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
     * Run BrFs to compute distances.
     */
    Distances compute_distances(const StateIndicesSet& state_indices, bool forward, bool stop_if_goal) const;
    Distances compute_goal_distances() const;

    /**
     * For more readable iterations.
     */
    void for_each_state(std::function<void(const core::State&)>&& function) const;
    void for_each_forward_successor_state_index(std::function<void(int)>&& function, StateIndex state) const;
    void for_each_backward_successor_state_index(std::function<void(int)>&& function, StateIndex state) const;

    bool is_goal(StateIndex state) const;
    bool is_nongoal(StateIndex state) const;

    /**
     * Pretty printing.
     */
    void print() const;
    /**
     * Creates a dot representation that can be read with graphviz.
     */
    std::string to_dot(int verbosity_level) const;

    /**
     * Setters.
     */
    void set_initial_state_index(StateIndex initial_state);
    void set_goal_state_indices(const StateIndicesSet& goal_states);

    /**
     * Getters.
     */
    std::shared_ptr<const core::InstanceInfo> get_instance_info() const;
    const StateMapping& get_states() const;
    StateIndex get_initial_state_index() const;
    const AdjacencyList& get_forward_successor_state_indices() const;
    const AdjacencyList& get_backward_successor_state_indices() const;
    const StateIndicesSet& get_goal_state_indices() const;
};

/**
 * Generate a state space from PDDL input files.
 */
extern StateSpace generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file,
    std::shared_ptr<const core::VocabularyInfo> vocabulary_info=nullptr,
    int index=-1);

}

#endif
