#include "../../include/dlplan/novelty.h"

#include <cassert>
#include <iostream>


namespace dlplan::novelty {

template <typename Iterator>
bool next_combination(const Iterator first, Iterator k, const Iterator last)
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


TupleIndexGenerator::tuple_index_iterator::tuple_index_iterator(
    std::shared_ptr<const NoveltyBase> novelty_base,
    const AtomIndices& atom_indices,
    bool end) :
    m_novelty_base(novelty_base),
    m_atom_indices(atom_indices),
    m_width(novelty_base->get_width()),
    m_count(end ? ((atom_indices.size() - novelty_base->get_width() + 1) * (atom_indices.size() - novelty_base->get_width() + 2) / 2) : 0),
    m_atom_tuple(novelty_base->get_width()) {
    seek_next();
    // It is required to pad atom_indices with an additional dummy atom
    // in advance if atom_indices.size() < width.
    assert(static_cast<int>(atom_indices.size()) >= novelty_base->get_width());
    m_tuple_index = novelty_base->atom_tuple_to_tuple_index(m_atom_tuple);
}

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

TupleIndexGenerator::tuple_index_iterator
TupleIndexGenerator::begin() {
    return TupleIndexGenerator::tuple_index_iterator(m_novelty_base, m_atom_indices, false);
}

TupleIndexGenerator::tuple_index_iterator
TupleIndexGenerator::end() {
    return TupleIndexGenerator::tuple_index_iterator(m_novelty_base, m_atom_indices, true);
}

}
