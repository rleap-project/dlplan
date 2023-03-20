#! /usr/bin/env python
import platform
import re

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
print(NODE)
REMOTE = re.match(r"tetralith\d+.nsc.liu.se|n\d+", NODE)

BENCHMARKS_DIR = "../benchmarks"
print(REMOTE)
if REMOTE:
    ENV = TetralithEnvironment(
        partition="tetralith",
        email="",
        memory_per_cpu="3G",
        cpus_per_task=16,
        setup=TetralithEnvironment.DEFAULT_SETUP,
        extra_options="#SBATCH --account=snic2022-5-341")
    SUITE = ["barman", "blocksworld_3", "blocksworld_4", "childsnack", "delivery", "gripper", "miconic", "reward", "spanner", "visitall"]
    TIME_LIMIT = 3 * 3600
else:
    ENV = LocalEnvironment(processes=4)
    SUITE = ["blocksworld_3:p-3-0.pddl", "childsnack:p-2-1.0-0.0-1-0.pddl", "delivery:instance_2_1_0.pddl", "gripper:p-1-0.pddl", "miconic:p-2-2-0.pddl", "reward:instance_2x2_0.pddl", "visitall:p-1-0.5-2-0.pddl"]
    TIME_LIMIT = 180
ATTRIBUTES = [
    Attribute("num_iterations", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_states", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_dynamic_atoms", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_static_atoms", absolute=True, min_wins=True, scale="linear"),
    Attribute("generate_time_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("generate_memory_complexity_5", absolute=True, min_wins=True, scale="linear"),
    Attribute("generate_time_complexity_10", absolute=True, min_wins=True, scale="linear"),
    Attribute("generate_memory_complexity_10", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_booleans", absolute=True, min_wins=True, scale="linear"),
    Attribute("num_numericals", absolute=True, min_wins=True, scale="linear"),
    Attribute("evaluate_time_single", absolute=True, min_wins=True, scale="linear"),
    Attribute("evaluate_time_single_cache", absolute=True, min_wins=True, scale="linear"),
    Attribute("evaluate_time_multi_cache", absolute=True, min_wins=True, scale="linear"),

]

MEMORY_LIMIT = (16 * 3000) * 0.98

GENERATOR_TIME_LIMIT = 2 * 3600
GENERATOR_FEATURE_LIMIT = 1000000
NUM_FEATURE_VALUATION_ITERATIONS = 1000

# Create a new experiment.
exp = Experiment(environment=ENV)
# Add custom parser for FF.
exp.add_parser("experiment_parser.py")

for task in suites.build_suite(BENCHMARKS_DIR, SUITE):
    for num_threads in [1]:
        for complexity in [10]:
            run = exp.add_run()
            # Create symbolic links and aliases. This is optional. We
            # could also use absolute paths in add_command().
            run.add_resource("domain", task.domain_file, symlink=True)
            run.add_resource("problem", task.problem_file, symlink=True)
            run.add_resource("experiment_generator", "experiment_generator", symlink=True)
            # 'ff' binary has to be on the PATH.
            # We could also use exp.add_resource().
            run.add_command(
                f"complexity-{complexity}-{num_threads}",
                ["./experiment_generator", "{domain}", "{problem}", complexity, complexity, complexity, complexity, complexity, GENERATOR_TIME_LIMIT, GENERATOR_FEATURE_LIMIT, num_threads, NUM_FEATURE_VALUATION_ITERATIONS],
                time_limit=num_threads * TIME_LIMIT,
                memory_limit=MEMORY_LIMIT,
            )
            # AbsoluteReport needs the following properties:
            # 'domain', 'problem', 'algorithm', 'coverage'.
            run.set_property("domain", task.domain)
            run.set_property("problem", task.problem)
            run.set_property("algorithm", f"complexity-{complexity}-{num_threads}")
            # BaseReport needs the following properties:
            # 'time_limit', 'memory_limit'.
            run.set_property("time_limit", TIME_LIMIT)
            run.set_property("memory_limit", MEMORY_LIMIT)
            # Every run has to have a unique id in the form of a list.
            # The algorithm name is only really needed when there are
            # multiple algorithms.
            run.set_property("id", [f"complexity-{complexity}-{num_threads}", task.domain, task.problem])

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