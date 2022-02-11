#!/usr/bin/python3

import dlplan


def construct_vocabulary_info():
    vocabulary_info = dlplan.VocabularyInfo()
    # Add predicates and constants of the domain.
    # Note that there are no constants in Blocksworld.
    vocabulary_info.add_predicate("on", 2)
    vocabulary_info.add_predicate("on_g", 2)
    vocabulary_info.add_predicate("ontable", 1)
    vocabulary_info.add_predicate("holding", 1)
    vocabulary_info.add_predicate("clear", 1)
    vocabulary_info.add_predicate("arm-empty", 0)
    return vocabulary_info


def construct_instance_info(vocabulary_info):
    instance_info = dlplan.InstanceInfo(vocabulary_info)
    # Add dynamic atoms
    instance_info.add_atom("on", ["a", "b"])
    instance_info.add_atom("on", ["b", "a"])
    instance_info.add_atom("ontable", ["a"])
    instance_info.add_atom("ontable", ["b"])
    instance_info.add_atom("holding", ["a"])
    instance_info.add_atom("holding", ["b"])
    instance_info.add_atom("clear", ["a"])
    instance_info.add_atom("clear", ["b"])
    instance_info.add_atom("arm-empty", [])
    # Add static goal atoms
    instance_info.add_static_atom("on_g", ["a", "b"])
    # Add static atoms
    # Note that there are no static atoms in Blocksworld.
    return instance_info


# The simplest example to illustrate the construction and evaluation of elements.
def main():
    # 1. Initialize VocabularyInfo
    vocabulary_info = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    instance_info = construct_instance_info(vocabulary_info)
    # 3. Initialize SyntacticElementFactory
    factory = dlplan.SyntacticElementFactory(vocabulary_info)

    # 4. Construct a state.
    atoms = instance_info.get_atoms()
    a0 = atoms[0]
    a3 = atoms[3]
    a6 = atoms[6]
    state = dlplan.State(instance_info, [a0, a3, a6])

    # 5. Parse and evaluate elements.
    numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    print(f"repr: {numerical.compute_repr()}")
    print(f"value: {numerical.evaluate(state)}")

    boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    print(f"repr: {boolean.compute_repr()}")
    print(f"value: {boolean.evaluate(state)}")


if __name__ == "__main__":
    main()
