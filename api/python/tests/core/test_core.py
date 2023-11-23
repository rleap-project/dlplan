from dlplan.core import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State


def generate_bw_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    return vocabulary


def generate_bw_instance(vocabulary):
    instance = InstanceInfo(-1, vocabulary)
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
    """ Exhaustively test VocabularyInfo
    """
    vocabulary = generate_bw_vocabulary()
    predicates = vocabulary.get_predicates()
    assert predicates[0].get_name() == "on"
    assert predicates[0].get_index() == 0
    assert predicates[0].get_arity() == 2
    assert predicates[1].get_name() == "on_g"
    assert predicates[1].get_index() == 1
    assert predicates[1].get_arity() == 2
    assert predicates[2].get_name() == "ontable"
    assert predicates[2].get_index() == 2
    assert predicates[2].get_arity() == 1
    assert predicates[3].get_name() == "holding"
    assert predicates[3].get_index() == 3
    assert predicates[3].get_arity() == 1
    assert predicates[4].get_name() == "clear"
    assert predicates[4].get_index() == 4
    assert predicates[4].get_arity() == 1


def test_instance():
    """ Exhaustively test InstanceInfo
    """
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)

    objects = instance.get_objects()
    assert objects[0].get_name() == "a"
    assert objects[0].get_index() == 0
    assert objects[1].get_name() == "b"
    assert objects[1].get_index() == 1

    atoms = instance.get_atoms()
    assert atoms[0].get_name() == "on(a,b)"
    assert atoms[0].get_index() == 0
    assert [objects[obj].get_name() for obj in atoms[0].get_object_indices()] == ["a", "b"]
    assert atoms[1].get_name() == "on(b,a)"
    assert atoms[1].get_index() == 1
    assert [objects[obj].get_name() for obj in atoms[1].get_object_indices()] == ["b", "a"]
    assert atoms[2].get_name() == "ontable(a)"
    assert atoms[2].get_index() == 2
    assert [objects[obj].get_name() for obj in atoms[2].get_object_indices()] == ["a"]


def test_factory():
    """ Test the construction, evaluation, and getters of Elements.
    """
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)

    atoms = instance.get_atoms()
    state = State(-1, instance, [atoms[0], atoms[3], atoms[6]])
    assert str(state) == "(instance index=-1, state index=-1, atoms={on(a,b), ontable(b), clear(a)})"

    numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    assert numerical.compute_complexity() == 4
    assert numerical.evaluate(state) == 1

    boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    assert boolean.compute_complexity() == 4
    assert boolean.evaluate(state) == 0

    numerical = factory.parse_numerical("n_count(c_projection(r_primitive(on_g,0,1), 1))")
    assert numerical.compute_complexity() == 3
    assert numerical.evaluate(state) == 1
