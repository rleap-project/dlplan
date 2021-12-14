#! /usr/bin/env python

"""
Example experiment for the FF planner
(http://fai.cs.uni-saarland.de/hoffmann/ff.html).
"""

import os
import platform

from downward import suites
from downward.reports.absolute import AbsoluteReport
from lab.environments import TetralithEnvironment, BaselSlurmEnvironment, LocalEnvironment
from lab.experiment import Experiment
from lab.reports import Attribute, geometric_mean


# Create custom report class with suitable info and error attributes.
class BaseReport(AbsoluteReport):
    INFO_ATTRIBUTES = ["time_limit", "memory_limit"]
    ERROR_ATTRIBUTES = [
        "domain",
        "problem",
        "algorithm",
        "unexplained_errors",
        "error",
        "node",
    ]


NODE = platform.node()
REMOTE = NODE.endswith("tetralith.nsc.liu.se") or NODE.endswith(".scicore.unibas.ch") or NODE.endswith(".cluster.bc2.ch")
BENCHMARKS_DIR = "../benchmarks"
if REMOTE:
    ENV = TetralithEnvironment(email="")
    SUITE = ["barman", "blocksworld_3", "blocksworld_4", "childsnack", "delivery", "gripper", "miconic", "reward", "spanner", "visitall"]
else:
    ENV = LocalEnvironment(processes=2)
    SUITE = ["visitall:p-1-0.5-2-0.pddl", "barman:p-1-2-1-0.pddl"]
ATTRIBUTES = [
    Attribute("time_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("memory_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_generated_features_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_novel_features_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_states", absolute=True, min_wins=False, scale="linear"),
    Attribute("num_dynamic_atoms", absolute=True, min_wins=False, scale="linear"),
    Attribute("num_static_atoms", absolute=True, min_wins=False, scale="linear"),
]
TIME_LIMIT = 24 * 1800
MEMORY_LIMIT = 3000

GENERATOR_COMPLEXITY_LIMIT = 5  # when changing this, we must adapt the parser as well
GENERATOR_TIME_LIMIT = 24 * 1800
GENERATOR_FEATURE_LIMIT = 1000000


# Create a new experiment.
exp = Experiment(environment=ENV)
# Add custom parser for FF.
exp.add_parser("experiment_parser.py")

for task in suites.build_suite(BENCHMARKS_DIR, SUITE):
    run = exp.add_run()
    # Create symbolic links and aliases. This is optional. We
    # could also use absolute paths in add_command().
    run.add_resource("domain", task.domain_file, symlink=True)
    run.add_resource("problem", task.problem_file, symlink=True)
    run.add_resource("main", "main.py", symlink=True)
    # 'ff' binary has to be on the PATH.
    # We could also use exp.add_resource().
    run.add_command(
        "baseline",
        ["python3", "main.py", "--domain", "{domain}", "--instance", "{problem}", "--c", GENERATOR_COMPLEXITY_LIMIT, "--t", GENERATOR_TIME_LIMIT, "--f", GENERATOR_FEATURE_LIMIT],
        time_limit=TIME_LIMIT,
        memory_limit=MEMORY_LIMIT,
    )
    # AbsoluteReport needs the following properties:
    # 'domain', 'problem', 'algorithm', 'coverage'.
    run.set_property("domain", task.domain)
    run.set_property("problem", task.problem)
    run.set_property("algorithm", "baseline")
    # BaseReport needs the following properties:
    # 'time_limit', 'memory_limit'.
    run.set_property("time_limit", TIME_LIMIT)
    run.set_property("memory_limit", MEMORY_LIMIT)
    # Every run has to have a unique id in the form of a list.
    # The algorithm name is only really needed when there are
    # multiple algorithms.
    run.set_property("id", ["baseline", task.domain, task.problem])

# Add step that writes experiment files to disk.
exp.add_step("build", exp.build)

# Add step that executes all runs.
exp.add_step("start", exp.start_runs)

# Add step that collects properties from run directories and
# writes them to *-eval/properties.
exp.add_fetcher(name="fetch")

# Make a report.
exp.add_report(BaseReport(attributes=ATTRIBUTES), outfile="report.html")

# Parse the commandline and run the specified steps.
exp.run_steps()