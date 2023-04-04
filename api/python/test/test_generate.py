
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
    generator = FeatureGenerator()

    a0, a1, a2, a3, a4, a5, a6, a7 = instance.get_atoms()
    s0 = State(instance, [a0, a3])  # a on b
    s1 = State(instance, [a1, a2])  # b on a
    s2 = State(instance, [a2, a3])  # both on table
    states = [s0, s1, s2]
    feature_reprs = generator.generate(factory, states, 2, 2, 5, 5, 5, 180, 100000)

    boolean_reprs = [repr for repr in feature_reprs if repr.startswith("b_")]
    numerical_reprs = [repr for repr in feature_reprs if repr.startswith("n_")]

    #assert boolean_reprs == ['b_empty(c_top)', 'b_empty(c_bot)', 'b_empty(c_primitive(on,0))', 'b_inclusion(c_primitive(on,0),c_primitive(on_g,0))', 'b_inclusion(c_primitive(on,0),c_primitive(on_g,1))', 'b_inclusion(c_primitive(on_g,0),c_primitive(on,0))', 'b_inclusion(c_primitive(on_g,0),c_primitive(on,1))']
    #assert numerical_reprs == ['n_count(c_top)', 'n_count(c_primitive(on,0))', 'n_count(c_primitive(ontable,0))', 'n_count(r_top)', 'n_count(r_not(r_primitive(on,0,1)))', 'n_count(r_not(r_primitive(on_g,0,1)))', 'n_count(r_transitive_reflexive_closure(r_primitive(on,0,1)))', 'n_concept_distance(c_primitive(on_g,0),r_top,c_bot)', 'n_concept_distance(c_primitive(on_g,0),r_top,c_primitive(on,0))', 'n_concept_distance(c_primitive(on_g,0),r_top,c_primitive(on,1))', 'n_concept_distance(c_primitive(on_g,0),r_primitive(on,0,1),c_primitive(on,0))', 'n_concept_distance(c_primitive(on_g,0),r_primitive(on,0,1),c_primitive(on_g,1))', 'n_concept_distance(c_primitive(on_g,1),r_primitive(on,0,1),c_primitive(on,0))', 'n_concept_distance(c_primitive(on_g,1),r_primitive(on,0,1),c_primitive(on_g,0))', 'n_concept_distance(c_primitive(on_g,1),r_primitive(on_g,0,1),c_primitive(ontable,0))', 'n_role_distance(r_top,r_top,r_primitive(on,0,1))', 'n_sum_concept_distance(c_primitive(on,0),r_top,c_primitive(on,1))', 'n_sum_concept_distance(c_primitive(ontable,0),r_primitive(on,0,1),c_primitive(on_g,0))', 'n_concept_distance(c_primitive(on_g,0),r_not(r_primitive(on,0,1)),c_primitive(on_g,1))', 'n_concept_distance(c_primitive(on_g,1),r_not(r_primitive(on,0,1)),c_primitive(on_g,0))', 'n_sum_concept_distance(c_primitive(ontable,0),r_primitive(on,0,1),c_not(c_primitive(on,1)))', 'n_sum_concept_distance(c_primitive(ontable,0),r_primitive(on_g,0,1),c_not(c_primitive(on,1)))', 'n_sum_concept_distance(c_not(c_primitive(on,1)),r_primitive(on_g,0,1),c_primitive(ontable,0))', 'n_sum_role_distance(r_top,r_top,r_identity(c_primitive(ontable,0)))']

    b0, b1, b2 = [factory.parse_boolean(b) for b in boolean_reprs[:3]]
    n0, n1, n2, n3 = [factory.parse_numerical(n) for n in numerical_reprs[:4]]

    #assert b0.compute_repr() == "b_empty(c_top)"
    #assert [b0.evaluate(s) for s in states] == [False, False, False]
    #assert b1.compute_repr() == "b_empty(c_bot)"
    #assert [b1.evaluate(s) for s in states] == [True, True, True]
    #assert b2.compute_repr() == "b_empty(c_primitive(on,0))"
    #assert [b2.evaluate(s) for s in states] == [False, False, True]
#
    #assert n0.compute_repr() == "n_count(c_top)"
    #assert [n0.evaluate(s) for s in states] == [2, 2, 2]
    #assert n1.compute_repr() == "n_count(c_primitive(on,0))"
    #assert [n1.evaluate(s) for s in states] == [1, 1, 0]
    #assert n2.compute_repr() == "n_count(c_primitive(ontable,0))"
    #assert [n2.evaluate(s) for s in states] == [1, 1, 2]
    #assert n3.compute_repr() == "n_count(r_top)"
    #assert [n3.evaluate(s) for s in states] == [4, 4, 4]
