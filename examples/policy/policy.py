#!/usr/bin/python3

"""
Example illustrating the policy component.
"""

from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory, \
    DenotationsCaches
from dlplan.policy import PolicyFactory, PositiveBooleanEffect, DecrementNumericalEffect


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
    vocabulary = construct_vocabulary_info()
    instance = construct_instance_info(vocabulary)
    element_factory = SyntacticElementFactory(vocabulary)

    print("Constructing policy:")
    policy_factory = PolicyFactory(element_factory)
    # boolean represents whether the hand is empty or not
    boolean = policy_factory.make_boolean("b0", element_factory.parse_boolean("b_nullary(arm-empty)"))
    # numerical representes the number of blocks on top of another block
    numerical = policy_factory.make_numerical("n1", element_factory.parse_numerical("n_count(r_primitive(on,0,1))"))
    b_pos_condition_0 = policy_factory.make_pos_condition(boolean)
    b_bot_effect_0 = policy_factory.make_bot_effect(boolean)
    n_gt_condition_0 = policy_factory.make_gt_condition(numerical)
    n_dec_effect_0 = policy_factory.make_dec_effect(numerical)
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
    state_0 = State(0, instance, [atom_0, atom_3, atom_6, atom_8])  # a on b
    state_1 = State(1, instance, [atom_1, atom_2, atom_7, atom_8])  # b on a
    state_2 = State(2, instance, [atom_2, atom_3, atom_6, atom_7, atom_8])  # a,b on table

    caches = DenotationsCaches()
    assert policy.evaluate(state_0, state_2, caches)
    assert policy.evaluate(state_1, state_2, caches)
    assert not policy.evaluate(state_2, state_0, caches)
    assert not policy.evaluate(state_2, state_1, caches)

    print(str(policy))
    print()

    print("Parsing policy:")
    policy_str = "(:policy\n" \
        "(:booleans (b0 \"b_nullary(arm-empty)\"))\n" \
        "(:numericals (n0 \"n_count(r_primitive(on,0,1))\"))\n" \
        "(:rule (:conditions (:c_b_pos b0) (:c_n_gt n0)) (:effects (:e_b_bot b0) (:e_n_dec n0)))\n" \
        ")"
    policy_in = policy_factory.parse_policy(policy_str)
    print(str(policy_in))
    print()


if __name__ == "__main__":
    main()
