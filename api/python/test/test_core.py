
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State


def generate_bw_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    return vocabulary


def test_simple():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    assert vocabulary.exists_predicate_name("on")


def test_instances():
    vocabulary = generate_bw_vocabulary()

    factory = SyntacticElementFactory(vocabulary)
    num = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))")
    assert num.compute_complexity() == 4

    instance = InstanceInfo(vocabulary)

    a0 = instance.add_atom("on", ["a", "b"])
    a1 = instance.add_atom("on", ["b", "a"])
    a2 = instance.add_atom("on_g", ["a", "b"])

    state = State(instance, [a0, a1, a2])
    num.evaluate(state)
    assert str(state) == "{on(a,b), on(b,a), on_g(a,b)}"
    assert num.evaluate(state) == 1
