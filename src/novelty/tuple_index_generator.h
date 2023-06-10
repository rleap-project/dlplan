#ifndef DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_TUPLE_INDEX_GENERATOR_H_

#include "../../include/dlplan/novelty.h"

#include "../utils/math.h"
#include "../utils/logging.h"

#include <functional>


namespace dlplan::novelty {

// https://stackoverflow.com/questions/127704/algorithm-to-return-all-combinations-of-k-elements-from-n
// https://stackoverflow.com/questions/4973077/the-amortized-complexity-of-stdnext-permutation
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


/// @brief
/// @tparam Arity
/// @param novelty_base
/// @param atom_indices
/// @param callback
template<int Arity=-1>
void for_each_tuple_index(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(int)>& callback) {
    int arity = novelty_base.get_arity();
    // Add placeholders to be able to generate tuples of size less than arity.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atom_indices = static_cast<int>(atom_indices.size());
    // Initialize iteration indices.
    std::vector<int> indices(arity);
    AtomIndices atom_tuple_indices(arity);
    for (int i = 0; i < arity; ++i) {
        int index = std::min(i, num_atom_indices);
        indices[i] = index;
        atom_tuple_indices[i] = atom_indices[index];
    }
    TupleIndex tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
    while (true) {
        bool finished = callback(tuple_index);
        if (finished) return;
        // Find the rightmost index to increment
        int i = arity - 1;
        while (i >= 0 && (indices[i] >= num_atom_indices - 1)) {
            --i;
        }
        if (i < 0) {
            // Exit the loop when all indices have reached their maximum values
            return;
        }
        int index = ++indices[i];
        atom_tuple_indices[i] = atom_indices[index];
        // Update indices right of the incremented rightmost index i.
        for (int j = i + 1; j < arity; ++j) {
            // Ensure that index is not larger than index of place holder.
            int index = std::min(num_atom_indices - 1, indices[j-1] + 1);
            indices[j] = index;
            atom_tuple_indices[j] = atom_indices[index];
        }
        tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
    }
}

template<>
void for_each_tuple_index<1>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(int)>& callback);

template<>
void for_each_tuple_index<2>(
    const NoveltyBase &novelty_base,
    AtomIndices atom_indices,
    const std::function<bool(int)>& callback);


