#include "../../include/dlplan/novelty.h"

#include "tuple_index_generator.h"

#include "../utils/collections.h"


namespace dlplan::novelty {

template<typename T>
static TupleIndices compute_novel_tuple_indices_with_generator(T&& tuple_index_generator, const std::vector<bool>& table) {
    TupleIndices result;
    for (const auto tuple_index : tuple_index_generator) {
        if (table[tuple_index]) {
            result.push_back(tuple_index);
        }
    }
    return result;
}


template<typename T>
static bool insert_with_generator(T&& tuple_index_generator, std::vector<bool>& table, bool stop_if_novel) {
    bool result = false;
    for (const auto tuple_index : tuple_index_generator) {
        bool is_novel = table[tuple_index];
        if (!result && is_novel) {
            result = true;
        }
        table[tuple_index] = false;
        if (stop_if_novel && is_novel) {
            break;
        }
    }
    return result;
}


NoveltyTable::NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base, int num_atoms)
    : m_novelty_base(novelty_base), m_table(pow(num_atoms, novelty_base->get_arity()), true) { }

NoveltyTable::NoveltyTable(const NoveltyTable& other) = default;

NoveltyTable& NoveltyTable::operator=(const NoveltyTable& other) = default;

NoveltyTable::NoveltyTable(NoveltyTable&& other) = default;

NoveltyTable& NoveltyTable::operator=(NoveltyTable&& other) = default;

NoveltyTable::~NoveltyTable() = default;

void NoveltyTable::reset_novelty(const TupleIndices& tuple_indices) {
    for (const auto tuple_index : tuple_indices) {
        m_table[tuple_index] = false;
    }
}

TupleIndices NoveltyTable::compute_novel_tuple_indices(
    const AtomIndices& atom_indices,
    const AtomIndices& effect_atom_indices) const {
    TupleIndices result;
    if (m_novelty_base->get_arity() == 1) {
        return compute_novel_tuple_indices_with_generator(TupleIndexGenerator<1>(m_novelty_base, atom_indices, effect_atom_indices), m_table);
    } else if (m_novelty_base->get_arity() == 2) {
        return compute_novel_tuple_indices_with_generator(TupleIndexGenerator<2>(m_novelty_base, atom_indices, effect_atom_indices), m_table);
    } else {
        return compute_novel_tuple_indices_with_generator(TupleIndexGenerator<-1>(m_novelty_base, atom_indices, effect_atom_indices), m_table);
    }
    return result;
}

bool NoveltyTable::insert(
    const AtomIndices& atom_indices,
    const AtomIndices& effect_atom_indices,
    bool stop_if_novel) {
    bool result = false;
    if (m_novelty_base->get_arity() == 1) {
        return insert_with_generator(TupleIndexGenerator<1>(m_novelty_base, atom_indices, effect_atom_indices), m_table, stop_if_novel);
    } else if (m_novelty_base->get_arity() == 2) {
        return insert_with_generator(TupleIndexGenerator<2>(m_novelty_base, atom_indices, effect_atom_indices), m_table, stop_if_novel);
    } else {
        return insert_with_generator(TupleIndexGenerator<-1>(m_novelty_base, atom_indices, effect_atom_indices), m_table, stop_if_novel);
    }
    return result;
}

bool NoveltyTable::insert(const TupleIndices& tuple_indices, bool stop_if_novel) {
    bool result = false;
    for (const auto tuple_index : tuple_indices) {
        assert(utils::in_bounds(tuple_index, m_table));
        bool is_novel = m_table[tuple_index];
        if (!result && is_novel) {
            result = true;
        }
        m_table[tuple_index] = false;
        if (stop_if_novel && is_novel) {
            break;
        }
    }
    return result;
}

}