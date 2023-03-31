#ifndef DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_
#define DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_

#include "core.h"
#include "state_space.h"

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


/**
 * Forward declarations and usings
 */
namespace dlplan::novelty {
class TupleNode;

using AtomIndex = int;

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;
using TupleIndicesSet = std::unordered_set<TupleIndex>;

using TupleNodes = std::vector<TupleNode>;

using AtomIndices = std::vector<AtomIndex>;
}


namespace dlplan::novelty {
/**
 * Provides functionality to map atom tuples to indices and vice versa.
 */
class NoveltyBase {
private:
    std::vector<int> m_factors;
    int m_num_atoms;
    int m_max_tuple_size;
    int m_num_tuples;

public:
    NoveltyBase(int num_atoms, int max_tuple_size);
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

    /**
     * Getters.
     */
    int get_max_tuple_size() const;
    int get_num_atoms() const;
    int get_num_tuples() const;
};


/**
 * Generates all tuple indices of atom tuples of
 * size at most k of a given set of atom indices.
 */
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
};


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

    std::string str() const;

    /**
     * Getters.
     */
    TupleIndex get_tuple_index() const;
    const state_space::StateIndices& get_state_indices() const;
    const TupleIndices& get_predecessors() const;
    const TupleIndices& get_successors() const;
};


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
        state_space::StateIndex root_state,
        int width);
    TupleGraph(const TupleGraph& other);
    TupleGraph& operator=(const TupleGraph& other);
    TupleGraph(TupleGraph&& other);
    TupleGraph& operator=(TupleGraph&& other);
    ~TupleGraph();

    std::string str() const;
    std::string to_dot(int verbosity_level) const;

    /**
     * Getters.
     */
    const std::vector<TupleNodes>& get_tuple_nodes_by_distance() const;
    const std::vector<state_space::StateIndices>& get_state_indices_by_distance() const;
    state_space::StateIndex get_root_state_index() const;
    int get_width() const;
};

}

#endif