/// @brief
/// @tparam Arity
/// @param novelty_base
/// @param atom_indices
/// @param add_atom_indices
/// @param callback
template<int Arity=-1>
void for_each_tuple_index(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(int)>& callback) {
    int arity = novelty_base.get_arity();
    //throw std::runtime_error("for_each_tuple_index - no general implementation available for overload with add_atom_indices.");
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    assert(std::is_sorted(add_atom_indices.begin(), add_atom_indices.end()));
    // Add placeholders to be able to not pick an atom from atom_indices.
    int place_holder = novelty_base.get_num_atoms();
    atom_indices.push_back(place_holder);
    int num_atom_indices = static_cast<int>(atom_indices.size());
    int num_add_atom_indices = static_cast<int>(add_atom_indices.size());
    // Initialize book-keeping for efficient sorted iteration.
    std::vector<int> geq_indices(num_atom_indices, std::numeric_limits<int>::max());
    std::vector<int> geq_indices_add(num_add_atom_indices, std::numeric_limits<int>::max());
    int j = 0;
    int i = 0;
    while (j < static_cast<int>(geq_indices.size()) && i < static_cast<int>(geq_indices_add.size())) {
        if (atom_indices[j] < add_atom_indices[i]) {
            geq_indices[j] = i;
            ++j;
        } else if (atom_indices[j] > add_atom_indices[i]) {
            geq_indices_add[i] = j;
            ++i;
        } else {
            geq_indices_add[i] = j;
            geq_indices[j] = i;
            ++j;
            ++i;
        }
    }
    // Iteration that selects at least one atom index from add_atom_indices.
    AtomIndices atom_tuple_indices(arity);
    std::vector<AtomIndices> a_indices{std::move(atom_indices), std::move(add_atom_indices)};
    std::vector<int> a_num_atom_indices{num_atom_indices, num_add_atom_indices};
    std::vector<std::vector<int>> a_geq{std::move(geq_indices), std::move(geq_indices_add)};
    std::vector<int> a(arity);
    for (int k = 1; k < std::pow(2, arity); ++k) {
        int tmp = k;
        for (int i = 0; i < arity; ++i) {
            a[i] = (tmp & 1) > 0;
            tmp >>= 1;
        }
        // Initialize iteration indices.
        std::vector<int> indices(arity);
        AtomIndices atom_tuple_indices(arity);
        indices[0] = 0;
        atom_tuple_indices[0] = a_indices[a[0]][0];
        bool exhausted = false;
        for (int i = 1; i < arity; ++i) {
            int index = (a[i-1] != a[i]) ? a_geq[a[i-1]][indices[i-1]] : indices[i-1] + 1;
            if (index == std::numeric_limits<int>::max()) {
                exhausted = true;
                break;
            }
            indices[i] = index;
            atom_tuple_indices[i] = a_indices[a[i]][index];
        }
        if (exhausted) {
            continue;
        }
        TupleIndex tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
        while (true) {
            bool finished = callback(tuple_index);
            if (finished) return;
            // Find the rightmost index to increment
            int i = arity - 1;
            while (i >= 0 && (indices[i] >= a_num_atom_indices[a[i]] - 1)) {
                --i;
            }
            if (i < 0) {
                // Exit the loop when all indices have reached their maximum values
                break;
            }
            int index = ++indices[i];
            atom_tuple_indices[i] = a_indices[a[i]][index];
            // Update indices right of the incremented rightmost index i.
            bool exhausted = false;
            for (int j = i + 1; j < arity; ++j) {
                // Ensure that index is not larger than index of place holder.
                int index = (a[j-1] != a[j]) ? a_geq[a[j-1]][indices[j-1]] : std::min(a_num_atom_indices[a[j]] - 1, indices[j-1] + 1);
                if (index == std::numeric_limits<int>::max()) {
                    // no atom larger atom index can be appended.
                    exhausted = true;
                    break;
                }
                indices[j] = index;
                if (a[j-1] == a[j] && a[j] == 1 && indices[j-1] == indices[j]) {
                    // duplicate atom indices from add_atom_indices
                    exhausted = true;
                    break;
                }
                atom_tuple_indices[j] = a_indices[a[j]][index];
            }
            if (exhausted) {
                break;
            }
            tuple_index = novelty_base.atom_tuple_to_tuple_index(atom_tuple_indices);
        }
    }
}


template<>
void for_each_tuple_index<1>(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(int)>& callback);


template<>
void for_each_tuple_index<2>(
    const NoveltyBase& novelty_base,
    AtomIndices atom_indices,
    AtomIndices add_atom_indices,
    const std::function<bool(int)>& callback);


