#include "../../include/dlplan/novelty.h"

#include <cassert>
#include <iostream>


namespace dlplan::novelty {

// https://stackoverflow.com/questions/55421835/c-binomial-coefficient-is-too-slow
static int binomial_coefficient(int n, int k) {
    int result = n - k + 1;
    for (int i = 1; i < k; ++i) {
        result = result * (n - k + 1 + i) / (i + 1);
    }
    return result;
}

// https://stackoverflow.com/questions/127704/algorithm-to-return-all-combinations-of-k-elements-from-n
template <typename Iterator>
static bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
    /* Credits: Mark Nelson http://marknelson.us */
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


TupleIndexGenerator::tuple_index_iterator::tuple_index_iterator(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const AtomIndices& atom_indices,
    bool end) :
    m_novelty_base(novelty_base),
    m_atom_indices(pad_and_sort_atom_indices(
        atom_indices,
        novelty_base->get_width(),
        novelty_base->get_dummy_atom_index())),
    m_width(novelty_base->get_width()),
    m_count(end ? binomial_coefficient(
        std::max(novelty_base->get_width(), static_cast<int>(atom_indices.size())),
        novelty_base->get_width()) : -1),
    m_atom_tuple(novelty_base->get_width()) {
    assert(static_cast<int>(m_atom_indices.size()) >= novelty_base->get_width());
    assert(std::is_sorted(m_atom_indices.begin(), m_atom_indices.end()));
    if (!end) seek_next();
}

TupleIndexGenerator::tuple_index_iterator::tuple_index_iterator(const TupleIndexGenerator::tuple_index_iterator& other) = default;

TupleIndexGenerator::tuple_index_iterator&
TupleIndexGenerator::tuple_index_iterator::operator=(const TupleIndexGenerator::tuple_index_iterator& other) = default;

TupleIndexGenerator::tuple_index_iterator::tuple_index_iterator(TupleIndexGenerator::tuple_index_iterator&& other) = default;

TupleIndexGenerator::tuple_index_iterator&
TupleIndexGenerator::tuple_index_iterator::operator=(TupleIndexGenerator::tuple_index_iterator&& other) = default;

TupleIndexGenerator::tuple_index_iterator::~tuple_index_iterator() = default;

bool
TupleIndexGenerator::tuple_index_iterator::operator!=(
    const tuple_index_iterator& other) const {
    return !(*this == other);
}

bool
TupleIndexGenerator::tuple_index_iterator::operator==(
    const tuple_index_iterator& other) const {
    return m_count == other.m_count;
}

TupleIndex
TupleIndexGenerator::tuple_index_iterator::operator*() const {
    return m_tuple_index;
}

TupleIndexGenerator::tuple_index_iterator
TupleIndexGenerator::tuple_index_iterator::operator++(int) {
    TupleIndexGenerator::tuple_index_iterator prev = *this;
    seek_next();
    return prev;
}

TupleIndexGenerator::tuple_index_iterator&
TupleIndexGenerator::tuple_index_iterator::operator++() {
    seek_next();
    return *this;
}

void TupleIndexGenerator::tuple_index_iterator::seek_next() {
    ++m_count;
    next_combination(m_atom_indices.begin(), m_atom_indices.begin() + m_width, m_atom_indices.end());
    std::copy(m_atom_indices.begin(), m_atom_indices.begin() + m_width, m_atom_tuple.begin());
    m_tuple_index = m_novelty_base->atom_tuple_to_tuple_index(m_atom_tuple);
}

TupleIndexGenerator::TupleIndexGenerator(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const AtomIndices& atom_indices)
    : m_novelty_base(novelty_base), m_atom_indices(atom_indices) { }

TupleIndexGenerator::TupleIndexGenerator(const TupleIndexGenerator& other) = default;

TupleIndexGenerator& TupleIndexGenerator::operator=(const TupleIndexGenerator& other) = default;

TupleIndexGenerator::TupleIndexGenerator(TupleIndexGenerator&& other) = default;

TupleIndexGenerator& TupleIndexGenerator::operator=(TupleIndexGenerator&& other) = default;

TupleIndexGenerator::~TupleIndexGenerator() = default;

TupleIndexGenerator::tuple_index_iterator
TupleIndexGenerator::begin() {
    return TupleIndexGenerator::tuple_index_iterator(m_novelty_base, m_atom_indices, false);
}

TupleIndexGenerator::tuple_index_iterator
TupleIndexGenerator::end() {
    return TupleIndexGenerator::tuple_index_iterator(m_novelty_base, m_atom_indices, true);
}

}
