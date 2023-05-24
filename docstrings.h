/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_dlplan_core_Atom = R"doc(An Atom belongs to a specific instance.)doc";

static const char *__doc_dlplan_core_Atom_Atom = R"doc()doc";

static const char *__doc_dlplan_core_Atom_Atom_2 = R"doc()doc";

static const char *__doc_dlplan_core_Atom_Atom_3 = R"doc()doc";

static const char *__doc_dlplan_core_Atom_get_index = R"doc()doc";

static const char *__doc_dlplan_core_Atom_get_name = R"doc(Getters.)doc";

static const char *__doc_dlplan_core_Atom_get_object_indices = R"doc()doc";

static const char *__doc_dlplan_core_Atom_get_predicate_index = R"doc()doc";

static const char *__doc_dlplan_core_Atom_is_static = R"doc()doc";

static const char *__doc_dlplan_core_Atom_m_index = R"doc()doc";

static const char *__doc_dlplan_core_Atom_m_is_static = R"doc()doc";

static const char *__doc_dlplan_core_Atom_m_name = R"doc()doc";

static const char *__doc_dlplan_core_Atom_m_object_indices = R"doc()doc";

static const char *__doc_dlplan_core_Atom_m_predicate_index = R"doc()doc";

static const char *__doc_dlplan_core_Atom_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Atom_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Atom_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_Atom_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_BaseElement = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_compute_complexity =
R"doc(Returns the complexity of the element measured in the size of the
abstract syntax tree.)doc";

static const char *__doc_dlplan_core_BaseElement_compute_repr = R"doc(Returns a canonical string representation.)doc";

static const char *__doc_dlplan_core_BaseElement_compute_repr_2 = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_get_index = R"doc(Getters.)doc";

static const char *__doc_dlplan_core_BaseElement_get_vocabulary_info = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_is_static = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_m_index = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_m_is_static = R"doc(if true then element is evaluated per instance rather than per state.)doc";

static const char *__doc_dlplan_core_BaseElement_m_vocabulary_info = R"doc()doc";

static const char *__doc_dlplan_core_BaseElement_set_index = R"doc(Setters.)doc";

static const char *__doc_dlplan_core_Boolean = R"doc(Boolean evaluates to bool.)doc";

static const char *__doc_dlplan_core_Boolean_2 = R"doc(Boolean evaluates to bool.)doc";

static const char *__doc_dlplan_core_Boolean_Boolean = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_Boolean_2 = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_Boolean_3 = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_evaluate = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_evaluate_2 = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_evaluate_3 = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_evaluate_impl = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_evaluate_impl_2 = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Boolean_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Concept = R"doc(Concept evaluates to ConceptDenotation.)doc";

static const char *__doc_dlplan_core_Concept_2 = R"doc(Concept evaluates to ConceptDenotation.)doc";

static const char *__doc_dlplan_core_ConceptDenotation = R"doc(Represents a set of objects from an instance.)doc";

static const char *__doc_dlplan_core_ConceptDenotation_2 = R"doc(Represents a set of objects from an instance.)doc";

static const char *__doc_dlplan_core_ConceptDenotation_ConceptDenotation = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_ConceptDenotation_2 = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_ConceptDenotation_3 = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_begin = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_const_iterator = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_m_data = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_m_index = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_m_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_operator_inc = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_operator_inc_2 = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_operator_mul = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_const_iterator_seek_next = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_contains = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_empty = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_end = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_erase = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_get_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_hash = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_insert = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_intersects = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_is_subset_of = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_m_data = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_m_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_bnot = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_iand = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_ior = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_isub = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_set = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_size = R"doc()doc";

static const char *__doc_dlplan_core_ConceptDenotation_str =
R"doc(Get a string representation of the object.

Returns:
    A string representation of the object.)doc";

static const char *__doc_dlplan_core_ConceptDenotation_to_sorted_vector =
R"doc(Compute and return a sparse and continuous representation of the
objects in the set.

Returns:
    A vector of indices of the objects in the set.)doc";

static const char *__doc_dlplan_core_Concept_Concept = R"doc()doc";

static const char *__doc_dlplan_core_Concept_Concept_2 = R"doc()doc";

static const char *__doc_dlplan_core_Concept_Concept_3 = R"doc()doc";

