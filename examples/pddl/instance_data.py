import tarski
from tarski.io import PDDLReader


class InstanceData:
    """ Store data related to a single instance. """
    def __init__(self, instance_file, domain_data):
        self.instance_file = instance_file
        self.domain_data = domain_data
        # PDDL information
        problem = self._parse_instance_file(domain_data.domain_file, instance_file)
        self.tarski_goal_atoms = self._parse_conjunctive_formula(problem.goal)
        # Store other instance related data here, e.g., transition system, instance_info
        self.transition_system = None
        self.instance_info = None

    def _parse_instance_file(self, domain_file, instance_file):
        """ Parses the PDDL instance file using Tarski. """
        reader = PDDLReader()
        reader.parse_domain(domain_file)
        reader.parse_instance(instance_file)
        return reader.problem

    def _parse_conjunctive_formula(self, goal):
        """ Compute all tarski Atoms from a tarski ComboundFormula. """
        if isinstance(goal, tarski.syntax.formulas.CompoundFormula):
            if goal.connective == tarski.syntax.formulas.Connective.And:
                atom_names = []
                for subformula in goal.subformulas:
                    atom_names.extend(self._parse_conjunctive_formula(subformula))
                return atom_names
            else:
                raise Exception(f"Unsupported connective {goal.connective} in goal description.")
        elif isinstance(goal, tarski.syntax.formulas.Atom):
            return [goal]
