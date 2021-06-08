
from dlplan import VocabularyInfo


def test_simple():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    assert vocabulary.exists_predicate_name("on")

