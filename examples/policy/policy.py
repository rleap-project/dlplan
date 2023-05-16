import dlplan


def main():
    vocabulary_info = dlplan.VocabularyInfo()
    vocabulary_info.add_predicate("unary", 1)
    factory = dlplan.SyntacticElementFactory(vocabulary_info)
    b = factory.parse_boolean("b_empty(c_primitive(unary,0))")
    n = factory.parse_numerical("n_count(c_primitive(unary,0))")

    builder = dlplan.PolicyBuilder()
    b_neg_condition_0 = builder.add_neg_condition(b)
    b_bot_effect_0 = builder.add_bot_effect(b)
    n_gt_condition_0 = builder.add_gt_condition(n)
    n_dec_effect_0 = builder.add_dec_effect(n)
    r = builder.add_rule(
        {b_neg_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    )
    policy = builder.add_policy({r})

    instance_info = dlplan.InstanceInfo(vocabulary_info)
    a0 = instance_info.add_atom("unary", ["A"])
    a1 = instance_info.add_atom("unary", ["B"])

    s0 = dlplan.State(instance_info, [], 0)
    s1 = dlplan.State(instance_info, [a0], 1)
    s2 = dlplan.State(instance_info, [a0, a1], 2)

    caches = dlplan.DenotationsCaches()

    assert policy.evaluate_lazy(s2, s1, caches)
    assert not policy.evaluate_lazy(s2, s0, caches)
    assert not policy.evaluate_lazy(s1, s2, caches)
    assert not policy.evaluate_lazy(s0, s2, caches)

    assert policy.evaluate_lazy(s2, s1)
    assert not policy.evaluate_lazy(s2, s0)
    assert not policy.evaluate_lazy(s1, s2)
    assert not policy.evaluate_lazy(s0, s2)

    print("Write policy:")
    print(policy.compute_repr())
    print(policy.str())
    print()
    with open("policy.txt", "w") as f:
        f.write(dlplan.PolicyWriter().write(policy))

    print("Read policy:")
    with open("policy.txt", "r") as f:
        policy_in = dlplan.PolicyReader().read("\n".join(f.readlines()), builder, factory)
    print(policy_in.compute_repr())
    print(policy_in.str())
    print()



if __name__ == "__main__":
    main()
