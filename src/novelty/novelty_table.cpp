#include "../../include/dlplan/novelty.h"

#include "tuple_index_generator.h"

#include "../utils/collections.h"


namespace dlplan::novelty {

NoveltyTable::NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base)
    : m_novelty_base(novelty_base), m_table(pow(novelty_base->get_num_atoms()+1, novelty_base->get_arity()), true) { }

NoveltyTable::NoveltyTable(const NoveltyTable& other) = default;

NoveltyTable& NoveltyTable::operator=(const NoveltyTable& other) = default;

NoveltyTable::NoveltyTable(NoveltyTable&& other) = default;

NoveltyTable& NoveltyTable::operator=(NoveltyTable&& other) = default;

NoveltyTable::~NoveltyTable() = default;

struct ComputeNovelTupleIndicesCallback {
    const std::vector<bool>& m_table;
    TupleIndices& m_result;

    ComputeNovelTupleIndicesCallback(const std::vector<bool>& table, TupleIndices& result)
        : m_table(table), m_result(result) {}

    bool operator()(TupleIndex tuple_index) {
        if (m_table[tuple_index]) {
            m_result.push_back(tuple_index);
        }
        return false;
    }
};

TupleIndices NoveltyTable::compute_novel_tuple_indices(
    const AtomIndices& atom_indices,
    const AtomIndices& add_atom_indices) const {
    TupleIndices result;
    auto callback = ComputeNovelTupleIndicesCallback{m_table, result};
    if (m_novelty_base->get_arity() == 1) {
        for_each_tuple_index<1>(*m_novelty_base, atom_indices, add_atom_indices, callback);
    } else if (m_novelty_base->get_arity() == 2) {
        for_each_tuple_index<2>(*m_novelty_base, atom_indices, add_atom_indices, callback);
    } else {
        for_each_tuple_index(*m_novelty_base, atom_indices, add_atom_indices, callback);
    }
    return result;
}

TupleIndices NoveltyTable::compute_novel_tuple_indices(
    const AtomIndices& atom_indices) const {
    TupleIndices result;
    auto callback = ComputeNovelTupleIndicesCallback{m_table, result};
    if (m_novelty_base->get_arity() == 1) {
        for_each_tuple_index<1>(*m_novelty_base, atom_indices, callback);
    } else if (m_novelty_base->get_arity() == 2) {
        for_each_tuple_index<2>(*m_novelty_base, atom_indices, callback);
    } else {
        for_each_tuple_index(*m_novelty_base, atom_indices, callback);
    }
    return result;
}

struct InsertCallback {
    std::vector<bool>& m_table;
    bool m_stop_if_novel;
    bool& m_result;

    InsertCallback(std::vector<bool>& table, bool stop_if_novel, bool& result)
        : m_table(table), m_stop_if_novel(stop_if_novel), m_result(result) {}

    bool operator()(TupleIndex tuple_index) {
        bool is_novel = m_table[tuple_index];
        m_table[tuple_index] = false;
        if (is_novel) {
            m_result = true;
            if (m_stop_if_novel) {
                return true;
            }
        }
        return false;
    }
};

bool NoveltyTable::insert_atom_indices(
    const AtomIndices& atom_indices,
    bool stop_if_novel) {
    bool result = false;
    auto callback = InsertCallback{m_table, stop_if_novel, result};
    if (m_novelty_base->get_arity() == 1) {
        for_each_tuple_index<1>(*m_novelty_base, atom_indices, callback);
    } else if (m_novelty_base->get_arity() == 2) {
        for_each_tuple_index<2>(*m_novelty_base, atom_indices, callback);
    } else {
        for_each_tuple_index(*m_novelty_base, atom_indices, callback);
    }
    return result;
}

bool NoveltyTable::insert_atom_indices(
    const AtomIndices& atom_indices,
    const AtomIndices& add_atom_indices,
    bool stop_if_novel) {
    bool result = false;
    auto callback = InsertCallback{m_table, stop_if_novel, result};
    if (m_novelty_base->get_arity() == 1) {
        for_each_tuple_index<1>(*m_novelty_base, atom_indices, add_atom_indices, callback);
    } else if (m_novelty_base->get_arity() == 2) {
        for_each_tuple_index<2>(*m_novelty_base, atom_indices, add_atom_indices, callback);
    } else {
        for_each_tuple_index(*m_novelty_base, atom_indices, add_atom_indices, callback);
    }
    return result;
}

bool NoveltyTable::insert_tuple_indices(const TupleIndices& tuple_indices, bool stop_if_novel) {
    bool result = false;
    for (const auto tuple_index : tuple_indices) {
        assert(utils::in_bounds(tuple_index, m_table));
        bool is_novel = m_table[tuple_index];
        m_table[tuple_index] = false;
        if (is_novel) {
            result = true;
            if (stop_if_novel) {
                break;
            }
        }
    }
    return result;
}

}