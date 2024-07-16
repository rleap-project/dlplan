#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include "../../../include/dlplan/core.h"

namespace py = pybind11;

using namespace dlplan;
using namespace dlplan::core;


void init_core(py::module_ &m_core) {
    py::class_<ConceptDenotation, std::shared_ptr<ConceptDenotation>>(m_core, "ConceptDenotation")
        .def(py::init<int>())
        .def("__eq__", &ConceptDenotation::are_equal_impl)
        .def("__ne__", &ConceptDenotation::operator!=)
        .def("__str__", py::overload_cast<>(&ConceptDenotation::str, py::const_))
        .def("__len__", &ConceptDenotation::size)
        .def("__hash__", &ConceptDenotation::hash)
        .def("contains", &ConceptDenotation::contains)
        .def("set", &ConceptDenotation::set)
        .def("insert", &ConceptDenotation::insert)
        .def("erase", &ConceptDenotation::erase)
        .def("intersects", &ConceptDenotation::intersects)
        .def("is_subset_of", &ConceptDenotation::is_subset_of)
        .def("to_vector", &ConceptDenotation::to_vector)
        .def("to_sorted_vector", &ConceptDenotation::to_sorted_vector)
        .def("get_num_objects", &ConceptDenotation::get_num_objects)
    ;

    py::class_<RoleDenotation, std::shared_ptr<RoleDenotation>>(m_core, "RoleDenotation")
        .def(py::init<int>())
        .def("__eq__", &RoleDenotation::are_equal_impl)
        .def("__ne__", &RoleDenotation::operator!=)
        .def("__str__", py::overload_cast<>(&RoleDenotation::str, py::const_))
        .def("__len__", &RoleDenotation::size)
        .def("__hash__", &RoleDenotation::hash)
        .def("contains", &RoleDenotation::contains)
        .def("set", &RoleDenotation::set)
        .def("insert", &RoleDenotation::insert)
        .def("erase", &RoleDenotation::erase)
        .def("intersects", &RoleDenotation::intersects)
        .def("is_subset_of", &RoleDenotation::is_subset_of)
        .def("to_vector", &RoleDenotation::to_vector)
        .def("to_sorted_vector", &RoleDenotation::to_sorted_vector)
        .def("get_num_objects", &RoleDenotation::get_num_objects)
    ;

    py::class_<DenotationsCaches, std::shared_ptr<DenotationsCaches>>(m_core, "DenotationsCaches")
        .def(py::init<>())
    ;

    py::class_<Constant>(m_core, "Constant")
        .def("__eq__", &Constant::are_equal_impl)
        .def("__ne__", &Constant::operator!=)
        .def("__str__", py::overload_cast<>(&Constant::str, py::const_))
        .def("get_index", &Constant::get_index)
        .def("get_name", &Constant::get_name)
    ;

    py::class_<Predicate>(m_core, "Predicate")
        .def("__eq__", &Predicate::are_equal_impl)
        .def("__ne__", &Predicate::operator!=)
        .def("__str__", py::overload_cast<>(&Predicate::str, py::const_))
        .def("get_index", &Predicate::get_index)
        .def("get_name", &Predicate::get_name)
        .def("get_arity", &Predicate::get_arity)
        .def("is_static", &Predicate::is_static)
    ;

    py::class_<VocabularyInfo, std::shared_ptr<VocabularyInfo>>(m_core, "VocabularyInfo")
        .def(py::init<>())
        .def("__str__", py::overload_cast<>(&VocabularyInfo::str, py::const_))
        .def("add_predicate", &VocabularyInfo::add_predicate, py::arg("name"), py::arg("arity"), py::arg("is_static") = false)
        .def("add_constant", &VocabularyInfo::add_constant)
        .def("get_predicates", &VocabularyInfo::get_predicates)
        .def("get_constants", &VocabularyInfo::get_constants)
        .def("get_predicate", &VocabularyInfo::get_predicate)
        .def("get_constant", &VocabularyInfo::get_constant)
    ;

    py::class_<Object>(m_core, "Object")
        .def("__eq__", &Object::are_equal_impl)
        .def("__ne__", &Object::operator!=)
        .def("__str__", py::overload_cast<>(&Object::str, py::const_))
        .def("get_index", &Object::get_index)
        .def("get_name", &Object::get_name)
    ;

    py::class_<Atom>(m_core, "Atom")
        .def("__eq__", &Atom::are_equal_impl)
        .def("__ne__", &Atom::operator!=)
        .def("__str__",  py::overload_cast<>(&Atom::str, py::const_))
        .def("get_index", &Atom::get_index)
        .def("get_name", &Atom::get_name)
        .def("get_predicate_index", &Atom::get_predicate_index)
        .def("get_object_indices", &Atom::get_object_indices)
        .def("is_static", &Atom::is_static)
    ;

    py::class_<InstanceInfo, std::shared_ptr<InstanceInfo>>(m_core, "InstanceInfo")
        .def(py::init<int, std::shared_ptr<VocabularyInfo>>(), py::arg("index"), py::arg("vocabulary_info"))
        .def("__str__",  py::overload_cast<>(&InstanceInfo::str, py::const_))
        .def("add_object", &InstanceInfo::add_object)
        .def("add_atom", py::overload_cast<const Predicate&, const std::vector<Object>&>(&InstanceInfo::add_atom))
        .def("add_atom", py::overload_cast<int, const std::vector<int>&>(&InstanceInfo::add_atom))
        .def("add_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&InstanceInfo::add_atom))
        .def("add_static_atom", py::overload_cast<const Predicate&, const std::vector<Object>&>(&InstanceInfo::add_static_atom))
        .def("add_static_atom", py::overload_cast<int, const std::vector<int>&>(&InstanceInfo::add_static_atom))
        .def("add_static_atom", py::overload_cast<const std::string&, const std::vector<std::string>&>(&InstanceInfo::add_static_atom))
        .def("get_index", &InstanceInfo::get_index)
        .def("get_objects", &InstanceInfo::get_objects)
        .def("get_atoms", &InstanceInfo::get_atoms)
        .def("get_static_atoms", &InstanceInfo::get_static_atoms)
        .def("get_object", &InstanceInfo::get_object)
        .def("get_atom", &InstanceInfo::get_atom)
        .def("get_vocabulary_info", &InstanceInfo::get_vocabulary_info)
        .def("clear_atoms", &InstanceInfo::clear_atoms)
        .def("clear_static_atoms", &InstanceInfo::clear_static_atoms)
    ;

    py::class_<State>(m_core, "State")
        .def(py::init<int, std::shared_ptr<InstanceInfo>, const std::vector<Atom>&>(), py::arg("index"), py::arg("instance_info"), py::arg("atoms"))
        .def(py::init<int, std::shared_ptr<InstanceInfo>, const std::vector<int>&>(), py::arg("index"), py::arg("instance_info"), py::arg("atom_indices"))
        .def(py::init<int, std::shared_ptr<InstanceInfo>, std::vector<int>&&>(), py::arg("index"), py::arg("instance_info"), py::arg("atom_indices"))
        .def("__eq__", &State::are_equal_impl)
        .def("__ne__", &State::operator!=)
        .def("__str__",  py::overload_cast<>(&State::str, py::const_))
        .def("__hash__", &State::hash)
        .def("get_index", &State::get_index)
        .def("get_atom_indices", &State::get_atom_indices)
        .def("get_instance_info", &State::get_instance_info)
    ;

    py::class_<Concept, std::shared_ptr<Concept>>(m_core, "Concept")
        .def("__str__",  py::overload_cast<>(&Concept::str, py::const_))
        .def("compute_complexity", &Concept::compute_complexity)
        .def("get_index", &Concept::get_index)
        .def("get_vocabulary_info", &Concept::get_vocabulary_info)
        .def("evaluate", py::overload_cast<const State&>(&Concept::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Concept::evaluate, py::const_))
        .def("evaluate", [](const Concept& self, const States& states, DenotationsCaches& caches) {
            // std::shared_ptr<const std::vector<std::shared_ptr<const ConcepDenotation>>> is not registered so we must dereference to obtain a registered type
            return *self.evaluate(states, caches);
        })
    ;

    py::class_<Role, std::shared_ptr<Role>>(m_core, "Role")
        .def("__str__", py::overload_cast<>(&Role::str, py::const_))
        .def("compute_complexity", &Role::compute_complexity)
        .def("get_index", &Role::get_index)
        .def("get_vocabulary_info", &Role::get_vocabulary_info)
        .def("evaluate", py::overload_cast<const State&>(&Role::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Role::evaluate, py::const_))
        .def("evaluate", [](const Role& self, const States& states, DenotationsCaches& caches) {
            // std::shared_ptr<const std::vector<std::shared_ptr<const RoleDenotation>>> is not registered so we must dereference to obtain a registered type
            return *self.evaluate(states, caches);
        })
    ;

    py::class_<Numerical, std::shared_ptr<Numerical>>(m_core, "Numerical")
        .def("__str__", py::overload_cast<>(&Numerical::str, py::const_))
        .def("compute_complexity", &Numerical::compute_complexity)
        .def("get_index", &Numerical::get_index)
        .def("get_vocabulary_info", &Numerical::get_vocabulary_info)
        .def("evaluate", py::overload_cast<const State&>(&Numerical::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Numerical::evaluate, py::const_))
        .def("evaluate", [](const Numerical& self, const States& states, DenotationsCaches& caches) {
            // std::shared_ptr<const std::vector<std::shared_ptr<const NumericalDenotation>>> is not registered so we must dereference to obtain a registered type
            return *self.evaluate(states, caches);
        })
    ;

    py::class_<Boolean, std::shared_ptr<Boolean>>(m_core, "Boolean")
        .def("__str__", py::overload_cast<>(&Boolean::str, py::const_))
        .def("compute_complexity", &Boolean::compute_complexity)
        .def("get_index", &Boolean::get_index)
        .def("get_vocabulary_info", &Boolean::get_vocabulary_info)
        .def("evaluate", py::overload_cast<const State&>(&Boolean::evaluate, py::const_))
        .def("evaluate", py::overload_cast<const State&, DenotationsCaches&>(&Boolean::evaluate, py::const_))
        .def("evaluate", [](const Boolean& self, const States& states, DenotationsCaches& caches) {
            // std::shared_ptr<const std::vector<std::shared_ptr<const BooleanDenotation>>> is not registered so we must dereference to obtain a registered type
            return *self.evaluate(states, caches);
        })
    ;

    py::class_<SyntacticElementFactory, std::shared_ptr<SyntacticElementFactory>>(m_core, "SyntacticElementFactory")
        .def(py::init<std::shared_ptr<VocabularyInfo>>())

        .def("parse_concept", py::overload_cast<const std::string&, const std::string&>(&SyntacticElementFactory::parse_concept), py::arg("description"), py::arg("filename") = "")
        .def("parse_role", py::overload_cast<const std::string&, const std::string&>(&SyntacticElementFactory::parse_role), py::arg("description"), py::arg("filename") = "")
        .def("parse_numerical", py::overload_cast<const std::string&, const std::string&>(&SyntacticElementFactory::parse_numerical), py::arg("description"), py::arg("filename") = "")
        .def("parse_boolean", py::overload_cast<const std::string&, const std::string&>(&SyntacticElementFactory::parse_boolean), py::arg("description"), py::arg("filename") = "")

        .def("make_empty_boolean", py::overload_cast<const std::shared_ptr<const Concept>&>(&SyntacticElementFactory::make_empty_boolean))
        .def("make_empty_boolean", py::overload_cast<const std::shared_ptr<const Role>&>(&SyntacticElementFactory::make_empty_boolean))
        .def("make_inclusion_boolean", py::overload_cast<const std::shared_ptr<const Concept>&, const std::shared_ptr<const Concept>&>(&SyntacticElementFactory::make_inclusion_boolean))
        .def("make_inclusion_boolean", py::overload_cast<const std::shared_ptr<const Role>&, const std::shared_ptr<const Role>&>(&SyntacticElementFactory::make_inclusion_boolean))
        .def("make_nullary_boolean", &SyntacticElementFactory::make_nullary_boolean)

        .def("make_all_concept", &SyntacticElementFactory::make_all_concept)
        .def("make_and_concept", &SyntacticElementFactory::make_and_concept)
        .def("make_bot_concept", &SyntacticElementFactory::make_bot_concept)
        .def("make_diff_concept", &SyntacticElementFactory::make_diff_concept)
        .def("make_equal_concept", &SyntacticElementFactory::make_equal_concept)
        .def("make_not_concept", &SyntacticElementFactory::make_not_concept)
        .def("make_one_of_concept", &SyntacticElementFactory::make_one_of_concept)
        .def("make_or_concept", &SyntacticElementFactory::make_or_concept)
        .def("make_projection_concept", &SyntacticElementFactory::make_projection_concept)
        .def("make_primitive_concept", &SyntacticElementFactory::make_primitive_concept)
        .def("make_some_concept", &SyntacticElementFactory::make_some_concept)
        .def("make_subset_concept", &SyntacticElementFactory::make_subset_concept)
        .def("make_top_concept", &SyntacticElementFactory::make_top_concept)

        .def("make_concept_distance_numerical", &SyntacticElementFactory::make_concept_distance_numerical)
        .def("make_count_numerical", py::overload_cast<const std::shared_ptr<const Concept>&>(&SyntacticElementFactory::make_count_numerical))
        .def("make_count_numerical", py::overload_cast<const std::shared_ptr<const Role>&>(&SyntacticElementFactory::make_count_numerical))
        .def("make_role_distance_numerical", &SyntacticElementFactory::make_role_distance_numerical)
        .def("make_sum_concept_distance_numerical", &SyntacticElementFactory::make_sum_concept_distance_numerical)
        .def("make_sum_role_distance_numerical", &SyntacticElementFactory::make_sum_role_distance_numerical)

        .def("make_and_role", &SyntacticElementFactory::make_and_role)
        .def("make_compose_role", &SyntacticElementFactory::make_compose_role)
        .def("make_diff_role", &SyntacticElementFactory::make_diff_role)
        .def("make_identity_role", &SyntacticElementFactory::make_identity_role)
        .def("make_inverse_role", &SyntacticElementFactory::make_inverse_role)
        .def("make_not_role", &SyntacticElementFactory::make_not_role)
        .def("make_or_role", &SyntacticElementFactory::make_or_role)
        .def("make_primitive_role", &SyntacticElementFactory::make_primitive_role)
        .def("make_restrict_role", &SyntacticElementFactory::make_restrict_role)
        .def("make_til_c_role", &SyntacticElementFactory::make_til_c_role)
        .def("make_top_role", &SyntacticElementFactory::make_top_role)
        .def("make_transitive_closure", &SyntacticElementFactory::make_transitive_closure)
        .def("make_transitive_reflexive_closure", &SyntacticElementFactory::make_transitive_reflexive_closure)

        .def("get_vocabulary_info", &SyntacticElementFactory::get_vocabulary_info)
    ;
}
