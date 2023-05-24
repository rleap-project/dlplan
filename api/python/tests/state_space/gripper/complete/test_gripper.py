from pathlib import Path

from dlplan.state_space import generate_state_space, GeneratorExitCode


HERE = Path(__file__).resolve().parent


def test_state_space_gripper_incomplete():
    """ """
    result = generate_state_space(str(HERE / "domain.pddl"), str(HERE / "p-1-0.pddl"), None, -1, 10)
    assert result.exit_code == GeneratorExitCode.COMPLETE
