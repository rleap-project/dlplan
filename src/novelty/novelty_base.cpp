#include "../../include/dlplan/novelty.h"

#include "../utils/math.h"

#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>


namespace dlplan::novelty {

NoveltyBase::NoveltyBase(int num_atoms, int arity)
    : m_num_atoms(num_atoms+1), m_arity(arity) {
    if (m_arity < 0) {
        throw std::runtime_error("NoveltyBase::NoveltyBase - arity must be greater than or equal to 0.");
    }
    m_factors = std::vector<int>(m_arity);
    m_num_tuples = 0;
    for (int i = 0; i < m_arity; ++i) {
        m_factors[i] = std::pow(m_num_atoms, i);
        m_num_tuples += m_num_atoms * m_factors[i];
    }
}

NoveltyBase::NoveltyBase(const NoveltyBase& other) = default;

NoveltyBase& NoveltyBase::operator=(const NoveltyBase& other) = default;

NoveltyBase::NoveltyBase(NoveltyBase&& other) = default;

NoveltyBase& NoveltyBase::operator=(NoveltyBase&& other) = default;

NoveltyBase::~NoveltyBase() = default;

TupleIndex NoveltyBase::atom_tuple_to_tuple_index(const AtomIndices& tuple_atom_indices) const {
    assert(static_cast<int>(tuple_atom_indices.size()) == m_arity);
    TupleIndex result = 0;
    int i = 0;
    for (auto atom_index : tuple_atom_indices) {
        result += m_factors[i] * atom_index;
        ++i;
    }
    return result;
}

AtomIndices NoveltyBase::tuple_index_to_atom_tuple(TupleIndex tuple_index) const {
    AtomIndices result;
    for (int i = m_arity-1; i >= 0; --i) {
        int atom_index = tuple_index / m_factors[i];
        if (atom_index != get_num_atoms()) {  // index of dummy atom
            result.push_back(atom_index);
        }
        tuple_index -= atom_index * m_factors[i];
    }
    // result is unsorted.
    return result;
}

int NoveltyBase::get_num_atoms() const {
    return m_num_atoms;
}

int NoveltyBase::get_arity() const {
    return m_arity;
}

}
