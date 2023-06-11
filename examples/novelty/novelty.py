#!/usr/bin/python3

"""
Example illustrating the novelty component.
"""

from dlplan.novelty import NoveltyBase, NoveltyTable


def novelty_table_example():
    """
    The example illustrates the creation and usage of NoveltyBase and NoveltyTable.
    """
    novelty_base = NoveltyBase(4, 2)
    novelty_table = NoveltyTable(novelty_base)

    # 1. Exhaustively insert all tuple indices
    has_novel = novelty_table.insert_atom_indices([0,1], stop_if_novel=False)
    assert has_novel
    # All tuple indices are not novel anymore
    has_novel = novelty_table.insert_atom_indices([0,1], stop_if_novel=False)
    assert not has_novel

    # 2. Lazy insertion of tuple indices
    has_novel = novelty_table.insert_atom_indices([1,2,3], stop_if_novel=True)
    assert has_novel
    # All tuple indices are not novel anymore
    has_novel = novelty_table.insert_atom_indices([1,2,3], stop_if_novel=True)
    assert has_novel

    # 3. Exhaustively insert all tuple indices with consideration of add atom indices.
    has_novel = novelty_table.insert_atom_indices([0,2], [1,3], stop_if_novel=False)
    assert has_novel


if __name__ == "__main__":
    novelty_table_example()
