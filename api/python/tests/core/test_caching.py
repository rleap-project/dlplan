from dlplan.core import VocabularyInfo, InstanceInfo, \
    SyntacticElementFactory, State, DenotationsCaches

from ..utils.denotation import create_concept_denotation, create_role_denotation


def test_caching():
    vocabulary = VocabularyInfo()
    predicate_0 = vocabulary.add_predicate("role", 2)
    instance = InstanceInfo(0, vocabulary)
    atom_0 = instance.add_atom("role", ["A", "B"])

    state_0 = State(0, instance, [])
    state_1 = State(1, instance, [atom_0])

    factory = SyntacticElementFactory(vocabulary)
    caches = DenotationsCaches()

    concept_0 = factory.parse_concept("c_primitive(role, 0)")
    assert concept_0.evaluate(state_0) == create_concept_denotation(instance, [])
    assert concept_0.evaluate(state_0) == concept_0.evaluate(state_0, caches)
    assert concept_0.evaluate(state_0, caches) == concept_0.evaluate(state_0, caches)
    assert concept_0.evaluate(state_1) == create_concept_denotation(instance, ["A"])
    assert concept_0.evaluate(state_1) == concept_0.evaluate(state_1, caches)
    assert concept_0.evaluate(state_1, caches) == concept_0.evaluate(state_1, caches)
    assert [concept_0.evaluate(state_0,caches), concept_0.evaluate(state_1,caches)] == \
        concept_0.evaluate([state_0, state_1], caches)

    role_0 = factory.parse_role("r_primitive(role, 0, 1)")
    assert role_0.evaluate(state_0) == create_role_denotation(instance, [])
    assert role_0.evaluate(state_0) == role_0.evaluate(state_0, caches)
    assert role_0.evaluate(state_0, caches) == role_0.evaluate(state_0, caches)
    assert role_0.evaluate(state_1) == create_role_denotation(instance, [("A", "B")])
    assert role_0.evaluate(state_1) == role_0.evaluate(state_1, caches)
    assert role_0.evaluate(state_1, caches) == role_0.evaluate(state_1, caches)
    assert [role_0.evaluate(state_0,caches), role_0.evaluate(state_1,caches)] == \
        role_0.evaluate([state_0, state_1], caches)

    numerical_0 = factory.parse_numerical("n_count(c_primitive(role, 0))")
    assert numerical_0.evaluate(state_0) == 0
    assert numerical_0.evaluate(state_0) == numerical_0.evaluate(state_0, caches)
    assert numerical_0.evaluate(state_1) == 1
    assert numerical_0.evaluate(state_1) == numerical_0.evaluate(state_1, caches)
    assert [numerical_0.evaluate(state_0), numerical_0.evaluate(state_1)] == \
        numerical_0.evaluate([state_0, state_1], caches)
    assert numerical_0.evaluate([state_0, state_1], caches) == numerical_0.evaluate([state_0, state_1], caches)

    boolean_0 = factory.parse_boolean("b_empty(c_primitive(role, 0))")
    assert boolean_0.evaluate(state_0) is True
    assert boolean_0.evaluate(state_0) == boolean_0.evaluate(state_0, caches)
    assert boolean_0.evaluate(state_1) is False
    assert boolean_0.evaluate(state_1) == boolean_0.evaluate(state_1, caches)
    assert [boolean_0.evaluate(state_0), boolean_0.evaluate(state_1)] == \
        boolean_0.evaluate([state_0, state_1], caches)
    assert boolean_0.evaluate([state_0, state_1], caches) == boolean_0.evaluate([state_0, state_1], caches)
