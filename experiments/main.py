""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse
import logging

from domain_data import DomainData
from instance_data import InstanceData
from transition_system_generator import TransitionSystemGenerator



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generating features for a single PDDL instance.')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    parser.add_argument("--c", type=int, required=False, default=5)
    parser.add_argument("--t", type=int, required=False, default=1800)
    parser.add_argument("--f", type=int, required=False, default=1000000)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data)
    dlplan_states = [state.dlplan_state for state in instance_data.states]

    domain_data.feature_generator.generate(domain_data.syntactic_element_factory, args.c, args.t, args.f, dlplan_states)

    print("Instance information:")
    print(f"Number of states: {len(dlplan_states)}")
    print(f"Number of dynamic atoms: {len(instance_data.dynamic_atoms)}")
    print(f"Number of static atoms: {len(instance_data.static_atoms)}")
