from dlplan.core import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State
from dlplan.generator import FeatureGenerator


def generate_bw_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    return vocabulary


def generate_bw_instance(vocabulary):
    instance = InstanceInfo(0, vocabulary)
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
    generator = FeatureGenerator()

    atoms = instance.get_atoms()
    state_0 = State(0, instance, [atoms[0], atoms[3]])  # a on b
    state_1 = State(1, instance, [atoms[1], atoms[2]])  # b on a
    state_2 = State(2, instance, [atoms[2], atoms[3]])  # both on table
    states = [state_0, state_1, state_2]
    generated_booleans, generated_numericals, generated_concepts, generated_roles = generator.generate(factory, states, 2, 2, 5, 5, 5, 180, 100000)

