import general_policy as gp


def main():
    # Construct the empty policy
    policy = gp.GeneralPolicy()
    # Add Boolean features with indices 0,1,...
    b = policy.add_boolean_feature()
    # Add numerical features with indices 0,1,...
    n = policy.add_numerical_feature()
    # Add conditions and effects to rule.
    b_pos_condition = policy.add_b_pos_condition(b)
    b_neg_effect = policy.add_b_neg_effect(b)
    n_gt_condition = policy.add_n_gt_condition(n)
    n_dec_effect = policy.add_n_dec_effect(n)
    # Add rule
    r = policy.add_rule({b_pos_condition, n_gt_condition}, {b_neg_effect, n_dec_effect})

    # Evaluate the policy.
    b.set_source_evaluation(True)
    b.set_target_evaluation(False)
    n.set_source_evaluation(5)
    n.set_target_evaluation(4)
    r_idxs = policy.compute_rules_with_satisfied_condition()
    assert r_idxs == [0]
    satisfied = policy.exists_rule_with_satisfied_effect(r_idxs)
    assert satisfied

    print("Write policy:")
    print(str(policy))
    print()
    data = gp.GeneralPolicyWriter().write(policy)

    policy_in = gp.GeneralPolicyReader().read(data)
    print("Read policy:")
    print(str(policy_in))
    print()


if __name__ == "__main__":
    main()
