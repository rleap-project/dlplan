
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
    s0 = State(instance, [a0, a3])
    s1 = State(instance, [a1, a2])
    s2 = State(instance, [a2, a3])
    states = [s0, s1, s2]
    collection = generator.generate(states)
    assert len(collection.get_boolean_features()) == 3
    assert len(collection.get_numerical_features()) == 6
