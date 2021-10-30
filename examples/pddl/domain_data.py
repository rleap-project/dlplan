import tarski
from tarski.io import PDDLReader


class DomainData:
    """ Store data related to a domain. """
    def __init__(self, domain_file):
        self.domain_file = domain_file
        # PDDL information
        problem = self._parse_domain_file(domain_file)
        self.domain_name = problem.domain_name
        # Exclude functional predicates =, !=, <, <=, >, >= and predicates with arity 0.
        self.tarski_predicates = [predicate for predicate in problem.language.predicates if not isinstance(predicate, tarski.syntax.builtins.BuiltinPredicateSymbol) and predicate.arity > 0]
        self.tarski_constants =  problem.language.constants()
        # Exclude intervals from types.
        self.tarski_sorts = [sort for sort in problem.language.sorts if not isinstance(sort, tarski.syntax.sorts.Interval)]
        # Store other domain related data here
        self.vocabulary_info = None
        self.syntactic_element_factory = None

    def _parse_domain_file(self, domain_file):
        """ Parses the PDDL domain file using Tarski. """
        reader = PDDLReader()
        reader.parse_domain(domain_file)
        return reader.problem
