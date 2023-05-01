from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State


def generate_barman_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 2)
    vocabulary.add_predicate("handempty", 1)
    vocabulary.add_predicate("empty", 1)
    vocabulary.add_predicate("contains", 2)
    vocabulary.add_predicate("contains_g", 2)
    vocabulary.add_predicate("clean", 1)
    vocabulary.add_predicate("used", 2)
    vocabulary.add_predicate("dispenses", 2)
    vocabulary.add_predicate("shaker-empty-level", 2)
    vocabulary.add_predicate("shaker-level", 2)
    vocabulary.add_predicate("next", 2)
    vocabulary.add_predicate("unshaked", 1)
    vocabulary.add_predicate("shaked", 1)
    vocabulary.add_predicate("cocktail-part1", 2)
    vocabulary.add_predicate("cocktail-part2", 2)
    return vocabulary


def generate_barman_instance(vocabulary):
    # {shaker-level(shaker1,l1), contains(shaker1,ingredient2), unshaked(shaker1), empty(shot1),
    # used(shot1,ingredient2), holding(left,shot1), holding(right,shaker1), contains_g(shot1,cocktail1)}
    instance = InstanceInfo(vocabulary)
    instance.add_atom("shaker-level", ["shaker1", "l1"])
    instance.add_atom("contains", ["shaker1", "ingredient2"])
    instance.add_atom("unshaked", ["shaker1"])
    instance.add_atom("empty", ["shot1"])
    instance.add_atom("used", ["shot1", "ingredient2"])
    instance.add_atom("holding", ["left", "shot1"])
    instance.add_atom("holding", ["right", "shaker1"])
    # static atoms, only partially added for now
    instance.add_static_atom("cocktail-part1", ["cocktail1", "ingredient2"])
    instance.add_static_atom("cocktail-part2", ["cocktail1", "ingredient1"])
    instance.add_static_atom("contains_g", ["shot1", "cocktail1"])
    return instance


def test_core_instance():
    """ Exhaustively test InstanceInfo
    """
    vocabulary = generate_barman_vocabulary()
    instance = generate_barman_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)
    state = State(instance, [atom for atom in instance.get_atoms() if not atom.is_static()])
    concept = factory.parse_concept("c_primitive(shaker-level,0)")
    result = concept.evaluate(state).to_sorted_vector()
    assert result == [0]
    assert instance.get_objects()[0].get_name() == "shaker1"
    concept = factory.parse_concept("c_some(r_primitive(contains,1,0),c_primitive(shaker-level,0))")
    result = concept.evaluate(state).to_sorted_vector()
    assert result == [2]
    assert instance.get_objects()[2].get_name() == "ingredient2"
    role = factory.parse_role("r_primitive(cocktail-part1,0,1)")
    result = role.evaluate(state).to_sorted_vector()
    assert result == [(6,2)]
    concept = factory.parse_concept("c_some(r_primitive(cocktail-part1,0,1),c_some(r_primitive(contains,1,0),c_primitive(shaker-level,0)))")
    result = concept.evaluate(state).to_sorted_vector()
    assert result == [6]
    assert instance.get_objects()[6].get_name() == "cocktail1"
    numerical = factory.parse_numerical("n_count(c_and(c_some(r_primitive(cocktail-part1,0,1),c_some(r_primitive(contains,1,0),c_primitive(shaker-level,0))),c_projection(r_diff(r_primitive(contains_g,0,1),r_primitive(contains,0,1)),1)))")
    assert numerical.evaluate(state) == 1