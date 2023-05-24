from dlplan.core import VocabularyInfo, InstanceInfo, State, SyntacticElementFactory, DenotationsCaches
from dlplan.policy import PolicyBuilder, PolicyWriter, PolicyReader


def main():
    vocabulary_info = VocabularyInfo()
    vocabulary_info.add_predicate("unary", 1)
    factory = SyntacticElementFactory(vocabulary_info)
    b = factory.parse_boolean("b_empty(c_primitive(unary,0))")
    n = factory.parse_numerical("n_count(c_primitive(unary,0))")

    builder = PolicyBuilder()
    b_neg_condition_0 = builder.add_neg_condition(b)
    b_bot_effect_0 = builder.add_bot_effect(b)
    n_gt_condition_0 = builder.add_gt_condition(n)
    n_dec_effect_0 = builder.add_dec_effect(n)
    r = builder.add_rule(
        {b_neg_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    )
    policy = builder.add_policy({r})

    instance_info = InstanceInfo(vocabulary_info)
    a0 = instance_info.add_atom("unary", ["A"])
    a1 = instance_info.add_atom("unary", ["B"])

    s0 = State(instance_info, [], 0)
    s1 = State(instance_info, [a0], 1)
    s2 = State(instance_info, [a0, a1], 2)

    caches = DenotationsCaches()

    assert policy.evaluate(s2, s1, caches)
    assert not policy.evaluate(s2, s0, caches)
    assert not policy.evaluate(s1, s2, caches)
    assert not policy.evaluate(s0, s2, caches)

    assert policy.evaluate(s2, s1)
    assert not policy.evaluate(s2, s0)
    assert not policy.evaluate(s1, s2)
    assert not policy.evaluate(s0, s2)

    print("Write policy:")
    print(repr(policy))
    print(str(policy))
    print()
    with open("policy.txt", "w") as f:
        f.write(PolicyWriter().write(policy))

    print("Read policy:")
    with open("policy.txt", "r") as f:
        policy_in = PolicyReader().read("\n".join(f.readlines()), builder, factory)
    print(repr(policy_in))
    print(str(policy_in))
    print()



if __name__ == "__main__":
    main()
