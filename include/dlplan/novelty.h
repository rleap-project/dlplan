/// @brief Provides functionality for width-based planning and learning.

#ifndef DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_
#define DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_

#include "core.h"
#include "state_space.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


namespace dlplan::novelty {
class TupleNode;

using AtomIndex = int;
using AtomIndices = std::vector<AtomIndex>;

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;

using TupleNodes = std::vector<TupleNode>;


/// @brief Encapsulates mappings betweens tuples of atom indices and tuple indices, and
///        provides functionality to map from one to the other.
class NoveltyBase {
private:
    std::vector<int> m_factors;
    int m_num_atoms;
    int m_arity;
    int m_num_tuples;

public:
    NoveltyBase(int num_atoms, int arity);
    NoveltyBase(const NoveltyBase& other);
    NoveltyBase& operator=(const NoveltyBase& other);
    NoveltyBase(NoveltyBase&& other);
    NoveltyBase& operator=(NoveltyBase&& other);
    ~NoveltyBase();

    /**
     * Computes atom tuple to tuple index and vice versa.
     */
    TupleIndex atom_tuple_to_tuple_index(const AtomIndices& tuple_atom_indices) const;
    AtomIndices tuple_index_to_atom_tuple(TupleIndex tuple_index) const;

    int get_num_atoms() const;
    int get_arity() const;
};

/// @brief Implements a novelty table for the manipulation and querying of the
///        novelty status of a tuple with its representing index.
class NoveltyTable {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::vector<bool> m_table;

public:
    NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base, int num_atoms);
    NoveltyTable(const NoveltyTable& other);
    NoveltyTable& operator=(const NoveltyTable& other);
    NoveltyTable(NoveltyTable&& other);
    NoveltyTable& operator=(NoveltyTable&& other);
    ~NoveltyTable();

    /**
     * Useful for construction of tuple graphs.
     */
    void reset_novelty(const TupleIndices& tuple_indices);
    TupleIndices compute_novel_tuple_indices(
        const AtomIndices& atom_indices,
        const AtomIndices& effect_atom_indices) const;

    /**
     * Useful for width-based planners.
     *
     * Iterates over the tuples, marks each as not novel.
     * Returns true iff a novel tuple was found.
     * If the additional parameter stop_if_novel is true
     * then iteration stopps after novelty was proven.
     */
    bool insert(
        const AtomIndices& atom_indices,
        const AtomIndices& effect_atom_indices,
        bool stop_if_novel=true);

    /**
     * Useful for width-based planners (requires expert knowledge).
     *
     * The above method lacks taking into consideration
     * the atoms that were changed by operator application.
     * Hence, the user can compute tuples and pass them in here.
     */
    bool insert(const TupleIndices& tuple_indices, bool stop_if_novel=true);
};


/// @brief Encapsulates data related to a node in a tuple graph and provides
///        functionality to access it.
class TupleNode {
private:
    TupleIndex m_tuple_index;
    state_space::StateIndices m_state_indices;
    TupleIndices m_predecessors;
    TupleIndices m_successors;

public:
    TupleNode(TupleIndex tuple_index, const state_space::StateIndices& state_indices);
    TupleNode(TupleIndex tuple_index, state_space::StateIndices&& state_indices);
    TupleNode(const TupleNode& other);
    TupleNode& operator=(const TupleNode& other);
    TupleNode(TupleNode&& other);
    TupleNode& operator=(TupleNode&& other);
    ~TupleNode();

    void add_predecessor(TupleIndex tuple_index);
    void add_successor(TupleIndex tuple_index);

    /// @brief Compute the canonical string representation of this tuple node.
    /// @return The canonical string representation of this tuple node.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the TupleNode class.
    ///        Outputs a string representation of a TupleNode object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The TupleNode to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const TupleNode& tuple_node);

    /// @brief Compute a string representation of this tuple node.
    /// @return A string representation of this tuple node.
    std::string str() const;

    TupleIndex get_tuple_index() const;
    const state_space::StateIndices& get_state_indices() const;
    const TupleIndices& get_predecessors() const;
    const TupleIndices& get_successors() const;
};


/// @brief Implements a tuple graph and provides functionality for the
///        construction and for accessing the data.
class TupleGraph {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::shared_ptr<const state_space::StateSpace> m_state_space;
    // The novel tuples that make it into the tuple graph.
    std::vector<TupleNodes> m_tuple_nodes_by_distance;
    // The reachable states with distance at most the largest distance of a tuple node.
    std::vector<state_space::StateIndices> m_state_indices_by_distance;
    // The root state index
    state_space::StateIndex m_root_state_index;

public:
    TupleGraph(
        std::shared_ptr<const NoveltyBase> novelty_base,
        std::shared_ptr<const state_space::StateSpace> state_space,
        state_space::StateIndex root_state_index);
    TupleGraph(const TupleGraph& other);
    TupleGraph& operator=(const TupleGraph& other);
    TupleGraph(TupleGraph&& other);
    TupleGraph& operator=(TupleGraph&& other);
    ~TupleGraph();

    /// @brief Compute the canonical string representation of this tuple graph.
    /// @return The canonical string representation of this tuple graph.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the TupleGraph class.
    ///        Outputs a string representation of a TupleGraph object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The TupleGraph to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    friend std::ostream& operator<<(std::ostream& os, const TupleGraph& tuple_graph);

    /// @brief Compute a string representation of this tuple graph.
    /// @return A string representation of this tuple graph.
    std::string str() const;

    /// @brief Compute a string-dot representation of this tuple graph.
    /// @param verbosity_level
    /// @return A string-dot representation of this tuple graph.
    std::string to_dot(int verbosity_level) const;

    std::shared_ptr<const NoveltyBase> get_novelty_base() const;
    std::shared_ptr<const state_space::StateSpace> get_state_space() const;
    const std::vector<TupleNodes>& get_tuple_nodes_by_distance() const;
    const std::vector<state_space::StateIndices>& get_state_indices_by_distance() const;
    state_space::StateIndex get_root_state_index() const;
};

}

#endif
