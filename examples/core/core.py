#!/usr/bin/python3

"""
Example illustrating the core component.
"""

from dlplan.core import VocabularyInfo, InstanceInfo, State, \
    SyntacticElementFactory, DenotationsCaches


def construct_vocabulary_info():
    """ Construct a VocabularyInfo for the Blocks domain.

    Create an empty VocabularyInfo and then add predicates, and constants.

    Returns: the VocabularyInfo
    """
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    vocabulary.add_predicate("arm-empty", 0)
    return vocabulary


def construct_instance_info(vocabulary):
    """ Construct an InstanceInfo over the Blocks domain.

    Create an empty InstanceInfo and then add objects, atoms, and static atoms.

    Returns: the InstanceInfo
    """
    # User must ensure that each InstanceInfo gets its unique index for caching.
    instance = InstanceInfo(0, vocabulary)
    instance.add_atom("on", ["a", "b"])
    instance.add_atom("on", ["b", "a"])
    instance.add_atom("ontable", ["a"])
    instance.add_atom("ontable", ["b"])
    instance.add_atom("holding", ["a"])
    instance.add_atom("holding", ["b"])
    instance.add_atom("clear", ["a"])
    instance.add_atom("clear", ["b"])
    instance.add_atom("arm-empty", [])
    instance.add_static_atom("on_g", ["a", "b"])
    return instance


def main():
    """ Example illustrating the core component on a fragment of a planning
    problem over the Blocks domain.

    The example illustrates the creation of VocabularyInfo, InstanceInfo,
    State, Element, and the evaluation of Element on a State using
    DenotationsCaches.
    """
    vocabulary = construct_vocabulary_info()
    instance = construct_instance_info(vocabulary)
    factory = SyntacticElementFactory(vocabulary)

    atoms = instance.get_atoms()
    atom_0 = atoms[0]
    atom_1 = atoms[1]
    atom_3 = atoms[3]
    atom_6 = atoms[6]
    # User must ensure that each State gets its unique index for caching.
    state_0 = State(0, instance, [atom_0, atom_3, atom_6])  # a on b
    state_1 = State(1, instance, [atom_1, atom_3, atom_6])  # b on a
    states = [state_0, state_1]

    numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    print("Element repr:", repr(numerical))
    print()

    print("Evaluate for single state without cache.")
    print("State:", str(state_0))
    print("Value:", numerical.evaluate(state_0))
    print()

    caches = DenotationsCaches()
    print("Evaluate for single state with cache.")
    print("State:", str(state_1))
    print("Value:", numerical.evaluate(state_1, caches))
    print()

    print("Evaluate for multiple states with cache.")
    denotations = numerical.evaluate(states, caches)
    for state, value in zip(states, denotations):
        print("State:", str(state))
        print("Value:", value)


if __name__ == "__main__":
    main()
