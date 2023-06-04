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

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;
using TupleIndicesSet = std::unordered_set<TupleIndex>;

using TupleNodes = std::vector<TupleNode>;

using AtomIndices = std::vector<AtomIndex>;


/// @brief Encapsulates mappings betweens tuples of atom indices and tuple indices, and
///        provides functionality to map from one to the other.
class NoveltyBase {
private:
    std::vector<int> m_factors;
    int m_num_atoms;
    int m_tuple_size;
    int m_num_tuples;

public:
    NoveltyBase(int num_atoms, int tuple_size);
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

    int get_tuple_size() const;
    int get_num_atoms() const;
    int get_num_tuples() const;
};


/// @brief Implements an iterator over all tuples of atom indices of size k
///        from a given set of atom indices.
class TupleIndexGenerator {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    AtomIndices m_atom_indices;

public:
    class tuple_index_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = TupleIndex;
            using distance_type     = std::ptrdiff_t;

            tuple_index_iterator(std::shared_ptr<const NoveltyBase> novelty_base, const AtomIndices& atom_indices, bool end=false);
            tuple_index_iterator(const tuple_index_iterator& other);
            tuple_index_iterator& operator=(const tuple_index_iterator& other);
            tuple_index_iterator(tuple_index_iterator&& other);
            tuple_index_iterator& operator=(tuple_index_iterator&& other);
            ~tuple_index_iterator();

            bool operator!=(const tuple_index_iterator& other) const;
            bool operator==(const tuple_index_iterator& other) const;

            TupleIndex operator*() const;
            // Postfix increment
            tuple_index_iterator operator++(int);
            // Prefix increment
            tuple_index_iterator& operator++();

        private:
            // The input data.
            std::shared_ptr<const NoveltyBase> m_novelty_base;
            AtomIndices m_atom_indices;
            int m_width;
            /* The data to generate next tuple index. */
            // compact representation of atom tuple
            int m_count;
            // atom indices in current tuple
            AtomIndices m_tuple_atom_indices;
            // the output, i.e., the index of the atom tuple
            TupleIndex m_tuple_index;

        private:
            void seek_next();
    };

    TupleIndexGenerator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        const AtomIndices& atom_indices);
    TupleIndexGenerator(const TupleIndexGenerator& other);
    TupleIndexGenerator& operator=(const TupleIndexGenerator& other);
    TupleIndexGenerator(TupleIndexGenerator&& other);
    TupleIndexGenerator& operator=(TupleIndexGenerator&& other);
    ~TupleIndexGenerator();

    tuple_index_iterator begin();
    tuple_index_iterator end();

    int get_width() const;
};


/// @brief Implements a novelty table for the manipulation and querying of the
///        novelty status of a tuple with its representing index.
class NoveltyTable {
private:
    std::vector<bool> m_table;

public:
    explicit NoveltyTable(int num_tuples);
    NoveltyTable(const NoveltyTable& other);
    NoveltyTable& operator=(const NoveltyTable& other);
    NoveltyTable(NoveltyTable&& other);
    NoveltyTable& operator=(NoveltyTable&& other);
    ~NoveltyTable();

    /**
     * Useful for construction of tuple graphs.
     */
    void reset_novelty(const TupleIndicesSet& tuple_indices);
    TupleIndices compute_novel_tuple_indices(TupleIndexGenerator&& tuple_index_generator) const;

    /**
     * Useful for width-based planners.
     *
     * Iterates over the tuples, marks each as not novel.
     * Returns true iff a novel tuple was found.
     * If the additional parameter stop_if_novel is true
     * then iteration stopps after novelty was proven.
     */
    bool insert(TupleIndexGenerator&& tuple_index_generator, bool stop_if_novel=true);

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
    // The width
    int m_width;

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
