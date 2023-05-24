from typing import Overload, List, Tuple

from ..core import SyntacticElementFactory, State


class FeatureGenerator:
    pass


def generate_features(self, 
    factory: SyntacticElementFactory,
    states: List[State],
    concept_complexity_limit: int = 9,
    role_complexity_limit: int = 9,
    boolean_complexity_limit: int = 9,
    count_numerical_complexity_limit: int = 9,
    distance_numerical_complexity_limit: int = 9,
    time_limit: int = 3600,
    feature_limit: int = 100000,
    generate_empty_boolean: bool = True,
    generate_inclusion_boolean: bool = True,
    generate_nullary_boolean: bool = True,
    generate_all_concept: bool = True,
    generate_and_concept: bool = True,
    generate_bot_concept: bool = True,
    generate_diff_concept: bool = False,
    generate_equal_concept: bool = True,
    generate_not_concept: bool = True,
    generate_one_of_concept: bool = True,
    generate_or_concept: bool = False,
    generate_primitive_concept: bool = True,
    generate_projection_concept: bool = False,
    generate_some_concept: bool = True,
    generate_subset_concept: bool = False,
    generate_top_concept: bool = True,
    generate_concept_distance_numerical: bool = True,
    generate_count_numerical: bool = True,
    generate_and_role: bool = True,
    generate_compose_role: bool = False,
    generate_diff_role: bool = False,
    generate_identity_role: bool = True,
    generate_inverse_role: bool = True,
    generate_not_role: bool = False,
    generate_or_role: bool = False,
    generate_primitive_role: bool = True,
    generate_restrict_role: bool = True,
    generate_top_role: bool = False,
    generate_transitive_closure_role: bool = True,
    generate_transitive_reflexive_closure_role: bool = False): ...
