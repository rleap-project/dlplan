
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State


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


def test_vocabulary():
    vocabulary = generate_bw_vocabulary()
    #...


def test_instance():
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)

    atoms = instance.get_atoms()
    a0 = atoms[0]
    assert a0.get_name() == "on(a,b)"
    assert a0.get_index() == 0
    assert a0.get_predicate().get_name() == "on"
    a1 = atoms[1]
    assert a1.get_name() == "on(b,a)"
    assert a1.get_index() == 1
    assert a1.get_predicate().get_name() == "on"
    a2 = atoms[2]
    assert a2.get_name() == "ontable(a)"
    assert a2.get_index() == 2
    assert a2.get_predicate().get_name() == "ontable"
    #...


def test_factory():
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)

    numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))")
    atoms = instance.get_atoms()
    state = State(instance, [atoms[0], atoms[3], atoms[6]])
    numerical.evaluate(state)
    assert str(state) == "{on(a,b), ontable(b), clear(a), on_g(a,b)}"
    assert numerical.compute_complexity() == 4
    assert numerical.evaluate(state) == 1
    #...
