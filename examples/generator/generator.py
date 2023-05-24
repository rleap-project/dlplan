#!/usr/bin/python3

from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory
from dlplan.generator import FeatureGenerator


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


def main():
    # 1. Initialize VocabularyInfo
    vocabulary = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    instance = construct_instance_info(vocabulary)
    # 3. Initialize SyntacticElementFactory
    factory = SyntacticElementFactory(vocabulary)

    # 4. Construct a bunch of states
    atoms = instance.get_atoms()
    atom_0 = atoms[0]
    atom_1 = atoms[1]
    atom_2 = atoms[2]
    atom_3 = atoms[3]
    atom_4 = atoms[4]
    atom_5 = atoms[5]
    atom_6 = atoms[6]
    atom_7 = atoms[7]
    atom_8 = atoms[8]
    state_1 = State(instance, [atom_0, atom_3, atom_6, atom_8])
    state_2 = State(instance, [atom_1, atom_2, atom_7, atom_8])
    state_3 = State(instance, [atom_2, atom_3, atom_6, atom_7, atom_8])
    state_4 = State(instance, [atom_3, atom_4, atom_7])
    state_5 = State(instance, [atom_2, atom_5, atom_6])
    states = [state_1, state_2, state_3, state_4, state_5]

    # 5. Generate features up to complexity 4 with at most 180 seconds and at most 100000 features in total
    generator = FeatureGenerator()
    generator.set_generate_inclusion_boolean(False)
    generator.set_generate_diff_concept(False)
    generator.set_generate_or_concept(False)
    generator.set_generate_subset_concept(False)
    generator.set_generate_and_role(False)
    generator.set_generate_compose_role(False)
    generator.set_generate_diff_role(False)
    generator.set_generate_identity_role(False)
    generator.set_generate_not_role(False)
    generator.set_generate_or_role(False)
    generator.set_generate_top_role(False)
    generator.set_generate_transitive_reflexive_closure_role(False)

    features = generator.generate(factory, states, 5, 5, 10, 10, 10, 180, 100000)

    for feature in features:
        print(feature)


if __name__ == "__main__":
    main()
