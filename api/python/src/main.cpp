#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

#include <dlp/core.h>

namespace py = pybind11;

using namespace dlp::core;

PYBIND11_MODULE(_dlplan, m) {
    m.doc() = "Python bindings for the dlplan description logics first-order features for planning";

    py::class_<Predicate>(m, "Predicate")
        .def("name", &Predicate::get_name)
        .def("arity", &Predicate::get_arity)
        .def("index", &Predicate::get_predicate_idx)
    ;

    py::class_<VocabularyInfo, std::shared_ptr<VocabularyInfo>>(m, "VocabularyInfo")
        .def(py::init<>())
        .def("add_predicate", &VocabularyInfo::add_predicate)
        .def("exists_predicate_name", &VocabularyInfo::exists_predicate_name)
    ;

//    declare_element_class<int>(m, "Int");
//    declare_element_class<int>(m, "Bool");

    py::class_<Element<int>>(m, "IntElement")
        .def("evaluate", &Element<int>::evaluate)
        .def("compute_complexity", &Element<int>::compute_complexity)
        .def("compute_repr", &Element<int>::compute_repr)
    ;

    py::class_<Element<bool>>(m, "BoolElement")
        .def("evaluate", &Element<bool>::evaluate)
        .def("compute_complexity", &Element<bool>::compute_complexity)
        .def("compute_repr", &Element<bool>::compute_repr)
    ;

    py::class_<Numerical, Element<int>>(m, "Numerical");
    py::class_<Boolean, Element<bool>>(m, "Boolean");

    py::class_<SyntacticElementFactory>(m, "SyntacticElementFactory")
        .def(py::init<std::shared_ptr<const VocabularyInfo>>())

        .def("parse_concept", &SyntacticElementFactory::parse_concept)
        .def("parse_role", &SyntacticElementFactory::parse_role)
        .def("parse_numerical", &SyntacticElementFactory::parse_numerical)
        .def("parse_boolean", &SyntacticElementFactory::parse_boolean)

        .def("make_empty_boolean", py::overload_cast<const Concept&>(&SyntacticElementFactory::make_empty_boolean))
        .def("make_empty_boolean", py::overload_cast<const Role&>(&SyntacticElementFactory::make_empty_boolean))
        .def("make_all_concept", &SyntacticElementFactory::make_all_concept)
        .def("make_and_concept", &SyntacticElementFactory::make_and_concept)
        .def("make_bot_concept", &SyntacticElementFactory::make_bot_concept)
        .def("make_diff_concept", &SyntacticElementFactory::make_diff_concept)
        .def("make_not_concept", &SyntacticElementFactory::make_not_concept)
        .def("make_one_of_concept", &SyntacticElementFactory::make_one_of_concept)
        .def("make_or_concept", &SyntacticElementFactory::make_or_concept)
        .def("make_primitive_concept", &SyntacticElementFactory::make_primitive_concept)
        .def("make_some_concept", &SyntacticElementFactory::make_some_concept)
        .def("make_subset_concept", &SyntacticElementFactory::make_subset_concept)
        .def("make_top_concept", &SyntacticElementFactory::make_top_concept)
        .def("make_concept_distance", &SyntacticElementFactory::make_concept_distance)

        .def("make_role_distance", &SyntacticElementFactory::make_role_distance)
        .def("make_sum_concept_distance", &SyntacticElementFactory::make_sum_concept_distance)
        .def("make_sum_role_distance", &SyntacticElementFactory::make_sum_role_distance)
        .def("make_and_role", &SyntacticElementFactory::make_and_role)
        .def("make_compose_role", &SyntacticElementFactory::make_compose_role)
        .def("make_diff_role", &SyntacticElementFactory::make_diff_role)
        .def("make_identity_role", &SyntacticElementFactory::make_identity_role)
        .def("make_inverse_role", &SyntacticElementFactory::make_inverse_role)
        .def("make_not_role", &SyntacticElementFactory::make_not_role)
        .def("make_or_role", &SyntacticElementFactory::make_or_role)
        .def("make_primitive_role", &SyntacticElementFactory::make_primitive_role)

        .def("make_restrict_role", &SyntacticElementFactory::make_restrict_role)
        .def("make_top_role", &SyntacticElementFactory::make_top_role)
        .def("make_transitive_closure", &SyntacticElementFactory::make_transitive_closure)
        .def("make_transitive_reflexive_closure", &SyntacticElementFactory::make_transitive_reflexive_closure)
    ;

    py::class_<InstanceInfo>(m, "InstanceInfo")
        .def(py::init<std::shared_ptr<VocabularyInfo>>())
        .def("add_atom", &InstanceInfo::add_atom)
        .def("add_static_atom", &InstanceInfo::add_static_atom)
//        .def("parse_state", &InstanceInfo::parse_state)
        // TODO Will need to disambiguate:
//        .def("convert_state", py::overload_cast<const std::vector<Atom> &>(&InstanceInfo::convert_state))
//        .def("convert_state", py::overload_cast<const std::vector<int> &>(&InstanceInfo::convert_state))
//        .def("convert_state", &InstanceInfo::convert_state)
    ;

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

template<typename T>
void declare_element_class(py::module &m, const std::string& typestr) {
    using Class = Element<T>;
    std::string classname = std::string("Element") + typestr;
    py::class_<Class>(m, classname.c_str())
        .def("evaluate", &Class::evaluate)
        .def("compute_complexity", &Class::compute_complexity)
        .def("compute_repr", &Class::compute_repr)
    ;
}