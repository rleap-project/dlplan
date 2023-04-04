#!/usr/bin/python3

import dlplan


def construct_vocabulary_info():
    v = dlplan.VocabularyInfo()
    # Add predicates and constants of the domain.
    # Note that there are no constants in Blocksworld.
    v.add_predicate("on", 2)
    v.add_predicate("on_g", 2)
    v.add_predicate("ontable", 1)
    v.add_predicate("holding", 1)
    v.add_predicate("clear", 1)
    v.add_predicate("arm-empty", 0)
    return v


def construct_instance_info(v):
    i = dlplan.InstanceInfo(v)
    # Add dynamic atoms
    i.add_atom("on", ["a", "b"])
    i.add_atom("on", ["b", "a"])
    i.add_atom("ontable", ["a"])
    i.add_atom("ontable", ["b"])
    i.add_atom("holding", ["a"])
    i.add_atom("holding", ["b"])
    i.add_atom("clear", ["a"])
    i.add_atom("clear", ["b"])
    i.add_atom("arm-empty", [])
    # Add static goal atoms
    i.add_static_atom("on_g", ["a", "b"])
    # Add static atoms
    # Note that there are no static atoms in Blocksworld.
    return i


def main():
    # 1. Initialize VocabularyInfo
    v = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    i = construct_instance_info(v)
    # 3. Initialize SyntacticElementFactory
    f = dlplan.SyntacticElementFactory(v)

    # 4. Construct a bunch of states
    atoms = i.get_atoms()
    a0 = atoms[0]
    a1 = atoms[1]
    a2 = atoms[2]
    a3 = atoms[3]
    a4 = atoms[4]
    a5 = atoms[5]
    a6 = atoms[6]
    a7 = atoms[7]
    a8 = atoms[8]
    s0 = dlplan.State(i, [a0, a3, a6, a8])
    s1 = dlplan.State(i, [a1, a2, a7, a8])
    s2 = dlplan.State(i, [a2, a3, a6, a7, a8])
    s3 = dlplan.State(i, [a3, a4, a7])
    s4 = dlplan.State(i, [a2, a5, a6])
    states = [s0, s1, s2, s3, s4]

    # 5. Generate features up to complexity 4 with at most 180 seconds and at most 100000 features in total
    generator = dlplan.FeatureGenerator()
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

    features = generator.generate(f, states, 5, 5, 10, 10, 10, 180, 100000)

    for feature in features:
        print(feature)


if __name__ == "__main__":
    main()
