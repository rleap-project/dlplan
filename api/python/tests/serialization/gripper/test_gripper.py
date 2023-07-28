from dlplan.serialization import Data, serialize, deserialize
from dlplan.state_space import generate_state_space

from pathlib import Path

HERE = Path(__file__).resolve().parent


def test_serialization_gripper():
    result_1 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-1-0.pddl"), None, 0, 10)
    result_2 = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-2-0.pddl"), result_1.state_space.get_instance_info().get_vocabulary_info(), 1, 10)
    out_data = Data()
    out_data.state_spaces = [result_1.state_space, result_2.state_space]

    text = serialize(out_data)
    in_data = deserialize(text)
    assert len(in_data.state_spaces) == 2
    assert id(in_data.state_spaces[0].get_instance_info()) != id(in_data.state_spaces[1].get_instance_info())
    assert id(in_data.state_spaces[0].get_instance_info().get_vocabulary_info()) == id(in_data.state_spaces[1].get_instance_info().get_vocabulary_info())
