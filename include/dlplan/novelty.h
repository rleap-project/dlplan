#ifndef DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_
#define DLPLAN_INCLUDE_DLPLAN_NOVELTY_H_

#include "core.h"
#include "state_space.h"


namespace dlplan::novelty {
class TupleNode;

using AtomIndex = int;
using AtomTuple = std::vector<AtomIndex>;
using AtomTuples = std::vector<AtomTuple>;

using TupleIndex = int;
using TupleIndices = std::vector<TupleIndex>;

using TupleNodes = std::vector<TupleNode>;

using AtomIndices = std::vector<AtomIndex>;

using StateIndex = state_space::StateIndex;
using StateIndices = std::vector<StateIndex>;


/**
 * Provides functionality to map atom tuples to indices and vice versa.
 * For potential debugging purposes we add
 */
class NoveltyBase {
private:
    std::vector<int> m_factors;
    int m_width;

public:
    explicit NoveltyBase(int width);

    /**
     * Computes atom tuple to tuple index and vice versa.
     */
    TupleIndex atom_tuple_to_tuple_index(const AtomTuple& atom_tuple) const;
    AtomTuple tuple_index_to_atom_tuple(TupleIndex tuple_index) const;
};


/**
 * Generates all tuple indices of atom tuples of
 * size at most k of a given set of atom indices.
 */
class TupleIndexGenerator {
public:
    class value_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = TupleIndex;

            value_iterator(std::shared_ptr<const NoveltyBase> novelty_base, const AtomIndices& atom_indices, int k, bool end=false);

            bool operator!=(const value_iterator& other) const;
            bool operator==(const value_iterator& other) const;

            TupleIndex operator*() const;
            // Postfix increment
            value_iterator operator++(int);
            // Prefix increment
            value_iterator& operator++();

        private:
            // the data to generate next tuple index.
            std::shared_ptr<const NoveltyBase> m_novelty_base;
            AtomIndices m_atom_indices;
            int m_k;
            // the output
            TupleIndex m_tuple_index;

        private:
            void seek_next();
    };

    TupleIndexGenerator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        const AtomIndices& atom_indices);

    value_iterator begin();
    value_iterator end();
};


class NoveltyTable {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    std::vector<bool> m_table;
public:
    NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base);

    /**
     * Computes all tuples that are novel for a given state.
     */
    TupleIndices compute_novel_tuples(const core::State& state) const;

    /**
     * All tuples become not novel anymore
     */
    void reset_tuple_novelty(const TupleIndices& tuple_indices);
};


class TupleNode {
private:
    TupleIndex m_tuple_index;
    StateIndices m_state_indices;
};


class TupleGraph {
private:
    // The novel tuples that make it into the tuple graph.
    std::vector<TupleNodes> m_tuple_nodes_by_distance;
    // The reachable states with distance at most the largest distance of a tuple node.
    std::vector<StateIndices> m_state_indices_by_distance;
    // The root state index
    StateIndex m_root_state_index;

public:
    TupleGraph(
        std::shared_ptr<const NoveltyBase> novelty_base,
        const state_space::StateSpace& state_space,
        state_space::StateIndex root_state);
};

}

#endif
