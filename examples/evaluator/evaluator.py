import dlplan
from matplotlib.pyplot import subplot2grid


def main():
    vocabulary_info = dlplan.VocabularyInfo()
    vocabulary_info.add_predicate("unary", 1)
    factory = dlplan.SyntacticElementFactory(vocabulary_info)
    boolean = factory.parse_boolean("b_empty(c_primitive(unary,0))")
    numerical = factory.parse_numerical("n_count(c_primitive(unary,0))")

    builder = dlplan.PolicyBuilder()
    b = builder.add_boolean_feature(boolean)
    n = builder.add_numerical_feature(numerical)

    instance_info = dlplan.InstanceInfo(vocabulary_info)
    a0 = instance_info.add_atom("unary", ["A"])
    a1 = instance_info.add_atom("unary", ["B"])

    s0 = dlplan.State(instance_info, [], 0)
    s1 = dlplan.State(instance_info, [a0], 1)
    s2 = dlplan.State(instance_info, [a0, a1], 2)

    evaluation_cache = dlplan.EvaluationCache(1, 1)
    boolean.set_index(0)
    numerical.set_index(0)

    print("boolean(s0):", evaluation_cache.retrieve_or_evaluate(boolean, s0))
    print("boolean(s0):", evaluation_cache.retrieve_or_evaluate(boolean, s0))
    print("boolean(s1):", evaluation_cache.retrieve_or_evaluate(boolean, s1))
    print("boolean(s1):", evaluation_cache.retrieve_or_evaluate(boolean, s1))
    print("boolean(s2):", evaluation_cache.retrieve_or_evaluate(boolean, s2))
    print("boolean(s2):", evaluation_cache.retrieve_or_evaluate(boolean, s2))
    print("numerical(s0):", evaluation_cache.retrieve_or_evaluate(numerical, s0))
    print("numerical(s0):", evaluation_cache.retrieve_or_evaluate(numerical, s0))
    print("numerical(s1):", evaluation_cache.retrieve_or_evaluate(numerical, s1))
    print("numerical(s1):", evaluation_cache.retrieve_or_evaluate(numerical, s1))
    print("numerical(s2):", evaluation_cache.retrieve_or_evaluate(numerical, s2))
    print("numerical(s2):", evaluation_cache.retrieve_or_evaluate(numerical, s2))


if __name__ == "__main__":
    main()
