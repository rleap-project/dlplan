from typing import OrderedDict
import dlplan
import subprocess
import re

from typing import Dict, MutableSet
from collections import OrderedDict, defaultdict
from pathlib import Path
from utils import execute, read_file


DIR = Path(__file__).resolve().parent


class InstanceData:
    """ Store data related to a single instance. """
    def __init__(self, instance_file, domain_data, time_limit):
        self.instance_file = instance_file
        self.domain_data = domain_data

        try:
            execute([DIR / "scorpion/fast-downward.py", domain_data.domain_file, instance_file, "--translate-options", "--dump-static-atoms", "--dump-predicates", "--dump-goal-atoms", "--search-options", "--search", "dump_reachable_search_space()"], stdout="state_space.txt", timeout=time_limit)
        except subprocess.TimeoutExpired:
            pass

        self.instance_info = dlplan.InstanceInfo(domain_data.vocabulary_info)
        parse_static_atoms(self.instance_info, "static-atoms.txt")
        parse_goal_atoms(self.instance_info, "goal-atoms.txt")
        self.dlplan_states, self.goals, self.transitions = parse_state_space(self.instance_info, "state_space.txt")


def normalize_atom_name(name: str):
    tmp = name.replace('()', '').replace(')', '').replace('(', ',')
    if "=" in tmp:  # We have a functional atom
        tmp = tmp.replace("=", ',')
    return tmp.split(',')


def parse_static_atoms(instance_info: dlplan.InstanceInfo, filename: str):
    for line in read_file(filename):
        normalized_atom = normalize_atom_name(line)
        instance_info.add_static_atom(normalized_atom[0], normalized_atom[1:])


def parse_goal_atoms(instance_info: dlplan.InstanceInfo, filename: str):
    for line in read_file(filename):
        normalized_atom = normalize_atom_name(line)
        instance_info.add_static_atom(normalized_atom[0] + "_g", normalized_atom[1:])


def parse_state_space(instance_info: dlplan.InstanceInfo, filename: str):
    atom_idx_to_dlplan_atom = dict()
    dlplan_states = OrderedDict()
    goals = set()
    forward_transitions = defaultdict(set)
    for line in read_file(filename):
        if line.startswith("F "):
            parse_fact_line(instance_info, line, atom_idx_to_dlplan_atom)
        elif line.startswith("G "):
            parse_state_line(instance_info, line, atom_idx_to_dlplan_atom, dlplan_states, goals)
        elif line.startswith("N "):
            parse_state_line(instance_info, line, atom_idx_to_dlplan_atom, dlplan_states, goals)
        elif line.startswith("T "):
            parse_transition_line(line, forward_transitions)
    dlplan_states = list(dlplan_states.values())
    return dlplan_states, goals, forward_transitions


def parse_fact_line(instance_info: dlplan.InstanceInfo, line: str, atom_idx_to_dlplan_atom: Dict[int, dlplan.Atom]):
    """
    E.g.
    at-robby(roomb)
    at(ball1, rooma)
    """
    result = re.findall(r"F (\d+) (.*)", line)
    assert len(result) == 1 and len(result[0]) == 2
    atom_idx = int(result[0][0])
    atom_name = result[0][1]
    normalized_atom = normalize_atom_name(atom_name)
    if normalized_atom[0].startswith("dummy"):
        return
    dlplan_atom = instance_info.add_atom(normalized_atom[0], normalized_atom[1:])
    atom_idx_to_dlplan_atom[atom_idx] = dlplan_atom


def parse_state_line(instance_info: dlplan.InstanceInfo, line: str, atom_idx_to_dlplan_atom: Dict[int, dlplan.Atom], states: Dict[int, dlplan.State], goals: MutableSet[int]):
    """
    E.g.
    N 127 0 2 8 10 12 17 18
    G 129 0 2 8 10 11 17 19
    """
    result = re.findall(r"[NG] (.*)", line)
    assert len(result) == 1
    indices = [int(x) for x in result[0].split(" ")]
    state_idx = indices[0]
    atom_idxs = indices[1:]
    dlplan_atoms = [atom_idx_to_dlplan_atom[atom_idx] for atom_idx in atom_idxs if atom_idx in atom_idx_to_dlplan_atom]
    dlplan_state = dlplan.State(instance_info, dlplan_atoms)
    states[state_idx] = dlplan_state
    if line.startswith("G "):
        goals.add(state_idx)


def parse_transition_line(line: str, forward_transitions: Dict[int, MutableSet[int]]):
    """
    E.g.
    T 0 5
    T 1 4
    """
    result = re.findall(r"T (.*) (.*)", line)
    assert len(result) == 1
    source_idx = int(result[0][0])
    target_idx = int(result[0][1])
    forward_transitions[source_idx].add(target_idx)
