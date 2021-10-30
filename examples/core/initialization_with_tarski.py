#!/usr/bin/python3

""" Module description: library initialization using the PDDL parsing
    functionality of Tarski (https://tarski.readthedocs.io/en/latest/)
"""

import argparse
from collections import defaultdict

import tarski
from tarski.io import PDDLReader

import dlplan


class Object:
    """ An object occuring in an instance. """
    def __init__(self, index, name):
        self.index = index
        self.name = name

class Predicate:
    """ A predicate occuring in a domain. """
    def __init__(self, index, name, arity):
        self.index = index
        self.name = name
        self.arity = arity

class Atom:
    """ An atom occuring in an instance. """
    def __init__(self, index, predicate, objects):
        self.index = index
        self.predicate = predicate
        self.objects = objects

class State:
    """ A state over ground atoms of an instance. """
    def __init__(self, index, atoms):
        self.index = index
        self.atoms = atoms

class TransitionSystem:
    """ The transition system of an instance. """
    def __init__(self, states, transitions, dynamic_atoms, static_atoms):
        self.states = states
        self.transitions = transitions
        self.dynamic_atoms = dynamic_atoms
        self.static_atoms = static_atoms


class DomainData:
    """ Store data related to a domain. """
    def __init__(self, domain_file):
        self.domain_file = domain_file
        # PDDL information
        problem = parse_domain_file(domain_file)
        self.domain_name = problem.domain_name
        # Exclude functional predicates =, !=, <, <=, >, >= and predicates with arity 0
        self.tarski_predicates = [predicate for predicate in problem.language.predicates if not isinstance(predicate, tarski.syntax.builtins.BuiltinPredicateSymbol) and predicate.arity > 0]
        self.tarski_constants =  problem.language.constants()
        self.tarski_sorts = [sort for sort in problem.language.sorts if not isinstance(sort, tarski.syntax.sorts.Interval)]
        # Store other domain related data here
        self.vocabulary_info = construct_vocabulary_info(self)
        self.syntactic_element_factory = construct_syntactic_element_factory(self)


class InstanceData:
    """ Store data related to a single instance. """
    def __init__(self, instance_file, domain_data):
        self.instance_file = instance_file
        self.domain_data = domain_data
        # Assume that we have constructed the state space and parsed it into a TransitionSystem
        self.transition_system = TransitionSystem([], defaultdict(set), [], [])
        # PDDL information
        problem = parse_instance_file(domain_data.domain_file, instance_file)
        self.tarski_goal_atoms = parse_conjunctive_formula(problem.goal)
        # Store other instance related data here, e.g., transition system, instance_info
        self.instance_info = construct_instance_info(domain_data, self)


def parse_domain_file(domain_file):
    """ Parses the PDDL domain file using Tarski. """
    reader = PDDLReader()
    reader.parse_domain(domain_file)
    return reader.problem


def parse_instance_file(domain_file, instance_file):
    """ Parses the PDDL instance file using Tarski. """
    reader = PDDLReader()
    reader.parse_domain(domain_file)
    reader.parse_instance(instance_file)
    return reader.problem


def parse_conjunctive_formula(goal):
    """ Compute all tarski Atoms from a tarski ComboundFormula. """
    if isinstance(goal, tarski.syntax.formulas.CompoundFormula):
        if goal.connective == tarski.syntax.formulas.Connective.And:
            atom_names = []
            for subformula in goal.subformulas:
                atom_names.extend(parse_conjunctive_formula(subformula))
            return atom_names
        else:
            raise Exception(f"Unsupported connective {goal.connective} in goal description.")
    elif isinstance(goal, tarski.syntax.formulas.Atom):
        return [goal]


def construct_vocabulary_info(domain_data):
    """ Constructs a VocabularyInfo from a domain description. """
    vocabulary_info = dlplan.VocabularyInfo()
    # Add predicates
    for predicate in domain_data.tarski_predicates:
        vocabulary_info.add_predicate(str(predicate.name), predicate.arity)
        # we allow respective goal versions
        vocabulary_info.add_predicate(str(predicate.name) + "_g", predicate.arity)
    # Add constants
    for constant in domain_data.tarski_constants:
        vocabulary_info.add_constant(str(constant.name))
    # Add sorts
    for sort in domain_data.tarski_sorts:
        vocabulary_info.add_predicate(str(sort.name), 1)
    return vocabulary_info


def construct_instance_info(domain_data, instance_data):
    """ Constructs an InstanceInfo from a problem description. """
    instance_info = dlplan.InstanceInfo(domain_data.vocabulary_info)
    # Add static goal atoms
    for tarski_atom in instance_data.tarski_goal_atoms:
        predicate_name = tarski_atom.predicate.name
        object_names = []
        for object in tarski_atom.subterms:
            object_names.append(object.name)
        # add atom as goal version of the predicate
        instance_info.add_static_atom(predicate_name + "_g", object_names)
    # Adddynamic atoms
    for atom in instance_data.transition_system.dynamic_atoms:
        if not atom.objects or atom.negated: continue
        instance_info.add_atom(atom.predicate.name, [object.name for object in atom.objects])
    # Add other static atoms
    for static_atom in instance_data.transition_system.static_atoms:
        if not static_atom.objects: continue
        instance_info.add_static_atom(static_atom.predicate.name, [object.name for object in static_atom.objects])
    return instance_info


def construct_syntactic_element_factory(domain_data):
    """ Constructs an empty factory for constructing elements. """
    return dlplan.SyntacticElementFactory(domain_data.vocabulary_info)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parse PDDL with tarski')
    parser.add_argument("--domain", type=str, required=True)
    parser.add_argument("--instance", type=str, required=True)
    args = parser.parse_args()

    domain_data = DomainData(args.domain)
    instance_data = InstanceData(args.instance, domain_data)
