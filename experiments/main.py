#!/usr/bin/python3

""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse

from domain_data import DomainData
from instance_data import InstanceData
from transition_system_generator import TransitionSystemGenerator



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Example on how to initialize VocabularyInfo, InstanceInfo, SyntacticElementFactory from PDDL files.')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    # parser.add_argument("--path_to_sse", type=str, required=True)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data)
