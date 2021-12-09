import dlplan


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


def construct_syntactic_element_factory(domain_data):
    """ Constructs an empty factory for constructing elements. """
    return dlplan.SyntacticElementFactory(domain_data.vocabulary_info)


def construct_feature_generator():
    feature_generator = dlplan.FeatureGenerator()
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
