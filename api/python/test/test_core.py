
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory


def test_simple():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    assert vocabulary.exists_predicate_name("on")


def test_instances():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)

    factory = SyntacticElementFactory(vocabulary)
    num = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))")
    assert num.compute_complexity() == 4
