#include "../../include/dlplan/serialization.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/novelty.h"
#include "../../include/dlplan/policy.h"
#include "../../include/dlplan/state_space.h"

#include "../../src/core/element_factory.h"
#include "elements/booleans/empty.h"
#include "elements/booleans/inclusion.h"
#include "elements/booleans/nullary.h"
#include "elements/concepts/all.h"
#include "elements/concepts/bot.h"
#include "elements/concepts/and.h"
#include "elements/concepts/diff.h"
#include "elements/concepts/equal.h"
#include "elements/concepts/not.h"
#include "elements/concepts/one_of.h"
#include "elements/concepts/or.h"
#include "elements/concepts/projection.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/some.h"
#include "elements/concepts/subset.h"
#include "elements/concepts/top.h"
#include "elements/numericals/concept_distance.h"
#include "elements/numericals/count.h"
#include "elements/numericals/role_distance.h"
#include "elements/numericals/sum_concept_distance.h"
#include "elements/numericals/sum_role_distance.h"
#include "elements/roles/and.h"
#include "elements/roles/compose.h"
#include "elements/roles/diff.h"
#include "elements/roles/identity.h"
#include "elements/roles/inverse.h"
#include "elements/roles/not.h"
#include "elements/roles/or.h"
#include "elements/roles/primitive.h"
#include "elements/roles/restrict.h"
#include "elements/roles/top.h"
#include "elements/roles/transitive_closure.h"
#include "elements/roles/transitive_reflexive_closure.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>

// Runtime
// https://www.boost.org/doc/libs/1_82_0/libs/serialization/doc/serialization.html#export
BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Concept>, "dlplan::core::EmptyBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Role>, "dlplan::core::EmptyBoolean<dlplan::core::Role>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Concept>, "dlplan::core::InclusionBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Role>, "dlplan::core::InclusionBoolean<dlplan::core::Role>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::NullaryBoolean, "dlplan::core::NullaryBoolean")
BOOST_CLASS_EXPORT_GUID(dlplan::core::AllConcept, "dlplan::core::AllConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::BotConcept, "dlplan::core::BotConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::AndConcept, "dlplan::core::AndConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::DiffConcept, "dlplan::core::DiffConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::EqualConcept, "dlplan::core::EqualConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::NotConcept, "dlplan::core::NotConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::OneOfConcept, "dlplan::core::OneOfConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::OrConcept, "dlplan::core::OrConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::ProjectionConcept, "dlplan::core::ProjectionConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::SomeConcept, "dlplan::core::SomeConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::SubsetConcept, "dlplan::core::SubsetConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::TopConcept, "dlplan::core::TopConcept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::ConceptDistanceNumerical, "dlplan::core::ConceptDistanceNumerical")
BOOST_CLASS_EXPORT_GUID(dlplan::core::CountNumerical<dlplan::core::Concept>, "CountNumerical<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::CountNumerical<dlplan::core::Role>, "CountNumerical<dlplan::core::Role>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::RoleDistanceNumerical, "dlplan::core::RoleDistanceNumerical")
BOOST_CLASS_EXPORT_GUID(dlplan::core::SumConceptDistanceNumerical, "dlplan::core::SumConceptDistanceNumerical")
BOOST_CLASS_EXPORT_GUID(dlplan::core::SumRoleDistanceNumerical, "dlplan::core::SumRoleDistanceNumerical")
BOOST_CLASS_EXPORT_GUID(dlplan::core::AndRole, "dlplan::core::AndRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::ComposeRole, "dlplan::core::ComposeRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::DiffRole, "dlplan::core::DiffRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::IdentityRole, "dlplan::core::IdentityRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::InverseRole, "dlplan::core::InverseRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::NotRole, "dlplan::core::NotRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::OrRole, "dlplan::core::OrRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveRole, "dlplan::core::PrimitiveRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::RestrictRole, "dlplan::core::RestrictRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::TopRole, "dlplan::core::TopRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::TransitiveClosureRole, "dlplan::core::TransitiveClosureRole")
BOOST_CLASS_EXPORT_GUID(dlplan::core::TransitiveReflexiveClosureRole, "dlplan::core::TransitiveReflexiveClosureRole")

