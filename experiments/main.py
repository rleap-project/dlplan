#! /usr/bin/env python3
""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse
import time
import sys

from domain_data import DomainData
from instance_data import InstanceData


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generating features for a single PDDL instance.')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    parser.add_argument("--time_limit", type=int, required=False, default=float("inf"))
    parser.add_argument("--c", type=int, required=False, default=5)
    parser.add_argument("--t", type=int, required=False, default=1800)
    parser.add_argument("--f", type=int, required=False, default=1000000)
    parser.add_argument("--n", type=int, required=False, default=1)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data, args.time_limit)

    # print vocabulary and instance information for easy re-parsing from the cpp code.
    print("Predicates:")
    print("\n".join([str(predicate) for predicate in domain_data.vocabulary_info.get_predicates()]))
    print("\nConstants:")
    print("\n".join([str(constant) for constant in domain_data.vocabulary_info.get_constants()]))
    print("\nObjects:")
    print("\n".join([str(obj) for obj in instance_data.instance_info.get_objects()]))
    print("\nAtoms:")
    print("\n".join([str(obj) for obj in instance_data.instance_info.get_atoms()]))
    print("\nStatic atom indices:")
    print("\n".join([str(idx) for idx in instance_data.instance_info.get_static_atom_idxs()]))
    print("\nStates:")
    print("\n".join([str(state) for state in instance_data.dlplan_states]))
    print()

    print("Instance information:")
    print(f"Number of states: {len(instance_data.dlplan_states)}")

    sys.stdout.flush()

    feature_reprs = domain_data.feature_generator.generate(domain_data.syntactic_element_factory, args.c, args.t, args.f, args.n, instance_data.dlplan_states)
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
        for dlplan_state in instance_data.dlplan_states:
            f.evaluate(dlplan_state)
    end = time.time()
    print(f"Time to construct and evaluate features: {end - start}s")
