/// @brief Provides functionality for width-based planning and learning.

#ifndef DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_
#define DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core.h"
#include "state_space.h"


// Forward declarations of this header
namespace dlplan::novelty {
class NoveltyBase;
class TupleNode;
class TupleGraph;
}


namespace dlplan::novelty
{
using AtomIndex = int;
using AtomIndices = std::vector<AtomIndex>;

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;
using TupleIndicesSet = std::unordered_set<TupleIndex>;

using StateIndicesSet = std::set<int>;

using TupleNodeIndex = int;
using TupleNodeIndices = std::vector<TupleNodeIndex>;
using TupleNodeIndicesSet = std::unordered_set<TupleNodeIndex>;
using TupleNodes = std::vector<TupleNode>;

/// @brief Implements a perfect hash function between tuple index and atom indices.
class NoveltyBase
{
private:
    std::vector<int> m_factors;
    int m_num_atoms;
    int m_arity;

public:
    NoveltyBase(int num_atoms, int arity);
    NoveltyBase(const NoveltyBase &other);
    NoveltyBase &operator=(const NoveltyBase &other);
    NoveltyBase(NoveltyBase &&other);
    NoveltyBase &operator=(NoveltyBase &&other);
    ~NoveltyBase();

    /// @brief Convert the input atom indices of size arity to a tuple index.
    ///        This function is a perfect hash function.
    ///        The user must sort atoms if atom tuples are viewed as sets.
    /// @param atom_indices A vector of atom indices of size at most arity
    /// @return A tuple index that identifies the input atom indices.
    TupleIndex atom_indices_to_tuple_index(const AtomIndices &atom_indices) const;

    /// @brief Convert the input tuple index to atom indices. The resulting atom indices
    ///        are in the same order as they were used when computing the tuple index.
    /// @param tuple_index
    /// @return
    AtomIndices tuple_index_to_atom_indices(TupleIndex tuple_index) const;

    const std::vector<int>& get_factors() const;
    int get_num_atoms() const;
    int get_arity() const;

    static const int place_holder;
};


/// @brief Implements a novelty table for the manipulation and querying of the
///        novelty status of tuple indices.
class NoveltyTable
{
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::vector<bool> m_table;

public:
    NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base);
    NoveltyTable(const NoveltyTable &other);
    NoveltyTable &operator=(const NoveltyTable &other);
    NoveltyTable(NoveltyTable &&other);
    NoveltyTable &operator=(NoveltyTable &&other);
    ~NoveltyTable();

    /// @brief Compute all novel tuple indices derived from tuples of the input atom indices
    ///        of size that is at most the arity as specified in the novelty_base.
    /// @param atom_indices A vector of atom indices sorted ascendingly.
    ///                     The user must take care that the atom indices are within correct bound.
    /// @return Vector of novel tuples indices derived from the input atom indices.
    TupleIndices compute_novel_tuple_indices(
        const AtomIndices &atom_indices) const;

    /// @brief Compute all novel tuple indices derived from tuples of the input atom indices
    ///        and add atom indices of size that is at most the arity as specified in the novelty_base.
    ///        There is an additional constraint that requires that each tuple of atom indices
    ///        will contain at least one atom index from add atom indices.
    /// @param atom_indices A vector of atom indices sorted ascendingly.
    ///                     The user must take care that the atom indices are within correct bound.
    /// @param add_atom_indices A vector of atom indices sorted ascendingly.
    ///                         The user must take care that the atom indices are within correct bound.
    ///                         The user must take care that it is disjoint with atom indices.
    /// @return Vector of novel tuples indices derived from the input atom indices.
    TupleIndices compute_novel_tuple_indices(
        const AtomIndices &atom_indices,
        const AtomIndices &add_atom_indices) const;

    /// @brief Mark all input tuple indices as not novel.
    /// @param tuple_indices A vector of tuple indices.
    ///                      The user must take care that the tuple indices are within correct bound.
    /// @param stop_if_novel Stop the iteration early if a tuple index was novel.
    /// @return True if at least one given tuple index was novel.
    bool insert_tuple_indices(const TupleIndices &tuple_indices, bool stop_if_novel = false);

    /// @brief Mark all novel tuple indices derived from tuples of the input atom indices
    ///        of size that is at most the arity as specified in the novelty_base as not novel.
    /// @param atom_indices A vector of atom indices sorted ascendingly.
    ///                     The user must take care that the atom indices are within correct bound.
    /// @param stop_if_novel Stop the iteration early if a tuple index was novel.
    /// @return True if at least one given tuple index was novel.
    bool insert_atom_indices(
        const AtomIndices &atom_indices,
        bool stop_if_novel = false);

