#include "../../include/dlplan/novelty.h"


namespace dlplan::novelty {

NoveltyTable::NoveltyTable(int num_tuples)
    : m_table(num_tuples, true) { }

NoveltyTable::NoveltyTable(const NoveltyTable& other) = default;

NoveltyTable& NoveltyTable::operator=(const NoveltyTable& other) = default;

NoveltyTable::NoveltyTable(NoveltyTable&& other) = default;

NoveltyTable& NoveltyTable::operator=(NoveltyTable&& other) = default;

NoveltyTable::~NoveltyTable() = default;

bool NoveltyTable::insert(TupleIndexGenerator&& tuple_index_generator, bool stop_if_novel) {
    bool result = false;
    for (auto tuple_index : tuple_index_generator) {
        bool is_novel = m_table[tuple_index];
        if (!result && is_novel) {
            result = true;
        }
        m_table[tuple_index] = false;
        if (stop_if_novel) {
            break;
        }
    }
    return result;
}

}