static const char *__doc_dlplan_core_Concept_evaluate = R"doc()doc";

static const char *__doc_dlplan_core_Concept_evaluate_2 = R"doc()doc";

static const char *__doc_dlplan_core_Concept_evaluate_3 = R"doc()doc";

static const char *__doc_dlplan_core_Concept_evaluate_impl = R"doc()doc";

static const char *__doc_dlplan_core_Concept_evaluate_impl_2 = R"doc()doc";

static const char *__doc_dlplan_core_Concept_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Concept_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Constant =
R"doc(A Constant is a special element from the universe.

A Constant is a special element from the universe. That is, specific
Elements can refer directly to Constants. In the context of planning,
a Constant is an element that occurs in every problem instance.

The Constant class provides methods to retrieve the name and index of
a Constant, and supports comparison operators for equality and
inequality.)doc";

static const char *__doc_dlplan_core_Constant_Constant = R"doc()doc";

static const char *__doc_dlplan_core_Constant_Constant_2 = R"doc()doc";

static const char *__doc_dlplan_core_Constant_Constant_3 = R"doc()doc";

static const char *__doc_dlplan_core_Constant_Constant_4 = R"doc()doc";

static const char *__doc_dlplan_core_Constant_get_index =
R"doc(Retrieves the index of the constant.

Returns:
    The index of the constant.)doc";

static const char *__doc_dlplan_core_Constant_get_name =
R"doc(Retrieves the name of the constant.

Returns:
    The name of the constant.)doc";

static const char *__doc_dlplan_core_Constant_m_index = R"doc()doc";

static const char *__doc_dlplan_core_Constant_m_name = R"doc()doc";

static const char *__doc_dlplan_core_Constant_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Constant_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Constant_operator_eq =
R"doc(Checks if this constant is equal to another constant.

Parameter ``other``:
    The constant to compare against.

Returns:
    True if the constants are equal, false otherwise.)doc";

static const char *__doc_dlplan_core_Constant_operator_ne =
R"doc(Checks if this constant is not equal to another constant.

Parameter ``other``:
    The constant to compare against.

Returns:
    True if the constants are not equal, false otherwise.)doc";

static const char *__doc_dlplan_core_DenotationsCaches = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_b_denots_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_b_denots_mapping = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_b_denots_mapping_per_instance = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_b_denots_mapping_per_state = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_c_denot_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_c_denots_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_c_denots_mapping = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_c_denots_mapping_per_instance = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_c_denots_mapping_per_state = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_n_denots_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_n_denots_mapping = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_n_denots_mapping_per_instance = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_n_denots_mapping_per_state = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_r_denot_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_r_denots_cache = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_r_denots_mapping = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_r_denots_mapping_per_instance = R"doc()doc";

static const char *__doc_dlplan_core_DenotationsCaches_m_r_denots_mapping_per_state = R"doc()doc";

static const char *__doc_dlplan_core_DerefEqual = R"doc(Compares two std::unique_ptr<T> by comparing objects T.)doc";

static const char *__doc_dlplan_core_DerefEqual_operator_call = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo = R"doc(InstanceInfo stores information related to the planning instance.)doc";

static const char *__doc_dlplan_core_InstanceInfo_2 = R"doc(InstanceInfo stores information related to the planning instance.)doc";

static const char *__doc_dlplan_core_InstanceInfo_InstanceInfo = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_InstanceInfo_2 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_InstanceInfo_3 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_InstanceInfo_4 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom_2 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom_3 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom_4 = R"doc(Alternative 1 to add atoms.)doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom_5 = R"doc(Alternative 2 to add atoms.)doc";

static const char *__doc_dlplan_core_InstanceInfo_add_atom_6 = R"doc(Alternative 3 to add atoms.)doc";

static const char *__doc_dlplan_core_InstanceInfo_add_object = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_static_atom = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_static_atom_2 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_add_static_atom_3 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_atom = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_atoms = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_index = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_object = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_objects = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_static_atoms = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_get_vocabulary_info = R"doc(Getters.)doc";

static const char *__doc_dlplan_core_InstanceInfo_m_atom_name_to_index = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_atoms = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_index = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_object_name_to_index = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_objects = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_static_atom_name_to_index = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_static_atoms = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_m_vocabulary_info = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_InstanceInfo_set_index = R"doc(Setters.)doc";

