from transition_system_reader import TransitionSystemReader
from utils import execute


class TransitionSystemGenerator:
    def generate(self, instance_data, path_to_sse):
        execute([path_to_sse, "--workspace", "/tmp/workspace", "--domain", instance_data.domain_data.domain_file, "--instance", instance_data.instance_file, "--options", f"max_nodes_per_class=-1,ignore_non_fringe_dead_states=false,seed=1"], stdout="stace_space.log")
        return TransitionSystemReader().read_from_file("stace_space.log")
