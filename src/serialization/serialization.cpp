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

// Runtime
// https://www.boost.org/doc/libs/1_82_0/libs/serialization/doc/serialization.html#export
BOOST_CLASS_EXPORT_GUID(dlplan::core::Concept, "dlplan::core::Concept")
BOOST_CLASS_EXPORT_GUID(dlplan::core::Role, "dlplan::core::Role")
BOOST_CLASS_EXPORT_GUID(dlplan::core::Boolean, "dlplan::core::Boolean")
BOOST_CLASS_EXPORT_GUID(dlplan::core::Numerical, "dlplan::core::Numerical")
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
void serialize( Archive& ar, dlplan::core::Predicate& predicate, const unsigned int /* version */ )
{
    ar & predicate.m_index;
    ar & predicate.m_name;
    ar & predicate.m_arity;
    ar & predicate.m_is_static;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Constant& constant, const unsigned int /* version */ )
{
    ar & constant.m_index;
    ar & constant.m_name;
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Constant* constant, const unsigned int /* version */ ){
    ar << constant->m_index;
    ar << constant->m_name;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Constant* constant, const unsigned int /* version */ ){
    dlplan::core::ConstantIndex m_index;
    std::string m_name;
    ar >> m_index;
    ar >> m_name;
    ::new(constant)dlplan::core::Constant(m_name, m_index);
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Object& object, const unsigned int /* version */ )
{
    ar & object.m_index;
    ar & object.m_name;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Atom& atom, const unsigned int /* version */ )
{
    ar & atom.m_index;
    ar & atom.m_name;
    ar & atom.m_is_static;
    ar & atom.m_predicate_index;
    ar & atom.m_object_indices;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::VocabularyInfo& vocabulary_info, const unsigned int /* version */ )
{
    ar & vocabulary_info.m_constants;
    ar & vocabulary_info.m_constant_name_to_index;
    ar & vocabulary_info.m_predicates;
    ar & vocabulary_info.m_predicate_name_to_index;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::InstanceInfo& instance_info, const unsigned int /* version */ )
{
    ar & instance_info.m_vocabulary_info;
    ar & instance_info.m_index;
    ar & instance_info.m_objects;
    ar & instance_info.m_object_name_to_index;
    ar & instance_info.m_atoms;
    ar & instance_info.m_atom_name_to_index;
    ar & instance_info.m_static_atoms;
    ar & instance_info.m_static_atom_name_to_index;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::State& state, const unsigned int /* version */ )
{
    ar & state.m_index;
    ar & state.m_instance_info;
    ar & state.m_atom_indices;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::BaseElement& element, const unsigned int /* version */ ) {
    ar & element.m_index;
    ar & element.m_vocabulary_info;
    ar & element.m_is_static;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::Concept& concept, const unsigned int /* version */ ) {
    ar & boost::serialization::base_object<dlplan::core::BaseElement>(concept);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::Role& role, const unsigned int /* version */ ) {
    ar & boost::serialization::base_object<dlplan::core::BaseElement>(role);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::Boolean& boolean, const unsigned int /* version */ ) {
    ar & boost::serialization::base_object<dlplan::core::BaseElement>(boolean);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::Numerical& numerical, const unsigned int /* version */ ) {
    ar & boost::serialization::base_object<dlplan::core::BaseElement>(numerical);
}

template<typename Archive, typename T>
void serialize(Archive& ar, dlplan::core::EmptyBoolean<T>& boolean, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Boolean>(boolean);
    ar & boolean.m_element;
}

template<typename Archive, typename T>
void serialize(Archive& ar, dlplan::core::InclusionBoolean<T>& boolean, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Boolean>(boolean);
    ar & boolean.m_element_left;
    ar & boolean.m_element_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::NullaryBoolean& boolean, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Boolean>(boolean);
    ar & boolean.m_predicate;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::AllConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_concept;
    ar & concept.m_role;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::AndConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_concept_left;
    ar & concept.m_concept_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::BotConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::DiffConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_concept_left;
    ar & concept.m_concept_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::EqualConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_role_left;
    ar & concept.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::NotConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_concept;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::OneOfConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_constant;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::OrConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_concept_left;
    ar & concept.m_concept_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::PrimitiveConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_predicate;
    ar & concept.m_pos;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::ProjectionConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_role;
    ar & concept.m_pos;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::SomeConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_role;
    ar & concept.m_concept;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::SubsetConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
    ar & concept.m_role_left;
    ar & concept.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::TopConcept& concept, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Concept>(concept);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::ConceptDistanceNumerical& numerical, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Numerical>(numerical);
    ar & numerical.m_concept_from;
    ar & numerical.m_role;
    ar & numerical.m_concept_to;
}

template<typename Archive, typename T>
void serialize(Archive& ar, dlplan::core::CountNumerical<T>& numerical, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Numerical>(numerical);
    ar & numerical.m_element;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::RoleDistanceNumerical& numerical, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Numerical>(numerical);
    ar & numerical.m_role_from;
    ar & numerical.m_role;
    ar & numerical.m_role_to;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::SumConceptDistanceNumerical& numerical, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Numerical>(numerical);
    ar & numerical.m_concept_from;
    ar & numerical.m_role;
    ar & numerical.m_concept_to;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::SumRoleDistanceNumerical& numerical, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Numerical>(numerical);
    ar & numerical.m_role_from;
    ar & numerical.m_role;
    ar & numerical.m_role_to;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::AndRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role_left;
    ar & role.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::ComposeRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role_left;
    ar & role.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::DiffRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role_left;
    ar & role.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::IdentityRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_concept;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::InverseRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role;;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::NotRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::OrRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role_left;
    ar & role.m_role_right;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::PrimitiveRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_predicate;
    ar & role.m_pos_1;
    ar & role.m_pos_2;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::RestrictRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role;
    ar & role.m_concept;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::TopRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::TransitiveClosureRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::core::TransitiveReflexiveClosureRole& role, const unsigned int /* version */ )
{
    ar & boost::serialization::base_object<dlplan::core::Role>(role);
    ar & role.m_role;
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
    ar & cache.m_index_counter;
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