namespace boost::serialization {
template<typename Archive>
inline void serialize(Archive& /* ar */ , dlplan::core::Constant& /* t */, const unsigned int /* version */) {
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Constant* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Constant* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::ConstantIndex index;
    ar >> name;
    ar >> index;
    ::new(t)dlplan::core::Constant(name, index);
}

template<typename Archive>
inline void serialize(Archive& /* ar */ , dlplan::core::Predicate& /* t */, const unsigned int /* version */) {
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Predicate* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
    ar << t->m_arity;
    ar << t->m_is_static;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Predicate* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::PredicateIndex index;
    int arity;
    bool is_static;
    ar >> name;
    ar >> index;
    ar >> arity;
    ar >> is_static;
    ::new(t)dlplan::core::Predicate(name, index, arity, is_static);
}

template<typename Archive>
inline void serialize(Archive& /* ar */ , dlplan::core::Object& /* t */, const unsigned int /* version */) {
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Object* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Object* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::ObjectIndex index;
    ar >> name;
    ar >> index;
    ::new(t)dlplan::core::Object(name, index);
}

template<typename Archive>
inline void serialize(Archive& /* ar */ , dlplan::core::Atom& /* t */, const unsigned int /* version */) {
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Atom* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
    ar << t->m_predicate_index;
    ar << t->m_object_indices;
    ar << t->m_is_static;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Atom* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::AtomIndex index;
    dlplan::core::PredicateIndex predicate_index;
    dlplan::core::ObjectIndices object_indices;
    bool is_static;
    ar >> name;
    ar >> index;
    ar >> predicate_index;
    ar >> object_indices;
    ar >> is_static;
    ::new(t)dlplan::core::Atom(name, index, predicate_index, object_indices, is_static);
}


template<typename Archive>
void serialize( Archive& ar, dlplan::core::VocabularyInfo& t, const unsigned int /* version */ )
{
    ar & t.m_constants;
    ar & t.m_constant_name_to_index;
    ar & t.m_predicates;
    ar & t.m_predicate_name_to_index;
}

template<typename Archive>
inline void serialize(Archive& ar, dlplan::core::InstanceInfo& t, const unsigned int /* version */) {
    ar & t.m_objects;
    ar & t.m_object_name_to_index;
    ar & t.m_atoms;
    ar & t.m_atom_name_to_index;
    ar & t.m_static_atoms;
    ar & t.m_static_atom_name_to_index;
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::InstanceInfo* t, const unsigned int /* version */ ){
    ar & t->m_vocabulary_info;
    ar & t->m_index;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::InstanceInfo* t, const unsigned int /* version */ ){
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    dlplan::core::InstanceIndex index;
    ar & vocabulary;
    ar & index;
    ::new(t)dlplan::core::InstanceInfo(vocabulary);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::State& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::State* t, const unsigned int /* version */ ){
    ar << t->m_index;
    ar << t->m_instance_info;
    ar << t->m_atom_indices;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::State* t, const unsigned int /* version */ ){
    dlplan::core::InstanceIndex index;
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info;
    dlplan::core::AtomIndices atom_indices;
    ar >> index;
    ar >> instance_info;
    ar >> atom_indices;
    ::new(t)dlplan::core::State(instance_info, std::move(atom_indices), index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , std::pair<const int, dlplan::core::State>& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const std::pair<const int, dlplan::core::State>* t, const unsigned int /* version */ ){
    ar << t->first;
    ar << &t->second;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, std::pair<const int, dlplan::core::State>* t, const unsigned int /* version */ ){
    int first;
    dlplan::core::State* second;
    ar >> first;
    ar >> second;
    ::new(t)std::pair<int, dlplan::core::State>(first, *second);
    delete second;
}

template<typename Archive, typename T>
void serialize(Archive& /* ar */ , dlplan::core::EmptyBoolean<T>& /* t */, const unsigned int /* version */ )
{
}

template<class Archive, typename T>
void save_construct_data(Archive & ar, const dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive & ar, dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element;
    ar >> vocabulary;
    ar >> index;
    ar >> element;
    ::new(t)dlplan::core::EmptyBoolean<T>(vocabulary, index, element);
}

template<typename Archive, typename T>
void serialize(Archive& /* ar */, dlplan::core::InclusionBoolean<T>& /* t */, const unsigned int /* version */ )
{
}

template<class Archive, typename T>
void save_construct_data(Archive & ar, const dlplan::core::InclusionBoolean<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element_left;
    ar << t->m_element_right;
}

template<class Archive, typename T>
void load_construct_data(Archive & ar, dlplan::core::InclusionBoolean<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element_left;
    std::shared_ptr<const T> element_right;
    ar >> vocabulary;
    ar >> index;
    ar >> element_left;
    ar >> element_right;
    ::new(t)dlplan::core::InclusionBoolean<T>(vocabulary, index, element_left, element_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::NullaryBoolean& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ::new(t)dlplan::core::NullaryBoolean(vocabulary, index, *predicate);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::AllConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::AllConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::AllConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> concept;
    ::new(t)dlplan::core::AllConcept(vocabulary, index, role, concept);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::AndConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::AndConcept(vocabulary, index, concept_left, concept_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::BotConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::BotConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::BotConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::BotConcept(vocabulary, index);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::DiffConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::DiffConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::DiffConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::DiffConcept(vocabulary, index, concept_left, concept_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::EqualConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::EqualConcept(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::NotConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::NotConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::NotConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> vocabulary;
    ar >> index;
    ar >> concept;
    ::new(t)dlplan::core::NotConcept(vocabulary, index, concept);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::OneOfConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::OneOfConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_constant;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::OneOfConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Constant* constant;
    ar >> vocabulary;
    ar >> index;
    ar >> constant;
    ::new(t)dlplan::core::OneOfConcept(vocabulary, index, *constant);
    delete constant;
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::OrConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::OrConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::OrConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::OrConcept(vocabulary, index, concept_left, concept_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::PrimitiveConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ar >> pos;
    ::new(t)dlplan::core::PrimitiveConcept(vocabulary, index, *predicate, pos);
    delete predicate;
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::ProjectionConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> pos;
    ::new(t)dlplan::core::ProjectionConcept(vocabulary, index, role, pos);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::SomeConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::SomeConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::SomeConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> concept;
    ::new(t)dlplan::core::SomeConcept(vocabulary, index, role, concept);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::SubsetConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::SubsetConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::SubsetConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::SubsetConcept(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::TopConcept& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TopConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TopConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::TopConcept(vocabulary, index);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::ConceptDistanceNumerical& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::ConceptDistanceNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_from;
    ar << t->m_role;
    ar << t->m_concept_to;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::ConceptDistanceNumerical* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_from;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Concept> concept_to;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_from;
    ar >> role;
    ar >> concept_to;
    ::new(t)dlplan::core::ConceptDistanceNumerical(vocabulary, index, concept_from, role, concept_to);
}

template<typename Archive, typename T>
void serialize(Archive& /* ar */, dlplan::core::CountNumerical<T>& /* t */, const unsigned int /* version */ )
{
}

template<class Archive, typename T>
void save_construct_data(Archive & ar, const dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive & ar, dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element;
    ar >> vocabulary;
    ar >> index;
    ar >> element;
    ::new(t)dlplan::core::CountNumerical<T>(vocabulary, index, element);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::RoleDistanceNumerical& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::RoleDistanceNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_from;
    ar << t->m_role;
    ar << t->m_role_to;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::RoleDistanceNumerical* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_from;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Role> role_to;
    ar >> vocabulary;
    ar >> index;
    ar >> role_from;
    ar >> role;
    ar >> role_to;
    ::new(t)dlplan::core::RoleDistanceNumerical(vocabulary, index, role_from, role, role_to);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::SumConceptDistanceNumerical& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::SumConceptDistanceNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_from;
    ar << t->m_role;
    ar << t->m_concept_to;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::SumConceptDistanceNumerical* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_from;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Concept> concept_to;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_from;
    ar >> role;
    ar >> concept_to;
    ::new(t)dlplan::core::SumConceptDistanceNumerical(vocabulary, index, concept_from, role, concept_to);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::SumRoleDistanceNumerical& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::SumRoleDistanceNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_from;
    ar << t->m_role;
    ar << t->m_role_to;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::SumRoleDistanceNumerical* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_from;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Role> role_to;
    ar >> vocabulary;
    ar >> index;
    ar >> role_from;
    ar >> role;
    ar >> role_to;
    ::new(t)dlplan::core::SumRoleDistanceNumerical(vocabulary, index, role_from, role, role_to);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::AndRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::AndRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::AndRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::AndRole(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::ComposeRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::ComposeRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::ComposeRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::ComposeRole(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::DiffRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::DiffRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::DiffRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::DiffRole(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::IdentityRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::IdentityRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::IdentityRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> vocabulary;
    ar >> index;
    ar >> concept;
    ::new(t)dlplan::core::IdentityRole(vocabulary, index, concept);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::InverseRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::InverseRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::InverseRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::InverseRole(vocabulary, index, role);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::NotRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::NotRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::NotRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::NotRole(vocabulary, index, role);
}


template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::OrRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::OrRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::OrRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::OrRole(vocabulary, index, role_left, role_right);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::PrimitiveRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::PrimitiveRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
    ar << t->m_pos_1;
    ar << t->m_pos_2;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::PrimitiveRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    int pos_1;
    int pos_2;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ar >> pos_1;
    ar >> pos_2;
    ::new(t)dlplan::core::PrimitiveRole(vocabulary, index, *predicate, pos_1, pos_2);
    delete predicate;
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::RestrictRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::RestrictRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::RestrictRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> concept;
    ::new(t)dlplan::core::RestrictRole(vocabulary, index, role, concept);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::TopRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::TopRole(vocabulary, index);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::TransitiveClosureRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TransitiveClosureRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TransitiveClosureRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::TransitiveClosureRole(vocabulary, index, role);
}

template<typename Archive>
void serialize(Archive& /* ar */, dlplan::core::TransitiveReflexiveClosureRole& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TransitiveReflexiveClosureRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TransitiveReflexiveClosureRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::TransitiveReflexiveClosureRole(vocabulary, index, role);
}


template<typename Archive>
void serialize(Archive& ar, dlplan::core::SyntacticElementFactory& factory, const unsigned int /* version */ )
{
    ar & factory.m_pImpl;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::SyntacticElementFactoryImpl& factory, const unsigned int /* version */ )
{
    ar & factory.m_vocabulary_info;
    ar & factory.m_caches;
}


template<typename Archive>
void serialize(Archive& ar, dlplan::utils::pimpl<dlplan::core::SyntacticElementFactoryImpl>& pimpl, const unsigned int /* version */ )
{
    ar & pimpl.m;
}


template<typename Archive>
void serialize(Archive& ar, dlplan::core::Caches& caches, const unsigned int /* version */ )
{
    ar & caches.m_boolean_cache;
    ar & caches.m_numerical_cache;
    ar & caches.m_concept_cache;
    ar & caches.m_role_cache;
}

template<typename Archive, typename KEY, typename VALUE>
void serialize(Archive& ar, dlplan::utils::ReferenceCountedObjectCache<KEY, VALUE>& cache, const unsigned int /* version */ )
{
    ar & cache.m_cache;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::state_space::StateSpace& state_space, const unsigned int /* version */ )
{
    ar & state_space.m_instance_info;
    ar & state_space.m_states;
    ar & state_space.m_initial_state_index;
    ar & state_space.m_goal_state_indices;
    ar & state_space.m_forward_successor_state_indices;
    ar & state_space.m_backward_successor_state_indices;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::novelty::NoveltyBase& novelty_base, const unsigned int /* version */ )
{
    ar & novelty_base.m_factors;
    ar & novelty_base.m_num_atoms;
    ar & novelty_base.m_arity;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::novelty::TupleNode& tuple_node, const unsigned int /* version */ )
{
    ar & tuple_node.m_index;
    ar & tuple_node.m_tuple_index;
    ar & tuple_node.m_state_indices;
    ar & tuple_node.m_predecessors;
    ar & tuple_node.m_successors;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::novelty::TupleGraph& tuple_graph, const unsigned int /* version */ )
{
    ar & tuple_graph.m_novelty_base;
    ar & tuple_graph.m_state_space;
    ar & tuple_graph.m_root_state_index;
    ar & tuple_graph.m_nodes;
    ar & tuple_graph.m_node_indices_by_distance;
    ar & tuple_graph.m_state_indices_by_distance;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::BaseCondition& condition, const unsigned int /* version */ )
{
    ar & condition.m_index;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::BaseEffect& effect, const unsigned int /* version */ )
{
    ar & effect.m_index;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::Rule& rule, const unsigned int /* version */ )
{
    ar & rule.m_index;
    ar & rule.m_conditions;
    ar & rule.m_effects;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::Policy& policy, const unsigned int /* version */ )
{
    ar & policy.m_index;
    ar & policy.m_booleans;
    ar & policy.m_numericals;
    ar & policy.m_rules;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyBuilder& builder, const unsigned int /* version */ )
{
    ar & builder.m_pImpl;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::serialization::Data& data, const unsigned int /* version */ )
{
    ar & data.vocabulary_infos;
    ar & data.instance_infos;
    ar & data.syntatic_element_factories;
    ar & data.state_spaces;
    ar & data.tuple_graphs;
}

}


namespace dlplan::serialization {

void serialize(const Data& data, std::ostream& out_buffer) {
    boost::archive::text_oarchive oa(out_buffer);
    oa << data;
}

Data deserialize(std::istream& buffer) {
    boost::archive::text_iarchive ia(buffer);
    Data data;
    ia >> data;
    return data;
}

}
