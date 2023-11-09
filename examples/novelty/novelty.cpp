/// Example illustrating the novelty component.

#include "../../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::novelty;

/// @brief The example illustrates the creation and usage of NoveltyBase and
///        NoveltyTable.
static void novelty_table_example() {
    auto novelty_base = std::make_shared<NoveltyBase>(4, 2);
    auto novelty_table = NoveltyTable(novelty_base);

    // 1. Exhaustively insert all tuple indices
    bool has_novel = novelty_table.insert_atom_indices({0,1}, false);
    assert(has_novel);
    // All tuple indices are not novel anymore
    has_novel = novelty_table.insert_atom_indices({0,1}, false);
    assert(!has_novel);

    // 2. Lazy insertion of tuple indices
    has_novel = novelty_table.insert_atom_indices({1,2,3}, true);
    assert(has_novel);
    // All tuple indices are not novel anymore
    has_novel = novelty_table.insert_atom_indices({1,2,3}, true);
    assert(has_novel);

    // 3. Exhaustively insert all tuple indices with consideration of add atom indices.
    has_novel = novelty_table.insert_atom_indices({0,2}, {1,3}, false);
    assert(has_novel);
}

int main() {
    novelty_table_example();
}
