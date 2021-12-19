import dlplan
import tarski
from tarski.io import PDDLReader


class DomainData:
    """ Store data related to a domain. """
    def __init__(self, domain_file):
        self.domain_file = domain_file
        # PDDL information
        problem = parse_domain_file(domain_file)
        self.domain_name = problem.domain_name
        # Exclude functional predicates =, !=, <, <=, >, >= and predicates with arity 0.
        self.tarski_predicates = [predicate for predicate in problem.language.predicates if not isinstance(predicate.symbol, tarski.syntax.builtins.BuiltinPredicateSymbol)]
        self.tarski_constants =  problem.language.constants()
        # Exclude intervals from types.
        self.tarski_sorts = [sort for sort in problem.language.sorts if not isinstance(sort, tarski.syntax.sorts.Interval)]
        # Store other domain related data here
        self.vocabulary_info = construct_vocabulary_info(self)
        self.syntactic_element_factory = construct_syntactic_element_factory(self)
        self.feature_generator = construct_feature_generator()


def parse_domain_file(domain_file):
    """ Parses the PDDL domain file using Tarski. """
    reader = PDDLReader()
    reader.parse_domain(domain_file)
    return reader.problem


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


def construct_syntactic_element_factory(domain_data):
    """ Constructs an empty factory for constructing elements. """
    return dlplan.SyntacticElementFactory(domain_data.vocabulary_info)


def construct_feature_generator():
    feature_generator = dlplan.FeatureGenerator()
    feature_generator.set_generate_inclusion_boolean(False)
    feature_generator.set_generate_diff_concept(False)
    feature_generator.set_generate_or_concept(False)
    feature_generator.set_generate_subset_concept(False)
    feature_generator.set_generate_role_distance_numerical(False)
    feature_generator.set_generate_sum_concept_distance_numerical(False)
    feature_generator.set_generate_sum_role_distance_numerical(False)
    feature_generator.set_generate_and_role(False)
    feature_generator.set_generate_compose_role(False)
    feature_generator.set_generate_diff_role(False)
    feature_generator.set_generate_identity_role(False)
    feature_generator.set_generate_not_role(False)
    feature_generator.set_generate_or_role(False)
    feature_generator.set_generate_top_role(False)
    feature_generator.set_generate_transitive_reflexive_closure_role(False)
    return feature_generator