static const char *__doc_dlplan_core_Numerical = R"doc(Numerical evaluates to int.)doc";

static const char *__doc_dlplan_core_Numerical_2 = R"doc(Numerical evaluates to int.)doc";

static const char *__doc_dlplan_core_Numerical_Numerical = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_Numerical_2 = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_Numerical_3 = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_evaluate = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_evaluate_2 = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_evaluate_3 = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_evaluate_impl = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_evaluate_impl_2 = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Numerical_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Object = R"doc(An Object belongs to a specific instance.)doc";

static const char *__doc_dlplan_core_Object_Object = R"doc()doc";

static const char *__doc_dlplan_core_Object_Object_2 = R"doc()doc";

static const char *__doc_dlplan_core_Object_Object_3 = R"doc()doc";

static const char *__doc_dlplan_core_Object_get_index = R"doc()doc";

static const char *__doc_dlplan_core_Object_get_name = R"doc()doc";

static const char *__doc_dlplan_core_Object_m_index = R"doc()doc";

static const char *__doc_dlplan_core_Object_m_name = R"doc()doc";

static const char *__doc_dlplan_core_Object_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Object_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Object_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_Object_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_Predicate = R"doc(A Predicate is a name for a relation)doc";

static const char *__doc_dlplan_core_Predicate_Predicate = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_Predicate_2 = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_Predicate_3 = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_get_arity = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_get_index = R"doc(Getters.)doc";

static const char *__doc_dlplan_core_Predicate_get_name = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_is_static = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_m_arity = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_m_index = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_m_is_static = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_m_name = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_Predicate_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_Role = R"doc(Concept evaluates to RoleDenotation.)doc";

static const char *__doc_dlplan_core_Role_2 = R"doc(Concept evaluates to RoleDenotation.)doc";

static const char *__doc_dlplan_core_RoleDenotation = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_2 = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_RoleDenotation = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_RoleDenotation_2 = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_RoleDenotation_3 = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_begin = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_const_iterator = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_m_data = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_m_indices = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_m_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_inc = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_inc_2 = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_mul = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_operator_sub = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_const_iterator_seek_next = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_contains = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_empty = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_end = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_erase = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_get_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_hash = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_insert = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_intersects = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_is_subset_of = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_m_data = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_m_num_objects = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_bnot = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_iand = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_ior = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_isub = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_set = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_size = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_str = R"doc()doc";

static const char *__doc_dlplan_core_RoleDenotation_to_sorted_vector = R"doc()doc";

static const char *__doc_dlplan_core_Role_Role = R"doc()doc";

static const char *__doc_dlplan_core_Role_Role_2 = R"doc()doc";

static const char *__doc_dlplan_core_Role_Role_3 = R"doc()doc";

static const char *__doc_dlplan_core_Role_evaluate = R"doc()doc";

static const char *__doc_dlplan_core_Role_evaluate_2 = R"doc()doc";

static const char *__doc_dlplan_core_Role_evaluate_3 = R"doc()doc";

static const char *__doc_dlplan_core_Role_evaluate_impl = R"doc()doc";

static const char *__doc_dlplan_core_Role_evaluate_impl_2 = R"doc()doc";

static const char *__doc_dlplan_core_Role_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_Role_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_State = R"doc()doc";

static const char *__doc_dlplan_core_State_2 = R"doc()doc";

static const char *__doc_dlplan_core_State_State = R"doc()doc";

static const char *__doc_dlplan_core_State_State_2 = R"doc()doc";

static const char *__doc_dlplan_core_State_State_3 = R"doc()doc";

static const char *__doc_dlplan_core_State_State_4 = R"doc()doc";

static const char *__doc_dlplan_core_State_get_atom_indices = R"doc()doc";

static const char *__doc_dlplan_core_State_get_index = R"doc()doc";

static const char *__doc_dlplan_core_State_get_instance_info = R"doc(Getters.)doc";

static const char *__doc_dlplan_core_State_hash = R"doc(Compute a 64-Bit hash value.)doc";

static const char *__doc_dlplan_core_State_m_atom_indices = R"doc()doc";

static const char *__doc_dlplan_core_State_m_index = R"doc()doc";

static const char *__doc_dlplan_core_State_m_instance_info = R"doc()doc";

