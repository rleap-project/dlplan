#! /usr/bin/env python

"""
[t=0.152091s, 48508 KB] Complexity 5:
Total generated features: 399
Total novel features: 63
Total concept elements: 28
Total role elements: 5
Total numerical elements: 22
Total boolean elements: 8
"""

import re

from lab.parser import Parser

def get_num_generated_features_complexity_5(content, props):
    # All patterns are parsed before functions are called.
    if props.get("generate_time_complexity_5") is not None:
        props["num_generated_features_complexity_5"] = int(re.findall(r"Complexity 5:\nTotal generated features: (.+)", content, re.M)[0])

def get_num_novel_features_complexity_5(content, props):
    # All patterns are parsed before functions are called.
    if props.get("generate_time_complexity_5") is not None:
        props["num_novel_features_complexity_5"] = int(re.findall(r"Complexity 5:\nTotal generated features: .+\nTotal novel features: (.+)", content, re.M)[0])

def get_num_generated_features_complexity_10(content, props):
    # All patterns are parsed before functions are called.
    if props.get("generate_time_complexity_10") is not None:
        props["num_generated_features_complexity_10"] = int(re.findall(r"Complexity 10:\nTotal generated features: (.+)", content, re.M)[0])

def get_num_novel_features_complexity_10(content, props):
    # All patterns are parsed before functions are called.
    if props.get("generate_time_complexity_10") is not None:
        props["num_novel_features_complexity_10"] = int(re.findall(r"Complexity 10:\nTotal generated features: .+\nTotal novel features: (.+)", content, re.M)[0])


parser = Parser()
parser.add_pattern("generate_time_complexity_5", r"\[t=(.+)s, .+ KB\] Complexity 5:\n", type=float)
parser.add_pattern("generate_memory_complexity_5", r"\[t=.+s, (.+) KB\] Complexity 5:\n", type=int)
parser.add_pattern("generate_time_complexity_10", r"\[t=(.+)s, .+ KB\] Complexity 10:\n", type=float)
parser.add_pattern("generate_memory_complexity_10", r"\[t=.+s, (.+) KB\] Complexity 10:\n", type=int)
parser.add_pattern("num_states", r"Number of states: (.+)", type=int)
parser.add_pattern("num_dynamic_atoms", r"Number of dynamic atoms: (.+)", type=int)
parser.add_pattern("num_static_atoms", r"Number of static atoms: (.+)", type=int)
parser.add_pattern("evaluate_time", r"Time to construct and evaluate features: (.+)s", type=float)
parser.add_function(get_num_generated_features_complexity_5)
parser.add_function(get_num_novel_features_complexity_5)
parser.add_function(get_num_generated_features_complexity_10)
parser.add_function(get_num_novel_features_complexity_10)
parser.parse()
