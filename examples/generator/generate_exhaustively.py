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


def main():
    # 1. Initialize VocabularyInfo
    vocabulary_info = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    instance_info = construct_instance_info(vocabulary_info)
    # 3. Initialize SyntacticElementFactory
    factory = dlplan.SyntacticElementFactory(vocabulary_info)

    # 4. Construct a bunch of states
    atoms = instance_info.get_atoms()
    a0 = atoms[0]
    a1 = atoms[1]
    a2 = atoms[2]
    a3 = atoms[3]
    a4 = atoms[4]
    a5 = atoms[5]
    a6 = atoms[6]
    a7 = atoms[7]
    a8 = atoms[8]
    s0 = dlplan.State(instance_info, [a0, a3, a6, a8])
    s1 = dlplan.State(instance_info, [a1, a2, a7, a8])
    s2 = dlplan.State(instance_info, [a2, a3, a6, a7, a8])
    s3 = dlplan.State(instance_info, [a3, a4, a7])
    s4 = dlplan.State(instance_info, [a2, a5, a6])
    states = [s0, s1, s2, s3, s4]

    # 5. Generate features up to complexity 4 with at most 180 seconds and at most 100000 features in total
    generator = dlplan.FeatureGenerator()

    features = generator.generate(factory, 10, 180, 100000, 1, states)

    for feature in features:
        print(feature)


if __name__ == "__main__":
    main()
