#! /usr/bin/env python

"""
[t=1.72008s, 50092 KB] Complexity 10:
"""

import re

from lab.parser import Parser

parser = Parser()
parser.add_pattern("time", r"\[t=(.+)s, .+ KB\] Complexity 10:\n", type=float)
parser.parse()