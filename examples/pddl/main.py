#!/usr/bin/python3

""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse

from domain_data import DomainData
from instance_data import InstanceData
from transition_system_generator import TransitionSystemGenerator
from dlplan_wrapper import construct_vocabulary_info, construct_instance_info, construct_syntactic_element_factory

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parse PDDL with tarski')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    parser.add_argument("--path_to_sse", type=str, required=True)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data)

    domain_data.vocabulary_info = construct_vocabulary_info(domain_data)
    domain_data.syntactic_element_factory = construct_syntactic_element_factory(domain_data)

    instance_data.transition_system = TransitionSystemGenerator().generate(instance_data, args.path_to_sse)
    instance_data.instance_info = construct_instance_info(domain_data, instance_data)
