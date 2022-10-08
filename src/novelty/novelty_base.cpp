#include "../../include/dlplan/novelty.h"

#include <vector>
#include <cassert>
#include <iostream>


namespace dlplan::novelty {

NoveltyBase::NoveltyBase(int num_atoms, int width)
    : m_num_atoms(num_atoms+1), m_width(width), m_num_tuples(std::pow(num_atoms+1, width)) {
    m_factors = std::vector<int>(m_width);
    for (int i = 0; i < m_width; ++i) {
        m_factors[i] = std::pow(m_num_atoms, i);
    }
}

NoveltyBase::NoveltyBase(const NoveltyBase& other) = default;

NoveltyBase& NoveltyBase::operator=(const NoveltyBase& other) = default;

NoveltyBase::NoveltyBase(NoveltyBase&& other) = default;

NoveltyBase& NoveltyBase::operator=(NoveltyBase&& other) = default;

NoveltyBase::~NoveltyBase() = default;

TupleIndex NoveltyBase::atom_tuple_to_tuple_index(const AtomTuple& atom_tuple) const {
    assert(static_cast<int>(atom_tuple.size()) == m_width);
    TupleIndex result = 0;
    int i = 0;
    for (auto atom_index : atom_tuple) {
        result += m_factors[i] * atom_index;
        ++i;
    }
    return result;
}

AtomTuple NoveltyBase::tuple_index_to_atom_tuple(TupleIndex tuple_index) const {
    AtomTuple result;
    for (int i = m_width-1; i >= 0; --i) {
        int atom_index = tuple_index / m_factors[i];
        if (atom_index != get_dummy_atom_index()) {
            result.push_back(atom_index);
        }
        tuple_index -= atom_index * m_factors[i];
    }
    // result is unsorted.
    return result;
}

int NoveltyBase::get_width() const {
    return m_width;
}

int NoveltyBase::get_dummy_atom_index() const {
    return m_num_atoms;
}

int NoveltyBase::get_num_atoms() const {
    return m_num_atoms;
}

int NoveltyBase::get_num_tuples() const {
    return m_num_tuples;
}

}
