#include "../../include/dlplan/novelty.h"

#include "tuple_index_generator.h"

#include "../utils/collections.h"


namespace dlplan::novelty {

NoveltyTable::NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base)
    : m_novelty_base(novelty_base), m_table(pow(novelty_base->get_num_atoms()+1, novelty_base->get_arity()), true) {
}

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
    for_each_tuple_index(*m_novelty_base, atom_indices, add_atom_indices, callback);
    return result;
}

TupleIndices NoveltyTable::compute_novel_tuple_indices(
    const AtomIndices& atom_indices) const {
    TupleIndices result;
    auto callback = ComputeNovelTupleIndicesCallback{m_table, result};
    for_each_tuple_index(*m_novelty_base, atom_indices, callback);
    return result;
}

struct InsertCallback {
    std::vector<bool>& m_table;
    bool m_stop_if_novel;
    bool& m_result;

    InsertCallback(std::vector<bool>& table, bool stop_if_novel, bool& result)
        : m_table(table), m_stop_if_novel(stop_if_novel), m_result(result) {}

    bool operator()(TupleIndex tuple_index) {
        assert(tuple_index < static_cast<int>(m_table.size()) && tuple_index >= 0);
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
    for_each_tuple_index(*m_novelty_base, atom_indices, callback);
    return result;
}

bool NoveltyTable::insert_atom_indices(
    const AtomIndices& atom_indices,
    const AtomIndices& add_atom_indices,
    bool stop_if_novel) {
    bool result = false;
    auto callback = InsertCallback{m_table, stop_if_novel, result};
    for_each_tuple_index(*m_novelty_base, atom_indices, add_atom_indices, callback);
    return result;
}

bool NoveltyTable::insert_tuple_indices(const TupleIndices& tuple_indices, bool stop_if_novel) {
    bool result = false;
    for (const auto tuple_index : tuple_indices) {
        assert(tuple_index < static_cast<int>(m_table.size()) && tuple_index >= 0);
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

void NoveltyTable::resize(std::shared_ptr<const NoveltyBase> novelty_base) {
    if (novelty_base->get_arity() != m_novelty_base->get_arity()) {
        throw std::runtime_error("NoveltyTable::resize - missmatched arity of novelty_table and novelty_base.");
    }
    m_table.resize(pow(novelty_base->get_num_atoms()+1, novelty_base->get_arity()), true);
    m_novelty_base = novelty_base;
}

}