from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory, DenotationsCaches
from dlplan.policy import PolicyBuilder, PolicyWriter, PolicyReader


def main():
    vocabulary_info = VocabularyInfo()
    vocabulary_info.add_predicate("unary", 1)
    factory = SyntacticElementFactory(vocabulary_info)
    boolean_1 = factory.parse_boolean("b_empty(c_primitive(unary,0))")
    numerical_1 = factory.parse_numerical("n_count(c_primitive(unary,0))")

    builder = PolicyBuilder()
    b_neg_condition_0 = builder.add_neg_condition(boolean_1)
    b_bot_effect_0 = builder.add_bot_effect(boolean_1)
    n_gt_condition_0 = builder.add_gt_condition(numerical_1)
    n_dec_effect_0 = builder.add_dec_effect(numerical_1)
    rule_1 = builder.add_rule(
        {b_neg_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    )
    policy = builder.add_policy({rule_1})

    instance_info = InstanceInfo(vocabulary_info)
    atom_0 = instance_info.add_atom("unary", ["A"])
    atom_1 = instance_info.add_atom("unary", ["B"])

    state_1 = State(instance_info, [], 0)
    state_2 = State(instance_info, [atom_0], 1)
    state_3 = State(instance_info, [atom_0, atom_1], 2)

    caches = DenotationsCaches()

    assert policy.evaluate(state_3, state_2, caches)
    assert not policy.evaluate(state_3, state_1, caches)
    assert not policy.evaluate(state_2, state_3, caches)
    assert not policy.evaluate(state_1, state_3, caches)

    assert policy.evaluate(state_3, state_2)
    assert not policy.evaluate(state_3, state_1)
    assert not policy.evaluate(state_2, state_3)
    assert not policy.evaluate(state_1, state_3)

    print("Write policy:")
    print(repr(policy))
    print(str(policy))
    print()
    with open("policy.txt", "w") as file:
        file.write(PolicyWriter().write(policy))

    print("Read policy:")
    with open("policy.txt", "r") as file:
        policy_in = PolicyReader().read("\n".join(file.readlines()), builder, factory)
    print(repr(policy_in))
    print(str(policy_in))
    print()



if __name__ == "__main__":
    main()
