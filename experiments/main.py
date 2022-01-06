""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse
import logging
import time

from domain_data import DomainData
from instance_data import InstanceData


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generating features for a single PDDL instance.')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    parser.add_argument("--max_num_states", type=int, required=False, default=float("inf"))
    parser.add_argument("--c", type=int, required=False, default=5)
    parser.add_argument("--t", type=int, required=False, default=1800)
    parser.add_argument("--f", type=int, required=False, default=1000000)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data, args.max_num_states)
    dlplan_states = [state.dlplan_state for state in instance_data.states]

    feature_reprs = domain_data.feature_generator.generate(domain_data.syntactic_element_factory, args.c, args.t, args.f, dlplan_states)
    start = time.time()
    for feature_repr in feature_reprs:
        if feature_repr.startswith("n_"):
            f = domain_data.syntactic_element_factory.parse_numerical(feature_repr)
        elif feature_repr.startswith("b_"):
            f = domain_data.syntactic_element_factory.parse_boolean(feature_repr)
        elif feature_repr.startswith("c_"):
            f = domain_data.syntactic_element_factory.parse_concept(feature_repr)
        elif feature_repr.startswith("r_"):
            f = domain_data.syntactic_element_factory.parse_role(feature_repr)
        for dlplan_state in dlplan_states:
            f.evaluate(dlplan_state)
    end = time.time()
    print(f"Time to construct and evaluate features: {end - start}s")


    print("Instance information:")
    print(f"Number of states: {len(dlplan_states)}")
    print(f"Number of dynamic atoms: {len(instance_data.dynamic_atoms)}")
    print(f"Number of static atoms: {len(instance_data.static_atoms)}")
    print(f"Number of objects: {instance_data.instance_info.get_num_objects()}")
