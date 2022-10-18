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
 * Provides access to additional derived goal distance information of a StateSpace.
 */
class GoalDistanceInformation {
private:
    int m_initial_state_index;
    Distances m_goal_distances;
    StateIndicesSet m_deadend_state_indices;

    GoalDistanceInformation(
        int initial_state_index,
        Distances&& goal_distances,
        StateIndicesSet&& deadend_state_indices);
    friend class StateSpace;

public:
    GoalDistanceInformation(const GoalDistanceInformation& other);
    GoalDistanceInformation& operator=(const GoalDistanceInformation& other);
    GoalDistanceInformation(GoalDistanceInformation&& other);
    GoalDistanceInformation& operator=(GoalDistanceInformation&& other);
    ~GoalDistanceInformation();

    /**
     * Convenience functions.
     */
    bool is_goal(StateIndex state) const;
    bool is_nongoal(StateIndex state) const;
    bool is_deadend(StateIndex state) const;
    bool is_alive(StateIndex state) const;
    bool is_solvable() const;
    bool is_trivially_solvable() const;

    /**
     * Getters.
     */
    StateIndex get_initial_state_index() const;
    const StateIndicesSet& get_deadend_state_indices_ref() const;
    const Distances& get_goal_distances_ref() const;
};


/**
 * Provides access to information about States.
 */
class StateInformation {
private:
    StateMapping m_state_mapping;

    explicit StateInformation(StateMapping&& state_mapping);
    friend class StateSpace;
public:
    StateInformation(const StateInformation& other);
    StateInformation& operator=(const StateInformation& other);
    StateInformation(StateInformation&& other);
    StateInformation& operator=(StateInformation&& other);
    ~StateInformation();

    const core::State& get_state_ref(StateIndex state) const;
};


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
    core::StatesSet m_states;
    // Currently, state indices can be sparse.
    // If we plan on compressing them then
    // we need to provide functionality to obtain new indices.
    StateIndicesSet m_state_indices;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndicesSet m_goal_state_indices;

    /* Derived information */
    // for backward search
    AdjacencyList m_backward_successor_state_indices;

public:
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
     * Keeps the initial state of this.
     */
    StateSpace& operator|=(const StateSpace& other);

    /**
     * Run BrFs to compute distances.
     */
    Distances compute_distances(const StateIndicesSet& state_indices, bool forward, bool stop_if_goal) const;

    /**
     * For more readable iterations.
     */
    void for_each_state_index(std::function<void(int)>&& function) const;
    void for_each_forward_successor_state_index(std::function<void(int)>&& function, StateIndex state) const;
    void for_each_backward_successor_state_index(std::function<void(int)>&& function, StateIndex state) const;

    bool is_goal(StateIndex state) const;
    bool is_nongoal(StateIndex state) const;
    bool is_trivially_solvable() const;

    /**
     * Uniquely adds as a state and returns a reference to the stored state.
     */
    const core::State& add_state(const core::State& state);
    /**
     * Adds a transition between source and target state index.
     * Throws an exception if indices are out of bounds.
     */
    void add_transition(StateIndex source, StateIndex target);

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
    GoalDistanceInformation compute_goal_distance_information() const;
    StateInformation compute_state_information() const;
    const core::StatesSet& get_states_ref() const;
    const StateIndicesSet& get_state_indices_ref() const;
    int get_num_states() const;
    StateIndex get_initial_state_index() const;
    const AdjacencyList& get_forward_successor_state_indices_ref() const;
    const AdjacencyList& get_backward_successor_state_indices_ref() const;
    const StateIndicesSet& get_goal_state_indices_ref() const;
    const core::InstanceInfo& get_instance_info_ref() const;
    std::shared_ptr<const core::InstanceInfo> get_instance_info() const;
};

/**
 * ExitCodes from the scorpion planner.
 */
enum class ExitCode {
    /*
      For a full list of exit codes, please see driver/returncodes.py. Here,
      we only list codes that are used by the search component of the planner.
    */
    // 0-9: exit codes denoting a plan was found
    SUCCESS = 0,

    // 10-19: exit codes denoting no plan was found (without any error)
    SEARCH_UNSOLVABLE = 11,  // Task is provably unsolvable with given bound.
    SEARCH_UNSOLVED_INCOMPLETE = 12,  // Search ended without finding a solution.

    // 20-29: "expected" failures
    SEARCH_OUT_OF_MEMORY = 22,
    SEARCH_OUT_OF_TIME = 23,

    // 30-39: unrecoverable errors
    SEARCH_CRITICAL_ERROR = 32,
    SEARCH_INPUT_ERROR = 33,
    SEARCH_UNSUPPORTED = 34
};

/**
 * Generates files parsable by the StateSpaceReader
 * from given PDDL domain and instance files.
 */
class StateSpaceGenerator {
public:
    ExitCode generate_state_space(
        const std::string& domain_file,
        const std::string& instance_file) const;
};

/**
 * Parses StateSpaceGenerator output files into StateSpace.
 */
class StateSpaceReader {
public:
    StateSpace read(std::shared_ptr<const core::VocabularyInfo> vocabulary_info=nullptr, int index=-1) const;
};

}

#endif