template<int Arity=-1>
class tuple_index_iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = TupleIndex;
    using distance_type     = std::ptrdiff_t;

    const static int end_value = -1;

    /// @brief Implements an iterator over all sorted pairs of atom indices
    ///        of size specificed by novelty base.
    /// @param novelty_base
    /// @param atom_indices
    /// @param add_atom_indices
    /// @param end
    tuple_index_iterator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        AtomIndices atom_indices,
        AtomIndices add_atom_indices,
        bool end=false)
        : m_novelty_base(novelty_base),
          m_atom_indices(atom_indices),
          m_add_atom_indices(add_atom_indices),
          m_arity(novelty_base->get_arity()) {
        if (atom_indices.size() > 0) {
            throw std::runtime_error("tuple_index_iterator::tuple_index_iterator - generic implementation does currently not support atom_indices.");
        }
        int place_holder = novelty_base->get_num_atoms();
        // add placeholders to be able to generate tuples of size less than arity.
        m_add_atom_indices.insert(m_add_atom_indices.end(), m_arity, place_holder);
        m_tuple_index = (end) ? end_value : m_novelty_base->atom_tuple_to_tuple_index(
            AtomIndices(m_add_atom_indices.begin(), m_add_atom_indices.begin() + m_arity));
        assert(std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()));
        assert(std::is_sorted(m_add_atom_indices.begin(), m_add_atom_indices.end()));
    }
    ~tuple_index_iterator() = default;

    bool operator!=(const tuple_index_iterator& other) const {
        return !(*this == other);
    }

    bool operator==(const tuple_index_iterator& other) const {
        return m_tuple_index == other.m_tuple_index;
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
    AtomIndices m_add_atom_indices;
    int m_arity;
    // the output, i.e., the index of the atom tuple
    TupleIndex m_tuple_index;


private:
    void seek_next() {
        bool in_progress = next_combination(m_add_atom_indices.begin(), m_add_atom_indices.begin() + m_arity, m_add_atom_indices.end());
        m_tuple_index = m_novelty_base->atom_tuple_to_tuple_index(
            AtomIndices(m_add_atom_indices.begin(), m_add_atom_indices.begin() + m_arity));
        if (!in_progress) m_tuple_index = end_value;
    }
};


template<>
class tuple_index_iterator<1> {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = TupleIndex;
    using distance_type     = std::ptrdiff_t;

    const static int end_value = -1;

    /// @brief Implements an iterator over all sorted pairs of atom indices
    ///        of size specificed by novelty base.
    /// @param novelty_base
    /// @param atom_indices
    /// @param add_atom_indices
    /// @param end
    tuple_index_iterator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        AtomIndices,
        AtomIndices add_atom_indices,
        bool end=false)
        : m_novelty_base(novelty_base),
          m_add_atom_indices(add_atom_indices),
          m_pos(end ? m_add_atom_indices.size()+1 : 0) {
        assert(1 == novelty_base->get_arity());
        int place_holder = novelty_base->get_num_atoms();
        // add one placeholder to generate the empty tuple.
        // add an additional place holder to be able to remove a bound check in the seek next method.
        m_add_atom_indices.insert(m_add_atom_indices.end(), 2, place_holder);
        m_tuple_index = m_novelty_base->atom_tuple_to_tuple_index({m_add_atom_indices[0]});
    }
    ~tuple_index_iterator() = default;

    bool operator!=(const tuple_index_iterator& other) const {
        return !(*this == other);
    }

    bool operator==(const tuple_index_iterator& other) const {
        return m_pos == other.m_pos;
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
    AtomIndices m_add_atom_indices;
    size_t m_pos;
    // the output, i.e., the index of the atom tuple
    TupleIndex m_tuple_index;

private:
    void seek_next() {
        m_tuple_index = m_novelty_base->atom_tuple_to_tuple_index({m_add_atom_indices[++m_pos]});
    }
};


/// @brief Implements an iterator over all tuples of atom indices of size Arity
///        from a given set of atom indices.
template<int Arity=-1>
class TupleIndexGenerator {
private:
    std::shared_ptr<const NoveltyBase> m_novelty_base;
    AtomIndices m_atom_indices;
    AtomIndices m_add_atom_indices;

public:
    TupleIndexGenerator(
        std::shared_ptr<const NoveltyBase> novelty_base,
        const AtomIndices& atom_indices,
        const AtomIndices& add_atom_indices)
        : m_novelty_base(novelty_base),
          m_atom_indices(atom_indices),
          m_add_atom_indices(add_atom_indices) {
        std::sort(m_atom_indices.begin(), m_atom_indices.end());
        std::sort(m_add_atom_indices.begin(), m_add_atom_indices.end());
    }
    ~TupleIndexGenerator() = default;

    tuple_index_iterator<Arity> begin() {
        return tuple_index_iterator<Arity>(m_novelty_base, m_atom_indices, m_add_atom_indices, false);
    }

    tuple_index_iterator<Arity> end() {
        return tuple_index_iterator<Arity>(m_novelty_base, m_atom_indices, m_add_atom_indices, true);
    }
};

}

#endif
