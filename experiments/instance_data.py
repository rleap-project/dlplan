from os import stat
import tarski
from tarski.io import PDDLReader
from tarski.grounding import LPGroundingStrategy
from tarski.grounding.lp_grounding import ground_problem_schemas_into_plain_operators
from tarski.search import GroundForwardSearchModel
from dlplan_utils import construct_instance_info


class InstanceData:
    """ Store data related to a single instance. """
    def __init__(self, instance_file, domain_data):
        self.instance_file = instance_file
        self.domain_data = domain_data
        # PDDL information
        self.problem = parse_instance_file(domain_data.domain_file, instance_file)
        grounder = LPGroundingStrategy(self.problem)
        # Note: we transform tarski.StateVariableLite back to Atoms here.
        self.dynamic_atoms = set(tarski.syntax.Atom(state_variable_lite.symbol, state_variable_lite.binding)
                                 for state_variable_lite in grounder.ground_state_variables().data.keys())
        self.static_atoms = [tarski.syntax.Atom(state_variable_lite.symbol, state_variable_lite.binding)
                             for state_variable_lite in grounder.ground_static_variables().data.keys()]

        problem = parse_instance_file(domain_data.domain_file, instance_file)
        self.tarski_goal_atoms = parse_conjunctive_formula(problem.goal)
        self.instance_info, self.tarski_atom_to_dlplan_atom = construct_instance_info(domain_data, self)
        self.search_model = GroundForwardSearchModel(self.problem, ground_problem_schemas_into_plain_operators(self.problem))
        self.states = state_space_exploration(self.problem, self.search_model)
        print(self.states)


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
    return [model.as_atoms() for model in generated]
