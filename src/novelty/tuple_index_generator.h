#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_

#include "../../include/dlplan/novelty.h"

#include "../utils/math.h"


namespace dlplan::novelty {

// https://stackoverflow.com/questions/127704/algorithm-to-return-all-combinations-of-k-elements-from-n
template <typename Iterator>
static bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
    /* Credits: Mark Nelson http://marknelson.us
    https://marknelson.us/posts/2002/03/01/next-permutation.html
    */
    if ((first == last) || (first == k) || (last == k))
        return false;
    Iterator i1 = first;
    Iterator i2 = last;
    ++i1;
    if (last == i1)
        return false;
    i1 = last;
    --i1;
    i1 = k;
    --i2;
    while (first != i1)
    {
        if (*--i1 < *i2)
        {
            Iterator j = k;
            while (!(*i1 < *j)) ++j;
            std::iter_swap(i1,j);
            ++i1;
            ++j;
            i2 = k;
            std::rotate(i1,j,last);
            while (last != j)
            {
                ++j;
                ++i2;
            }
            std::rotate(k,i2,last);
            return true;
        }
    }
    std::rotate(first,k,last);
    return false;
}


static AtomIndices pad_and_sort_atom_indices(const AtomIndices& atom_indices, int width, int dummy_atom_index) {
    AtomIndices result(std::max(width, static_cast<int>(atom_indices.size())), dummy_atom_index);
    std::copy(atom_indices.begin(), atom_indices.end(), result.begin());
    std::sort(result.begin(), result.end());
    return result;
}


template<int Arity=-1>
class tuple_index_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = TupleIndex;
    using distance_type     = std::ptrdiff_t;

    /// @brief Implements an iterator over all sorted pairs of atom indices
    ///        of size specificed by novelty base.
    /// @param novelty_base
    /// @param atom_indices
    /// @param effect_atom_indices
    /// @param end
    tuple_index_iterator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        AtomIndices atom_indices,
        AtomIndices effect_atom_indices,
        bool end=false)
        : m_novelty_base(novelty_base),
          m_atom_indices(pad_and_sort_atom_indices(
            atom_indices, novelty_base->get_arity(), novelty_base->get_num_atoms())
          ),
          m_effect_atom_indices(pad_and_sort_atom_indices(
            effect_atom_indices, novelty_base->get_arity(), novelty_base->get_num_atoms())
          ),
          m_arity(novelty_base->get_arity()),
          m_count(end ? utils::binomial_coefficient(
            std::max(novelty_base->get_arity(), static_cast<int>(atom_indices.size())),
            novelty_base->get_arity()) : -1
          ),
          m_tuple_atom_indices(novelty_base->get_arity()) {
        if (effect_atom_indices.size() > 0) {
            throw std::runtime_error("tuple_index_iterator::tuple_index_iterator - generic implementation does currently not support effect_atom_indices.");
        }
        assert(static_cast<int>(m_atom_indices.size()) >= novelty_base->get_arity());
        assert(std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()));
        if (!end) seek_next();
    }
    ~tuple_index_iterator() = default;

    bool operator!=(const tuple_index_iterator& other) const {
        return !(*this == other);
    }

    bool operator==(const tuple_index_iterator& other) const {
        return m_count == other.m_count;
    }

    TupleIndex operator*() const {
        return m_tuple_index;
    }

    // Postfix increment
    tuple_index_iterator operator++(int) {
        tuple_index_iterator prev = *this;
        seek_next();
        return prev;
    }

    // Prefix increment
    tuple_index_iterator& operator++() {
        seek_next();
        return *this;
    }

private:
    // The input data.
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    AtomIndices m_atom_indices;
    AtomIndices m_effect_atom_indices;
    int m_arity;
    /* The data to generate next tuple index. */
    // compact representation of atom tuple
    int m_count;
    // atom indices in current tuple
    AtomIndices m_tuple_atom_indices;
    // the output, i.e., the index of the atom tuple
    TupleIndex m_tuple_index;

private:
    void seek_next() {
        ++m_count;
        next_combination(m_atom_indices.begin(), m_atom_indices.begin() + m_arity, m_atom_indices.end());
        std::copy(m_atom_indices.begin(), m_atom_indices.begin() + m_arity, m_tuple_atom_indices.begin());
        m_tuple_index = m_novelty_base->atom_tuple_to_tuple_index(m_tuple_atom_indices);
    }
};

/// @brief Implements an iterator over all tuples of atom indices of size Arity
///        from a given set of atom indices.
template<int Arity=-1>
class TupleIndexGenerator {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    AtomIndices m_atom_indices;
    AtomIndices m_effect_atom_indices;

public:
    TupleIndexGenerator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        AtomIndices atom_indices,
        AtomIndices effect_atom_indices)
        : m_novelty_base(novelty_base),
          m_atom_indices(atom_indices),
          m_effect_atom_indices(effect_atom_indices) { }
    ~TupleIndexGenerator() = default;

    tuple_index_iterator<Arity> begin() {
        return tuple_index_iterator<Arity>(m_novelty_base, m_atom_indices, m_effect_atom_indices, false);
    }

    tuple_index_iterator<Arity> end() {
        return tuple_index_iterator<Arity>(m_novelty_base, m_atom_indices, m_effect_atom_indices, true);
    }
};

}

#endif
