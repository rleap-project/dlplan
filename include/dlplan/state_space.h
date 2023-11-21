/// Provides functionality for generating state spaces from PDDL.

#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "core.h"


namespace dlplan::state_space {
using StateIndex = int;
using StateIndices = std::vector<StateIndex>;
using StateIndicesSet = std::unordered_set<StateIndex>;
using AdjacencyList = std::unordered_map<StateIndex, StateIndicesSet>;
using Distance = int;
using Distances = std::unordered_map<StateIndex, Distance>;
using StateMapping = std::unordered_map<StateIndex, core::State>;

const int UNDEFINED = -1;


/// @brief Implements a state space in sparse state representation.
class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<core::InstanceInfo> m_instance_info;
    StateMapping m_states;
    StateIndex m_initial_state_index;
    AdjacencyList m_forward_successor_state_indices;
    StateIndicesSet m_goal_state_indices;
    /* Derived information */
    // for backward search
    AdjacencyList m_backward_successor_state_indices;

public:
    StateSpace(
        std::shared_ptr<core::InstanceInfo>&& instance_info,
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
        const StateIndicesSet& state_indices);
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

    /**
     * Creates a string representations
     */
    std::string str() const;
    /**
     * Creates a dot representation that can be read with graphviz.
     */
    std::string to_dot(int verbosity_level) const;

    void set_initial_state_index(StateIndex initial_state);
    void set_goal_state_indices(const StateIndicesSet& goal_states);
    std::shared_ptr<core::InstanceInfo> get_instance_info() const;
    const StateMapping& get_states() const;
    StateIndex get_initial_state_index() const;
    const AdjacencyList& get_forward_successor_state_indices() const;
    const AdjacencyList& get_backward_successor_state_indices() const;
    const StateIndicesSet& get_goal_state_indices() const;
};


/// @brief Represents the status of the generated state space.
enum class GeneratorExitCode {
    COMPLETE,
    INCOMPLETE,
    FAIL,
};


/// @brief Encapsulates the result of the state space generation process.
struct GeneratorResult {
    GeneratorExitCode exit_code;
    std::shared_ptr<StateSpace> state_space;
};


/// @brief Generates a state space from PDDL input files and limit on resources.
/// @param domain_file
/// @param instance_file
/// @param vocabulary_info
/// @param index
/// @param max_time
/// @return
extern GeneratorResult generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file,
    std::shared_ptr<core::VocabularyInfo> vocabulary_info=nullptr,
    core::InstanceIndex index=-1,
    int max_time=std::numeric_limits<int>::max()-1,
    int max_num_states=std::numeric_limits<int>::max()-1);

}

#endif
