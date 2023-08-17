from dlplan.novelty import NoveltyBase, TupleGraph
from dlplan.serialization import Data, serialize, deserialize
from dlplan.state_space import generate_state_space

from pathlib import Path

HERE = Path(__file__).resolve().parent


def test_serialization_gripper():
    result_1 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-1-0.pddl"), None, 0)
    result_2 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-2-0.pddl"), result_1.state_space.get_instance_info().get_vocabulary_info(), 1)
    out_data = Data()
    out_data.state_spaces = {
        "0": result_1.state_space,
        "1": result_2.state_space
    }
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

    text = serialize(out_data)
    in_data = deserialize(text)
    assert len(in_data.state_spaces) == 2
    assert id(in_data.state_spaces["0"].get_instance_info()) != id(in_data.state_spaces["1"].get_instance_info())
    assert id(in_data.state_spaces["0"].get_instance_info().get_vocabulary_info()) == id(in_data.state_spaces["1"].get_instance_info().get_vocabulary_info())

    assert len(in_data.tuple_graphs) == len(result_1.state_space.get_states()) + len(result_2.state_space.get_states())
    assert in_data.tuple_graphs["0_0"] is not None
    assert in_data.tuple_graphs["1_0"] is not None