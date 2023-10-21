#!/usr/bin/python3

"""
Example illustrating the policy component.
"""

from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory, \
    DenotationsCaches
from dlplan.policy import PolicyFactory


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
    # User must ensure that each index gets its unique index for caching.
    instance = InstanceInfo(vocabulary, index=0)
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
    vocabulary = construct_vocabulary_info()
    instance = construct_instance_info(vocabulary)
    element_factory = SyntacticElementFactory(vocabulary)

    print("Constructing policy:")
    # boolean_1 represents whether the hand is empty or not
    boolean_1 = element_factory.parse_boolean("b_nullary(arm-empty)")
    # numerical_1 representes the number of blocks on top of another block
    numerical_1 = element_factory.parse_numerical("n_count(r_primitive(on,0,1))")

    policy_factory = PolicyFactory(element_factory)
    b_pos_condition_0 = policy_factory.make_pos_condition(boolean_1)
    b_bot_effect_0 = policy_factory.make_bot_effect(boolean_1)
    n_gt_condition_0 = policy_factory.make_gt_condition(numerical_1)
    n_dec_effect_0 = policy_factory.make_dec_effect(numerical_1)
    rule_1 = policy_factory.make_rule(
        {b_pos_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    )
    policy = policy_factory.make_policy({rule_1})

    atoms = instance.get_atoms()
    atom_0 = atoms[0]
    atom_1 = atoms[1]
    atom_2 = atoms[2]
    atom_3 = atoms[3]
    atom_6 = atoms[6]
    atom_7 = atoms[7]
    atom_8 = atoms[8]
    # User must ensure that each State gets its unique index for caching.
    state_0 = State(instance, [atom_0, atom_3, atom_6, atom_8], 0)  # a on b
    state_1 = State(instance, [atom_1, atom_2, atom_7, atom_8], 1)  # b on a
    state_2 = State(instance, [atom_2, atom_3, atom_6, atom_7, atom_8], 2)  # a,b on table

    caches = DenotationsCaches()
    assert policy.evaluate(state_0, state_2, caches)
    assert policy.evaluate(state_1, state_2, caches)
    assert not policy.evaluate(state_2, state_0, caches)
    assert not policy.evaluate(state_2, state_1, caches)

    print(repr(policy))
    print(str(policy))
    print()

    print("Parsing policy:")
    policy_str = "(:policy\n" \
        "(:booleans (b0 \"b_nullary(arm-empty)\"))\n" \
        "(:numericals (n0 \"n_count(r_primitive(on,0,1))\"))\n" \
        "(:rule (:conditions (:c_b_pos b0) (:c_n_gt n0)) (:effects (:e_b_bot b42) (:e_n_dec n0)))\n" \
        ")"
    policy_in = policy_factory.parse_policy(policy_str)
    print(repr(policy_in))
    print(str(policy_in))
    print()


if __name__ == "__main__":
    main()
