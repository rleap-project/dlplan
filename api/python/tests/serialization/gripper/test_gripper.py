from pathlib import Path

from dlplan.core import SyntacticElementFactory, DenotationsCaches
from dlplan.novelty import NoveltyBase, TupleGraph
from dlplan.policy import PolicyFactory
from dlplan.serialization import Data, serialize, deserialize
from dlplan.state_space import generate_state_space

HERE = Path(__file__).resolve().parent


def test_serialization_gripper():
    result_1 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-1-0.pddl"), None, 0)
    result_2 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-2-0.pddl"), result_1.state_space.get_instance_info().get_vocabulary_info(), 1)

    # StateSpace
    out_data = Data()
    out_data.state_spaces = {
        "0": result_1.state_space,
        "1": result_2.state_space
    }

    # TupleGraph
    novelty_base_1 = NoveltyBase(len(result_1.state_space.get_instance_info().get_atoms()), 1)
    novelty_base_2 = NoveltyBase(len(result_2.state_space.get_instance_info().get_atoms()), 1)
    tuple_graphs = {
        str(result_1.state_space.get_instance_info().get_index()) + "_" + str(s_idx): TupleGraph(novelty_base_1, result_1.state_space, s_idx) \
            for s_idx, state in result_1.state_space.get_states().items()
    }
    tuple_graphs.update({
        str(result_2.state_space.get_instance_info().get_index()) + "_" + str(s_idx): TupleGraph(novelty_base_2, result_2.state_space, s_idx) \
            for s_idx, state in result_2.state_space.get_states().items()
    })
    out_data.tuple_graphs = tuple_graphs

    # VocabularyInfo
    vocabulary_infos = {
        "0": result_1.state_space.get_instance_info().get_vocabulary_info(),
        "1": result_2.state_space.get_instance_info().get_vocabulary_info()
    }
    out_data.vocabulary_infos = vocabulary_infos

    # InstanceData
    instance_infos = {
        "0": result_1.state_space.get_instance_info(),
        "1": result_2.state_space.get_instance_info()
    }
    out_data.instance_infos = instance_infos

    # SyntacticElementFactory
    element_factory = SyntacticElementFactory(result_1.state_space.get_instance_info().get_vocabulary_info())
    numerical = element_factory.parse_numerical("n_count(c_primitive(free, 0))")
    boolean = element_factory.parse_boolean("b_empty(r_and(r_primitive(at, 0, 1), r_primitive(at_g, 0, 1)))");
    syntactic_element_factories = {
        "0": element_factory
    }
    out_data.syntactic_element_factories = syntactic_element_factories

    # PolicyBuilder
    policy_factory = PolicyFactory(element_factory)
    c_n_gt = policy_factory.make_gt_condition(numerical)
    e_n_dec = policy_factory.make_dec_effect(numerical)
    c_b_pos = policy_factory.make_pos_condition(boolean)
    e_b_neg = policy_factory.make_neg_effect(boolean)
    rule_1 = policy_factory.make_rule({c_n_gt}, {e_n_dec})
    rule_2 = policy_factory.make_rule({c_b_pos}, {e_b_neg})
    policy = policy_factory.make_policy({rule_1, rule_2})
    policy_factories = {
        "0": policy_factory
    }
    out_data.policy_factories = policy_factories

    # Policy
    policies = {
        "0": policy
    }
    out_data.policies = policies

    # DenotationsCaches
    denotations_caches_1 = DenotationsCaches()
    denotations_caches_2 = DenotationsCaches()
    for s_idx, state in result_1.state_space.get_states().items():
        numerical.evaluate(state, denotations_caches_1)
    for s_idx, state in result_2.state_space.get_states().items():
        numerical.evaluate(state, denotations_caches_2)
    denotations_caches = {
        "0": denotations_caches_1,
        "1": denotations_caches_2
    }
    out_data.denotations_caches = denotations_caches

    text = serialize(out_data)
    in_data = deserialize(text)
    assert len(in_data.state_spaces) == 2
    assert id(in_data.state_spaces["0"].get_instance_info()) != id(in_data.state_spaces["1"].get_instance_info())
    assert id(in_data.state_spaces["0"].get_instance_info().get_vocabulary_info()) == id(in_data.state_spaces["1"].get_instance_info().get_vocabulary_info())

    assert len(in_data.tuple_graphs) == len(result_1.state_space.get_states()) + len(result_2.state_space.get_states())
    assert in_data.tuple_graphs["0_0"] is not None
    assert in_data.tuple_graphs["1_0"] is not None

    assert len(in_data.vocabulary_infos) == 2
    assert in_data.vocabulary_infos["0"] == in_data.vocabulary_infos["1"]

    assert len(in_data.instance_infos) == 2
    assert in_data.instance_infos["0"] != in_data.instance_infos["1"]

    assert len(in_data.denotations_caches) == 2

    assert len(in_data.policies) == 1

    assert len(in_data.policy_factories) == 1