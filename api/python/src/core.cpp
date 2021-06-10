#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // Necessary for automatic conversion of e.g. std::vectors

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlp/core.h>

namespace py = pybind11;

using namespace dlp;

void init_core(py::module_ &m) {
    py::class_<core::Object>(m, "Object")
        .def("get_index", &core::Object::get_index)
        .def("get_name", &core::Object::get_name)
    ;

    py::class_<core::Predicate>(m, "Predicate")
        .def("get_name", &core::Predicate::get_name)
        .def("get_arity", &core::Predicate::get_arity)
        .def("get_predicate_idx", &core::Predicate::get_predicate_idx)
    ;

    py::class_<core::VocabularyInfo, std::shared_ptr<core::VocabularyInfo>>(m, "VocabularyInfo")
        .def(py::init<>())
        .def("add_predicate", &core::VocabularyInfo::add_predicate)
        .def("exists_predicate_name", &core::VocabularyInfo::exists_predicate_name)
    ;

    py::class_<core::Atom>(m, "Atom")
        .def("get_name", &core::Atom::get_name)
        .def("get_index", &core::Atom::get_index)
        .def("get_predicate", &core::Atom::get_predicate)
        .def("get_objects", &core::Atom::get_objects)
        .def("get_object", &core::Atom::get_object)
        .def("get_is_static", &core::Atom::get_is_static)
    ;

    py::class_<core::State>(m, "State")
        .def(py::init<std::shared_ptr<const core::InstanceInfo>, const std::vector<core::Atom>&>())
//        .def("__repr__", &State::str)
        .def("__repr__", [](const core::State& o){return o.str();})
        .def("get_atom_idxs", &core::State::get_atom_idxs)
    ;

    py::class_<core::Concept>(m, "ConceptElement")
        .def("evaluate", &core::Concept::evaluate)
        .def("compute_complexity", &core::Concept::compute_complexity)
        .def("compute_repr", &core::Concept::compute_repr)
    ;

    py::class_<core::Role>(m, "RoleElement")
        .def("evaluate", &core::Role::evaluate)
        .def("compute_complexity", &core::Role::compute_complexity)
        .def("compute_repr", &core::Role::compute_repr)
    ;

    py::class_<core::Numerical>(m, "NumericalElement")
        .def("evaluate", &core::Numerical::evaluate)
        .def("compute_complexity", &core::Numerical::compute_complexity)
        .def("compute_repr", &core::Numerical::compute_repr)
    ;

    py::class_<core::Boolean>(m, "BooleanElement")
        .def("evaluate", &core::Boolean::evaluate)
        .def("compute_complexity", &core::Boolean::compute_complexity)
        .def("compute_repr", &core::Boolean::compute_repr)
    ;

    py::class_<core::SyntacticElementFactory, std::shared_ptr<core::SyntacticElementFactory>>(m, "SyntacticElementFactory")
        .def(py::init<std::shared_ptr<const core::VocabularyInfo>>())

        .def("parse_concept", &core::SyntacticElementFactory::parse_concept)
        .def("parse_role", &core::SyntacticElementFactory::parse_role)
        .def("parse_numerical", &core::SyntacticElementFactory::parse_numerical)
        .def("parse_boolean", &core::SyntacticElementFactory::parse_boolean)

        .def("make_empty_boolean", py::overload_cast<const core::Concept&>(&core::SyntacticElementFactory::make_empty_boolean))
        .def("make_empty_boolean", py::overload_cast<const core::Role&>(&core::SyntacticElementFactory::make_empty_boolean))
        .def("make_all_concept", &core::SyntacticElementFactory::make_all_concept)
        .def("make_and_concept", &core::SyntacticElementFactory::make_and_concept)
        .def("make_bot_concept", &core::SyntacticElementFactory::make_bot_concept)
        .def("make_diff_concept", &core::SyntacticElementFactory::make_diff_concept)
        .def("make_not_concept", &core::SyntacticElementFactory::make_not_concept)
        .def("make_one_of_concept", &core::SyntacticElementFactory::make_one_of_concept)
        .def("make_or_concept", &core::SyntacticElementFactory::make_or_concept)
        .def("make_primitive_concept", &core::SyntacticElementFactory::make_primitive_concept)
        .def("make_some_concept", &core::SyntacticElementFactory::make_some_concept)
        .def("make_subset_concept", &core::SyntacticElementFactory::make_subset_concept)
        .def("make_top_concept", &core::SyntacticElementFactory::make_top_concept)
        .def("make_concept_distance", &core::SyntacticElementFactory::make_concept_distance)

        .def("make_role_distance", &core::SyntacticElementFactory::make_role_distance)
        .def("make_sum_concept_distance", &core::SyntacticElementFactory::make_sum_concept_distance)
        .def("make_sum_role_distance", &core::SyntacticElementFactory::make_sum_role_distance)
        .def("make_and_role", &core::SyntacticElementFactory::make_and_role)
        .def("make_compose_role", &core::SyntacticElementFactory::make_compose_role)
        .def("make_diff_role", &core::SyntacticElementFactory::make_diff_role)
        .def("make_identity_role", &core::SyntacticElementFactory::make_identity_role)
        .def("make_inverse_role", &core::SyntacticElementFactory::make_inverse_role)
        .def("make_not_role", &core::SyntacticElementFactory::make_not_role)
        .def("make_or_role", &core::SyntacticElementFactory::make_or_role)
        .def("make_primitive_role", &core::SyntacticElementFactory::make_primitive_role)

        .def("make_restrict_role", &core::SyntacticElementFactory::make_restrict_role)
        .def("make_top_role", &core::SyntacticElementFactory::make_top_role)
        .def("make_transitive_closure", &core::SyntacticElementFactory::make_transitive_closure)
        .def("make_transitive_reflexive_closure", &core::SyntacticElementFactory::make_transitive_reflexive_closure)
    ;

    py::class_<core::InstanceInfo, std::shared_ptr<core::InstanceInfo>>(m, "InstanceInfo")
        .def(py::init<std::shared_ptr<core::VocabularyInfo>>())
        .def("add_atom", &core::InstanceInfo::add_atom)
        .def("add_static_atom", &core::InstanceInfo::add_static_atom)
        .def("get_atoms", &core::InstanceInfo::get_atoms)
//        .def("parse_state", &InstanceInfo::parse_state)
        // TODO Will need to disambiguate:
//        .def("convert_state", py::overload_cast<const std::vector<Atom> &>(&InstanceInfo::convert_state))
//        .def("convert_state", py::overload_cast<const std::vector<int> &>(&InstanceInfo::convert_state))
//        .def("convert_state", &InstanceInfo::convert_state)
    ;
}