    /// @brief Mark all novel tuple indices derived from tuples of the input atom indices
    ///        of size that is at most the arity as specified in the novelty_base as not novel.
    ///        There is an additional constraint that requires that each tuple of atom indices
    ///        will contain at least one atom index from add atom indices.
    /// @param atom_indices A vector of atom indices sorted ascendingly.
    ///                     The user must take care that the atom indices are within correct bound.
    /// @param add_atom_indices A vector of atom indices sorted ascendingly.
    ///                         The user must take care that the atom indices are within correct bound.
    ///                         The user must take care that it is disjoint with atom indices.
    /// @param stop_if_novel Stop the iteration early if a tuple index was novel.
    /// @return True if at least one given tuple index was novel.
    bool insert_atom_indices(
        const AtomIndices &atom_indices,
        const AtomIndices &add_atom_indices,
        bool stop_if_novel = false);

    /// @brief Resizes the novelty table.
    void resize(std::shared_ptr<const NoveltyBase> novelty_base);

    const std::shared_ptr<const NoveltyBase> get_novelty_base() const;
};


/// @brief Encapsulates data related to a node in a tuple graph and provides
///        functionality to access it.
class TupleNode
{
private:
    TupleNodeIndex m_index;
    TupleIndex m_tuple_index;
    StateIndicesSet m_state_indices;
    TupleNodeIndices m_predecessors;
    TupleNodeIndices m_successors;

    TupleNode(TupleNodeIndex index, TupleIndex tuple_index, const StateIndicesSet &state_indices);
    TupleNode(TupleNodeIndex index, TupleIndex tuple_index, StateIndicesSet &&state_indices);

    void add_predecessor(TupleIndex tuple_index);
    void add_successor(TupleIndex tuple_index);

    friend class TupleGraphBuilder;
    friend class TupleGraph;

public:
    TupleNode(const TupleNode &other);
    TupleNode &operator=(const TupleNode &other);
    TupleNode(TupleNode &&other);
    TupleNode &operator=(TupleNode &&other);
    ~TupleNode();

    /// @brief Compute the canonical string representation of this tuple node.
    /// @return The canonical string representation of this tuple node.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the TupleNode class.
    ///        Outputs a string representation of a TupleNode object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The TupleNode to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream &operator<<(std::ostream &os, const TupleNode &tuple_node);

    /// @brief Compute a string representation of this tuple node.
    /// @return A string representation of this tuple node.
    std::string str() const;

    TupleNodeIndex get_index() const;
    TupleIndex get_tuple_index() const;
    const StateIndicesSet &get_state_indices() const;
    const TupleIndices &get_predecessors() const;
    const TupleIndices &get_successors() const;
};


/// @brief Implements a tuple graph and provides functionality for the
///        construction and for accessing the data.
class TupleGraph
{
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::shared_ptr<const state_space::StateSpace> m_state_space;
    state_space::StateIndex m_root_state_index;

    TupleNodes m_nodes;
    std::vector<TupleNodeIndices> m_node_indices_by_distance;
    std::vector<state_space::StateIndices> m_state_indices_by_distance;

public:
    TupleGraph(
        std::shared_ptr<const NoveltyBase> novelty_base,
        std::shared_ptr<const state_space::StateSpace> state_space,
        state_space::StateIndex root_state_index);
    TupleGraph(const TupleGraph &other);
    TupleGraph &operator=(const TupleGraph &other);
    TupleGraph(TupleGraph &&other);
    TupleGraph &operator=(TupleGraph &&other);
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
    friend std::ostream &operator<<(std::ostream &os, const TupleGraph &tuple_graph);

    /// @brief Compute a string representation of this tuple graph.
    /// @return A string representation of this tuple graph.
    std::string str() const;

    /// @brief Compute a string-dot representation of this tuple graph.
    /// @param verbosity_level
    /// @return A string-dot representation of this tuple graph.
    std::string to_dot(int verbosity_level) const;

    std::shared_ptr<const NoveltyBase> get_novelty_base() const;
    std::shared_ptr<const state_space::StateSpace> get_state_space() const;
    state_space::StateIndex get_root_state_index() const;
    const TupleNodes& get_tuple_nodes() const;
    const std::vector<TupleNodeIndices>& get_tuple_node_indices_by_distance() const;
    const std::vector<state_space::StateIndices>& get_state_indices_by_distance() const;
};

}

#endif
