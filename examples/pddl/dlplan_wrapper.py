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
        if not atom.objects: continue
        instance_info.add_atom(atom.predicate.name, [object.name for object in atom.objects])
    # Add other static atoms
    for static_atom in instance_data.transition_system.static_atoms:
        if not static_atom.objects: continue
        instance_info.add_static_atom(static_atom.predicate.name, [object.name for object in static_atom.objects])
    return instance_info


def construct_syntactic_element_factory(domain_data):
    """ Constructs an empty factory for constructing elements. """
    return dlplan.SyntacticElementFactory(domain_data.vocabulary_info)
