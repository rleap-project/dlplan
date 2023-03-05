#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/core.h"

namespace py = pybind11;

using namespace dlplan::core;


void init_core(py::module_ &m) {
    py::class_<ConceptDenotation>(m, "ConceptDenotation")
        .def("__copy__", [](const ConceptDenotation& denotation, py::object){ return ConceptDenotation(denotation); })
        .def("__deepcopy__", [](const ConceptDenotation& denotation, py::object){ return ConceptDenotation(denotation); })
        .def("to_sorted_vector", &ConceptDenotation::to_sorted_vector)
    ;

    py::class_<RoleDenotation>(m, "RoleDenotation")
        .def("__copy__", [](const RoleDenotation& denotation, py::object){ return RoleDenotation(denotation); })
        .def("__deepcopy__", [](const RoleDenotation& denotation, py::object){ return RoleDenotation(denotation); })
        .def("to_sorted_vector", &RoleDenotation::to_sorted_vector)
    ;

    py::class_<DenotationsCaches>(m, "DenotationsCaches")
        .def(py::init<>())
    ;

    py::class_<Constant>(m, "Constant")
        .def("__copy__", [](const Constant& constant, py::object){ return Constant(constant); })
        .def("__deepcopy__", [](const Constant& constant, py::object){ return Constant(constant); })
        .def("__eq__", &Constant::operator==)
        .def("__neq__", &Constant::operator!=)
        .def("__repr__", &Constant::get_name_ref, py::return_value_policy::reference)
        .def("__str__", &Constant::get_name_ref, py::return_value_policy::reference)
        .def("get_index", &Constant::get_index)
        .def("get_name", &Constant::get_name_ref, py::return_value_policy::reference)
    ;

    py::class_<Object>(m, "Object")
        .def("__copy__", [](const Object& object, py::object){ return Object(object); })
        .def("__deepcopy__", [](const Object& object, py::object){ return Object(object); })
        .def("__eq__", &Object::operator==)
        .def("__neq__", &Object::operator!=)
        .def("__str__", &Object::get_name_ref, py::return_value_policy::reference)
        .def("get_index", &Object::get_index)
        .def("get_name", &Object::get_name_ref, py::return_value_policy::reference)
    ;

    py::class_<Predicate>(m, "Predicate")
        .def("__copy__", [](const Predicate& predicate, py::object){ return Predicate(predicate); })
        .def("__deepcopy__", [](const Predicate& predicate, py::object){ return Predicate(predicate); })
        .def("__eq__", &Predicate::operator==)
        .def("__neq__", &Predicate::operator!=)
        .def("__repr__", &Predicate::get_name_ref, py::return_value_policy::reference)
        .def("__str__", &Predicate::get_name_ref, py::return_value_policy::reference)
        .def("get_index", &Predicate::get_index)
        .def("get_name", &Predicate::get_name_ref, py::return_value_policy::reference)
        .def("get_arity", &Predicate::get_arity)
    ;

    py::class_<Atom>(m, "Atom")
        .def("__copy__", [](const Atom& atom, py::object){ return Atom(atom); })
        .def("__deepcopy__", [](const Atom& atom, py::object){ return Atom(atom); })
        .def("__eq__", &Atom::operator==)
        .def("__neq__", &Atom::operator!=)
        .def("__repr__", &Atom::get_name_ref, py::return_value_policy::reference)
        .def("__str__", &Atom::get_name_ref, py::return_value_policy::reference)
        .def("get_index", &Atom::get_index)
        .def("get_name", &Atom::get_name_ref, py::return_value_policy::reference)
        .def("get_predicate", &Atom::get_predicate_ref, py::return_value_policy::reference)
        .def("get_objects", &Atom::get_objects_ref, py::return_value_policy::reference)
        .def("get_object", &Atom::get_object_ref, py::return_value_policy::reference)
        .def("get_is_static", &Atom::get_is_static)
    ;

    py::class_<State>(m, "State")
        .def(py::init<std::shared_ptr<const InstanceInfo>, const std::vector<Atom>&, int>(), py::arg("instance_info"), py::arg("atoms"), py::arg("index") = -1)
        .def("__copy__", [](const State& state, py::object){ return State(state); })
        .def("__deepcopy__", [](const State& state, py::object){ return State(state); })
        .def("__eq__", &State::operator==)
        .def("__hash__", &State::compute_hash)
        .def("__neq__", &State::operator!=)
        .def("__repr__", &State::str)
        .def("__str__", &State::str)
        .def("set_index", &State::set_index)
        .def("get_index", &State::get_index)
        .def("get_atom_idxs", &State::get_atom_idxs_ref, py::return_value_policy::reference)
        .def("compute_sorted_atom_idxs", &State::compute_sorted_atom_idxs)
        .def("get_instance_info", &State::get_instance_info)
    ;

    py::class_<VocabularyInfo, std::shared_ptr<VocabularyInfo>>(m, "VocabularyInfo")
        .def(py::init<>())
        .def("__copy__", [](const VocabularyInfo& vocabulary_info, py::object){ return VocabularyInfo(vocabulary_info); })
        .def("__deepcopy__", [](const VocabularyInfo& vocabulary_info, py::object){ return VocabularyInfo(vocabulary_info); })
        .def("add_predicate", &VocabularyInfo::add_predicate, py::arg("name"), py::arg("arity"), py::arg("is_static") = false)
        .def("add_constant", &VocabularyInfo::add_constant)
        .def("exists_predicate", &VocabularyInfo::exists_predicate)
        .def("exists_predicate_name", &VocabularyInfo::exists_predicate_name)
        .def("get_predicates", &VocabularyInfo::get_predicates_ref, py::return_value_policy::reference)
        .def("get_predicate_idx", &VocabularyInfo::get_predicate_idx)
        .def("get_predicate", &VocabularyInfo::get_predicate_ref, py::return_value_policy::reference)
        .def("exists_constant", &VocabularyInfo::exists_constant)
        .def("exists_constant_name", &VocabularyInfo::exists_constant_name)
        .def("get_constants", &VocabularyInfo::get_constants_ref, py::return_value_policy::reference)
        .def("get_constant_idx", &VocabularyInfo::get_constant_idx)
        .def("get_constant", &VocabularyInfo::get_constant_ref, py::return_value_policy::reference)
    ;

    py::class_<InstanceInfo, std::shared_ptr<InstanceInfo>>(m, "InstanceInfo")
        .def(py::init<std::shared_ptr<const VocabularyInfo>, int>(), py::arg("vocabulary_info"), py::arg("index") = -1)
        .def("__copy__", [](const InstanceInfo& instance_info, py::object){ return InstanceInfo(instance_info); })
        .def("__deepcopy__", [](const InstanceInfo& instance_info, py::object){ return InstanceInfo(instance_info); })
        .def("add_object", &InstanceInfo::add_object)
        .def("add_atom", py::overload_cast<const Predicate&, const std::vector<Object>&>(&InstanceInfo::add_atom))
        .def("add_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&InstanceInfo::add_atom))
        .def("add_static_atom", py::overload_cast<const Predicate&, const std::vector<Object>&>(&InstanceInfo::add_static_atom))
        .def("add_static_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&InstanceInfo::add_static_atom))
        .def("set_index", &InstanceInfo::set_index)
        .def("get_index", &InstanceInfo::get_index)
        .def("get_atoms", &InstanceInfo::get_atoms_ref, py::return_value_policy::reference)
        .def("get_static_atoms", &InstanceInfo::get_static_atoms_ref, py::return_value_policy::reference)
        .def("get_atom", &InstanceInfo::get_atom_ref, py::return_value_policy::reference)
        .def("get_atom_idx", &InstanceInfo::get_atom_idx)
        .def("get_objects", &InstanceInfo::get_objects_ref, py::return_value_policy::reference)
        .def("get_object", &InstanceInfo::get_object_ref, py::return_value_policy::reference)
        .def("get_object_idx", &InstanceInfo::get_object_idx)
        .def("get_num_objects", &InstanceInfo::get_num_objects)
        .def("get_vocabulary_info", &InstanceInfo::get_vocabulary_info)
    ;

    py::class_<BaseElement>(m, "BaseElement")
        .def("__repr__", &BaseElement::compute_repr)
        .def("__str__", &BaseElement::compute_repr)
        .def("compute_complexity", &BaseElement::compute_complexity)
        .def("compute_repr", &BaseElement::compute_repr)
        .def("set_index", &BaseElement::set_index)
        .def("get_index", &BaseElement::get_index)
        .def("get_vocabulary_info", &BaseElement::get_vocabulary_info)
    ;

    py::class_<Concept>(m, "Concept")
        .def("__copy__", [](const Concept& concept, py::object){ return Concept(concept); })
        .def("__deepcopy__", [](const Concept& concept, py::object){ return Concept(concept); })
        .def("__repr__", &Concept::compute_repr)
        .def("__str__", &Concept::compute_repr)
        .def("evaluate", py::overload_cast<const State&>(&Concept::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Concept::evaluate, py::const_), py::return_value_policy::reference)
        .def("evaluate", py::overload_cast<const States&, DenotationsCaches&>(&Concept::evaluate, py::const_), py::return_value_policy::reference)
        .def("compute_complexity", &Concept::compute_complexity)
        .def("compute_repr", &Concept::compute_repr)
        .def("set_index", &Concept::set_index)
        .def("get_index", &Concept::get_index)
    ;

    py::class_<Role>(m, "Role")
        .def("__copy__", [](const Role& role, py::object){ return Role(role); })
        .def("__deepcopy__", [](const Role& role, py::object){ return Role(role); })
        .def("__repr__", &Role::compute_repr)
        .def("__str__", &Role::compute_repr)
        .def("evaluate", py::overload_cast<const State&>(&Role::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Role::evaluate, py::const_), py::return_value_policy::reference)
        .def("evaluate", py::overload_cast<const States&, DenotationsCaches&>(&Role::evaluate, py::const_), py::return_value_policy::reference)
        .def("compute_complexity", &Role::compute_complexity)
        .def("compute_repr", &Role::compute_repr)
        .def("set_index", &Role::set_index)
        .def("get_index", &Role::get_index)
    ;

    py::class_<Numerical, std::shared_ptr<Numerical>>(m, "Numerical")
        .def("__copy__", [](const Numerical& numerical, py::object){ return Numerical(numerical); })
        .def("__deepcopy__", [](const Numerical& numerical, py::object){ return Numerical(numerical); })
        .def("__repr__", &Numerical::compute_repr)
        .def("__str__", &Numerical::compute_repr)
        .def("evaluate", py::overload_cast<const State&>(&Numerical::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Numerical::evaluate, py::const_), py::return_value_policy::reference)
        .def("evaluate", py::overload_cast<const States&, DenotationsCaches&>(&Numerical::evaluate, py::const_), py::return_value_policy::reference)
        .def("compute_complexity", &Numerical::compute_complexity)
        .def("compute_repr", &Numerical::compute_repr)
        .def("set_index", &Numerical::set_index)
        .def("get_index", &Numerical::get_index)
    ;

    py::class_<Boolean, std::shared_ptr<Boolean>>(m, "Boolean")
        .def("__copy__", [](const Boolean& boolean, py::object){ return Boolean(boolean); })
        .def("__deepcopy__", [](const Boolean& boolean, py::object){ return Boolean(boolean); })
        .def("__repr__", &Boolean::compute_repr)
        .def("__str__", &Boolean::compute_repr)
        .def("evaluate", py::overload_cast<const State&>(&Boolean::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Boolean::evaluate, py::const_), py::return_value_policy::reference)
        .def("evaluate", py::overload_cast<const States&, DenotationsCaches&>(&Boolean::evaluate, py::const_), py::return_value_policy::reference)
        .def("compute_complexity", &Boolean::compute_complexity)
        .def("compute_repr", &Boolean::compute_repr)
        .def("set_index", &Boolean::set_index)
        .def("get_index", &Boolean::get_index)
    ;

    py::class_<SyntacticElementFactory, std::shared_ptr<SyntacticElementFactory>>(m, "SyntacticElementFactory")
        .def(py::init<std::shared_ptr<const VocabularyInfo>>())
        .def("__copy__", [](const SyntacticElementFactory& factory, py::object){ return SyntacticElementFactory(factory); })
        .def("__deepcopy__", [](const SyntacticElementFactory& factory, py::object){ return SyntacticElementFactory(factory); })

        .def("parse_concept", &SyntacticElementFactory::parse_concept, py::arg("description"))
        .def("parse_role", &SyntacticElementFactory::parse_role, py::arg("description"))
        .def("parse_numerical", &SyntacticElementFactory::parse_numerical, py::arg("description"))
        .def("parse_boolean", &SyntacticElementFactory::parse_boolean, py::arg("description"))

        .def("make_empty_boolean", py::overload_cast<const Concept&>(&SyntacticElementFactory::make_empty_boolean), py::arg("concept"))
        .def("make_empty_boolean", py::overload_cast<const Role&>(&SyntacticElementFactory::make_empty_boolean), py::arg("role"))
        .def("make_inclusion_boolean", py::overload_cast<const Concept&, const Concept&>(&SyntacticElementFactory::make_inclusion_boolean), py::arg("concept_left"), py::arg("concept_right"))
        .def("make_inclusion_boolean", py::overload_cast<const Role&, const Role&>(&SyntacticElementFactory::make_inclusion_boolean), py::arg("role_left"), py::arg("role_right"))
        .def("make_nullary_boolean", &SyntacticElementFactory::make_nullary_boolean, py::arg("predicate"))

        .def("make_all_concept", &SyntacticElementFactory::make_all_concept, py::arg("role"), py::arg("concept"))
        .def("make_and_concept", &SyntacticElementFactory::make_and_concept, py::arg("concept_left"), py::arg("concept_right"))
        .def("make_bot_concept", &SyntacticElementFactory::make_bot_concept)
        .def("make_diff_concept", &SyntacticElementFactory::make_diff_concept, py::arg("concept_left"), py::arg("concept_right"))
        .def("make_equal_concept", &SyntacticElementFactory::make_equal_concept, py::arg("role_left"), py::arg("role_right"))
        .def("make_not_concept", &SyntacticElementFactory::make_not_concept, py::arg("concept"))
        .def("make_one_of_concept", &SyntacticElementFactory::make_one_of_concept, py::arg("constant"))
        .def("make_or_concept", &SyntacticElementFactory::make_or_concept, py::arg("concept_left"), py::arg("concept_right"))
        .def("make_projection_concept", &SyntacticElementFactory::make_projection_concept, py::arg("role"), py::arg("pos"))
        .def("make_primitive_concept", &SyntacticElementFactory::make_primitive_concept, py::arg("predicate"), py::arg("pos"))
        .def("make_some_concept", &SyntacticElementFactory::make_some_concept, py::arg("role"), py::arg("concept"))
        .def("make_subset_concept", &SyntacticElementFactory::make_subset_concept, py::arg("concept_left"), py::arg("concept_right"))
        .def("make_top_concept", &SyntacticElementFactory::make_top_concept)

        .def("make_concept_distance_numerical", &SyntacticElementFactory::make_concept_distance_numerical, py::arg("concept_from"), py::arg("role"), py::arg("concept_to"))
        .def("make_count_numerical", py::overload_cast<const Concept&>(&SyntacticElementFactory::make_count_numerical), py::arg("concept"))
        .def("make_count_numerical", py::overload_cast<const Role&>(&SyntacticElementFactory::make_count_numerical), py::arg("role"))
        .def("make_role_distance_numerical", &SyntacticElementFactory::make_role_distance_numerical, py::arg("role_from"), py::arg("role"), py::arg("role_to"))
        .def("make_sum_concept_distance_numerical", &SyntacticElementFactory::make_sum_concept_distance_numerical, py::arg("concept_from"), py::arg("role"), py::arg("concept_to"))
        .def("make_sum_role_distance_numerical", &SyntacticElementFactory::make_sum_role_distance_numerical, py::arg("role_from"), py::arg("role"), py::arg("role_to"))

        .def("make_and_role", &SyntacticElementFactory::make_and_role, py::arg("role_left"), py::arg("role_right"))
        .def("make_compose_role", &SyntacticElementFactory::make_compose_role, py::arg("role_left"), py::arg("role_right"))
        .def("make_diff_role", &SyntacticElementFactory::make_diff_role, py::arg("role_left"), py::arg("role_right"))
        .def("make_identity_role", &SyntacticElementFactory::make_identity_role, py::arg("concept"))
        .def("make_inverse_role", &SyntacticElementFactory::make_inverse_role, py::arg("role"))
        .def("make_not_role", &SyntacticElementFactory::make_not_role, py::arg("role"))
        .def("make_or_role", &SyntacticElementFactory::make_or_role, py::arg("role_left"), py::arg("role_right"))
        .def("make_primitive_role", &SyntacticElementFactory::make_primitive_role, py::arg("role"), py::arg("pos_1"), py::arg("pos_2"))
        .def("make_restrict_role", &SyntacticElementFactory::make_restrict_role, py::arg("role"), py::arg("concept"))
        .def("make_top_role", &SyntacticElementFactory::make_top_role)
        .def("make_transitive_closure", &SyntacticElementFactory::make_transitive_closure, py::arg("role"))
        .def("make_transitive_reflexive_closure", &SyntacticElementFactory::make_transitive_reflexive_closure, py::arg("role"))

        .def("get_vocabulary_info", &SyntacticElementFactory::get_vocabulary_info)
    ;
}
