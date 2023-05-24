#!/usr/bin/python3

from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory, DenotationsCaches
import dlplan

def construct_vocabulary_info():
    vocabulary = VocabularyInfo()
    # Add predicates and constants of the domain.
    # Note that there are no constants in Blocksworld.
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    vocabulary.add_predicate("arm-empty", 0)
    return vocabulary


def construct_instance_info(vocabulary):
    instance = InstanceInfo(vocabulary)
    # Add dynamic atoms
    instance.add_atom("on", ["a", "b"])
    instance.add_atom("on", ["b", "a"])
    instance.add_atom("ontable", ["a"])
    instance.add_atom("ontable", ["b"])
    instance.add_atom("holding", ["a"])
    instance.add_atom("holding", ["b"])
    instance.add_atom("clear", ["a"])
    instance.add_atom("clear", ["b"])
    instance.add_atom("arm-empty", [])
    # Add static goal atoms
    instance.add_static_atom("on_g", ["a", "b"])
    # Add static atoms
    # Note that there are no static atoms in Blocksworld.
    return instance


# The simplest example to illustrate the construction and evaluation of elements.
def main():
    # 1. Initialize VocabularyInfo
    vocabulary = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    instance = construct_instance_info(vocabulary)
    # 3. Initialize SyntacticElementFactory
    factory = SyntacticElementFactory(vocabulary)

    # 4. Construct a state.
    atoms = instance.get_atoms()
    atom_0 = atoms[0]
    atom_1 = atoms[1]
    atom_3 = atoms[3]
    atom_6 = atoms[6]
    state_1 = State(instance, [atom_0, atom_3, atom_6])
    state_2 = State(instance, [atom_1, atom_3, atom_6])

    # 5. Parse and evaluate elements.
    numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    print(f"repr: {repr(numerical)}")
    print(f"value: {numerical.evaluate(state_1)}")

    boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    print(f"repr: {repr(boolean)}")
    print(f"value: {boolean.evaluate(state_1)}")

    denotations_caches = DenotationsCaches()
    concept = factory.parse_concept("c_and(c_primitive(on_g,0),c_primitive(on,0))")
    denotations = concept.evaluate([state_1, state_2], denotations_caches)
    for denotation in denotations:
        print(denotation.to_sorted_vector())

    role = factory.parse_role("r_and(r_primitive(on_g,0,1),r_primitive(on,0,1))")
    denotations = role.evaluate([state_1, state_1, state_2], denotations_caches)
    for denotation in denotations:
        print(denotation.to_sorted_vector())


if __name__ == "__main__":
    main()
