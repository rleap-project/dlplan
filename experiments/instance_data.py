from os import stat
import dlplan
import tarski
from tarski.io import PDDLReader
from tarski.grounding import LPGroundingStrategy
from tarski.grounding.lp_grounding import ground_problem_schemas_into_plain_operators
from tarski.search import GroundForwardSearchModel


class State:
    def __init__(self, index, tarski_state, dlplan_state):
        self.index = index
        self.tarski_state = tarski_state
        self.dlplan_state = dlplan_state

    def __str__(self):
        return str(self.dlplan_state)


class InstanceData:
    """ Store data related to a single instance. """
    def __init__(self, instance_file, domain_data):
        self.instance_file = instance_file
        self.domain_data = domain_data

        self.problem = parse_instance_file(domain_data.domain_file, instance_file)
        grounder = LPGroundingStrategy(self.problem)
        # Note: we transform tarski.StateVariableLite back to Atoms here.
        self.dynamic_atoms = [tarski.syntax.Atom(state_variable_lite.symbol, state_variable_lite.binding)
                                 for state_variable_lite in grounder.ground_state_variables().data.keys()]
        self.static_atoms = [tarski.syntax.Atom(state_variable_lite.symbol, state_variable_lite.binding)
                             for state_variable_lite in grounder.ground_static_variables().data.keys()]

        self.tarski_goal_atoms = parse_conjunctive_formula(self.problem.goal)
        self.instance_info, self.tarski_atom_to_dlplan_atom = construct_instance_info(domain_data, self)
        self.search_model = GroundForwardSearchModel(self.problem, ground_problem_schemas_into_plain_operators(self.problem))
        self.states = [State(index, model.as_atoms(), self.map_tarski_atoms_to_dlplan_state(model.as_atoms())) for index, model in enumerate(state_space_exploration(self.problem, self.search_model))]

    def map_tarski_atoms_to_dlplan_state(self, tarski_atoms):
        # TODO: add error checking
        return dlplan.State(self.instance_info, [self.tarski_atom_to_dlplan_atom[tarski_atom]
                                                 for tarski_atom in tarski_atoms if tarski_atom in self.dynamic_atoms])


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


def state_space_exploration(problem, search_model):
    """ Run DFS to explore state space. """
    frontier = []
    frontier.append(problem.init)
    generated = set()
    generated.add(problem.init)
    while frontier:
        cur = frontier.pop()
        successors = search_model.successors(cur)
        for action, succ in successors:
            if succ in generated: continue
            frontier.append(succ)
            generated.add(succ)
    return generated


def construct_instance_info(domain_data, instance_data):
    """ Constructs an InstanceInfo from a problem description. """
    instance_info = dlplan.InstanceInfo(domain_data.vocabulary_info)
    # Add dynamic atoms
    tarski_atom_to_dlplan_atom = dict()
    for tarski_atom in instance_data.dynamic_atoms:
        dlplan_atom = instance_info.add_atom(tarski_atom.predicate.name, [obj.name for obj in tarski_atom.subterms])
        assert tarski_atom not in tarski_atom_to_dlplan_atom
        tarski_atom_to_dlplan_atom[tarski_atom] = dlplan_atom
    # Add other static atoms
    for static_atom in instance_data.static_atoms:
        instance_info.add_static_atom(static_atom.predicate.name, [obj.name for obj in static_atom.subterms])
    # Add static goal atoms
    for tarski_atom in instance_data.tarski_goal_atoms:
        predicate_name = tarski_atom.predicate.name
        object_names = []
        for obj in tarski_atom.subterms:
            object_names.append(obj.name)
        # add atom as goal version of the predicate
        instance_info.add_static_atom(predicate_name + "_g", object_names)
    return instance_info, tarski_atom_to_dlplan_atom