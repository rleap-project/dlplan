
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State, FeatureGenerator


def generate_bw_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    return vocabulary


def generate_bw_instance(vocabulary):
    instance = InstanceInfo(vocabulary)
    instance.add_atom("on", ["a", "b"])
    instance.add_atom("on", ["b", "a"])
    instance.add_atom("ontable", ["a"])
    instance.add_atom("ontable", ["b"])
    instance.add_atom("holding", ["a"])
    instance.add_atom("holding", ["b"])
    instance.add_atom("clear", ["a"])
    instance.add_atom("clear", ["b"])
    instance.add_static_atom("on_g", ["a", "b"])
    return instance


def test_generate_exhaustively():
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)
    generator = FeatureGenerator(factory, 2, 180)

    a0, a1, a2, a3, a4, a5, a6, a7, a8 = instance.get_atoms()
    s0 = State(instance, [a0, a3])  # a on b
    s1 = State(instance, [a1, a2])  # b on a
    s2 = State(instance, [a2, a3])  # both on table
    states = [s0, s1, s2]
    collection = generator.generate(states)
    assert len(collection.get_boolean_features()) == 3
    assert len(collection.get_numerical_features()) == 6

    b0, b1, b2 = collection.get_boolean_features()
    assert b0.get_repr() == "b_empty(on(0))"
    assert b0.get_state_evaluations() == [False, False, True]
    assert b1.get_repr() == "b_empty(on_g(0))"
    assert b1.get_state_evaluations() == [False, False, False]
    assert b2.get_repr() == "b_empty(holding(0))"
    assert b2.get_state_evaluations() == [True, True, True]

    n0, n1, n2, n3, n4, n5 = collection.get_numerical_features()
    assert n0.get_repr() == "n_count(on(0))"
    assert n0.get_state_evaluations() == [1, 1, 0]
    assert n1.get_repr() == "n_count(on_g(0))"
    assert n1.get_state_evaluations() == [1, 1, 1]
    assert n2.get_repr() == "n_count(ontable(0))"
    assert n2.get_state_evaluations() == [1, 1, 2]
    assert n3.get_repr() == "n_count(holding(0))"
    assert n3.get_state_evaluations() == [0, 0, 0]
    assert n4.get_repr() == "n_count(c_top)"
    assert n4.get_state_evaluations() == [2, 2, 2]
    assert n5.get_repr() == "n_count(r_top)"
    assert n5.get_state_evaluations() == [4, 4, 4]
