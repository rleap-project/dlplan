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


parser = Parser()
parser.add_pattern("num_iterations", r"Number of iterations: (.+)", type=int)
parser.add_pattern("num_states", r"Number of states: (.+)", type=int)
parser.add_pattern("num_dynamic_atoms", r"Number of dynamic atoms: (.+)", type=int)
parser.add_pattern("num_static_atoms", r"Number of static atoms: (.+)", type=int)
parser.add_pattern("generate_time_complexity_5", r"\[t=(.+)s, .+ KB\] Complexity 5:\n", type=float)
parser.add_pattern("generate_memory_complexity_5", r"\[t=.+s, (.+) KB\] Complexity 5:\n", type=int)
parser.add_pattern("generate_time_complexity_10", r"\[t=(.+)s, .+ KB\] Complexity 10:\n", type=float)
parser.add_pattern("generate_memory_complexity_10", r"\[t=.+s, (.+) KB\] Complexity 10:\n", type=int)
parser.add_pattern("num_booleans", r"Number of booleans: (.+)", type=int)
parser.add_pattern("num_numericals", r"Number of numericals: (.+)", type=int)
parser.add_pattern("evaluate_time_single", r"Time evaluate features for single state without cache: (.+)ms", type=float)
parser.add_pattern("evaluate_time_single_cache", r"Time evaluate features for single state with cache: (.+)ms", type=float)
parser.add_pattern("evaluate_time_multi_cache", r"Time evaluate features for all states with cache: (.+)ms", type=float)
parser.parse()