static const char *__doc_dlplan_core_State_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_State_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_State_operator_eq = R"doc()doc";

static const char *__doc_dlplan_core_State_operator_ne = R"doc()doc";

static const char *__doc_dlplan_core_State_set_index = R"doc(Setters.)doc";

static const char *__doc_dlplan_core_State_str = R"doc(Computes string-like representation of the state.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactory =
R"doc(The SyntacticElementFactory for creation of syntactically unique
elements.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_2 =
R"doc(The SyntacticElementFactory for creation of syntactically unique
elements.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactoryImpl = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_SyntacticElementFactory = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_SyntacticElementFactory_2 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_SyntacticElementFactory_3 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_get_vocabulary_info = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_m_pImpl = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_all_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_and_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_and_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_bot_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_compose_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_concept_distance_numerical = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_count_numerical = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_count_numerical_2 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_diff_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_diff_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_empty_boolean = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_empty_boolean_2 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_equal_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_identity_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_inclusion_boolean = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_inclusion_boolean_2 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_inverse_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_not_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_not_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_nullary_boolean = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_one_of_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_or_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_or_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_primitive_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_primitive_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_projection_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_restrict_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_role_distance_numerical = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_some_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_subset_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_sum_concept_distance_numerical = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_sum_role_distance_numerical = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_top_concept = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_top_role = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_transitive_closure = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_make_transitive_reflexive_closure = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_operator_assign_2 = R"doc()doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_parse_boolean =
R"doc(Returns a Boolean if the description is correct. If description is
incorrect, throw an error with human readable information.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_parse_concept =
R"doc(Returns a Concept if the description is correct. If description is
incorrect, throw an error with human readable information.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_parse_numerical =
R"doc(Returns a Numerical if the description is correct. If description is
incorrect, throw an error with human readable information.)doc";

static const char *__doc_dlplan_core_SyntacticElementFactory_parse_role =
R"doc(Returns a Role if the description is correct. If description is
incorrect, throw an error with human readable information.)doc";

static const char *__doc_dlplan_core_VocabularyInfo = R"doc(VocabularyInfo stores information related to the planning domain.)doc";

static const char *__doc_dlplan_core_VocabularyInfo_2 = R"doc(VocabularyInfo stores information related to the planning domain.)doc";

static const char *__doc_dlplan_core_VocabularyInfo_VocabularyInfo = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_VocabularyInfo_2 = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_VocabularyInfo_3 = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_add_constant = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_add_predicate = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_get_constant = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_get_constants = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_get_predicate = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_get_predicates = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_m_constant_name_to_index = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_m_constants = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_m_predicate_name_to_index = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_m_predicates = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_operator_assign = R"doc()doc";

static const char *__doc_dlplan_core_VocabularyInfo_operator_assign_2 = R"doc()doc";

static const char *__doc_std_hash = R"doc()doc";

static const char *__doc_std_hash_2 = R"doc()doc";

static const char *__doc_std_hash_3 = R"doc()doc";

static const char *__doc_std_hash_4 = R"doc()doc";

static const char *__doc_std_hash_5 = R"doc()doc";

static const char *__doc_std_hash_6 = R"doc()doc";

static const char *__doc_std_hash_7 = R"doc()doc";

static const char *__doc_std_hash_8 = R"doc()doc";

static const char *__doc_std_hash_9 = R"doc()doc";

static const char *__doc_std_hash_10 = R"doc()doc";

static const char *__doc_std_hash_11 = R"doc()doc";

static const char *__doc_std_hash_operator_call = R"doc()doc";

static const char *__doc_std_hash_operator_call_2 = R"doc()doc";

static const char *__doc_std_hash_operator_call_3 = R"doc()doc";

static const char *__doc_std_hash_operator_call_4 = R"doc()doc";

static const char *__doc_std_hash_operator_call_5 = R"doc()doc";

static const char *__doc_std_hash_operator_call_6 = R"doc()doc";

static const char *__doc_std_hash_operator_call_7 = R"doc()doc";

static const char *__doc_std_hash_operator_call_8 = R"doc()doc";

static const char *__doc_std_hash_operator_call_9 = R"doc()doc";

static const char *__doc_std_hash_operator_call_10 = R"doc()doc";

static const char *__doc_std_hash_operator_call_11 = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

