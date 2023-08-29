#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>

#include "../../include/dlplan/serialization.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/novelty.h"
#include "../../include/dlplan/policy.h"
#include "../../include/dlplan/state_space.h"

#include "../policy/condition.h"
#include "../policy/effect.h"
#include "../policy/policy_builder.h"

// For derived classes that are defined in header-only fashion, we can export them here.
// For derived classes that are defined in source files, we need to move the serialization code there.
//BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Concept>, "dlplan::core::EmptyBoolean<dlplan::core::Concept>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Role>, "dlplan::core::EmptyBoolean<dlplan::core::Role>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Concept>, "dlplan::core::InclusionBoolean<dlplan::core::Concept>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Role>, "dlplan::core::InclusionBoolean<dlplan::core::Role>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::NullaryBoolean, "dlplan::core::NullaryBoolean")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::AllConcept, "dlplan::core::AllConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::BotConcept, "dlplan::core::BotConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::AndConcept, "dlplan::core::AndConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::DiffConcept, "dlplan::core::DiffConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::EqualConcept, "dlplan::core::EqualConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::NotConcept, "dlplan::core::NotConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::OneOfConcept, "dlplan::core::OneOfConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::OrConcept, "dlplan::core::OrConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::ProjectionConcept, "dlplan::core::ProjectionConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::SomeConcept, "dlplan::core::SomeConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::SubsetConcept, "dlplan::core::SubsetConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::TopConcept, "dlplan::core::TopConcept")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::ConceptDistanceNumerical, "dlplan::core::ConceptDistanceNumerical")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::CountNumerical<dlplan::core::Concept>, "CountNumerical<dlplan::core::Concept>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::CountNumerical<dlplan::core::Role>, "CountNumerical<dlplan::core::Role>")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::RoleDistanceNumerical, "dlplan::core::RoleDistanceNumerical")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::SumConceptDistanceNumerical, "dlplan::core::SumConceptDistanceNumerical")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::SumRoleDistanceNumerical, "dlplan::core::SumRoleDistanceNumerical")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::AndRole, "dlplan::core::AndRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::ComposeRole, "dlplan::core::ComposeRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::DiffRole, "dlplan::core::DiffRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::IdentityRole, "dlplan::core::IdentityRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::InverseRole, "dlplan::core::InverseRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::NotRole, "dlplan::core::NotRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::OrRole, "dlplan::core::OrRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveRole, "dlplan::core::PrimitiveRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::RestrictRole, "dlplan::core::RestrictRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::TopRole, "dlplan::core::TopRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::TransitiveClosureRole, "dlplan::core::TransitiveClosureRole")
//BOOST_CLASS_EXPORT_GUID(dlplan::core::TransitiveReflexiveClosureRole, "dlplan::core::TransitiveReflexiveClosureRole")


namespace boost::serialization {


template<typename Archive>
void serialize( Archive& ar, dlplan::state_space::StateSpace& t, const unsigned int /* version */ )
{
    ar & t.m_instance_info;
    ar & t.m_states;
    ar & t.m_initial_state_index;
    ar & t.m_goal_state_indices;
    ar & t.m_forward_successor_state_indices;
    ar & t.m_backward_successor_state_indices;
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::novelty::NoveltyBase& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::novelty::NoveltyBase* t, const unsigned int /* version */ )
{
    ar << t->m_factors;
    ar << t->m_num_atoms;
    ar << t->m_arity;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::novelty::NoveltyBase* t, const unsigned int /* version */ )
{
    std::vector<int> factors;
    int num_atoms;
    int arity;
    ar >> factors;
    ar >> num_atoms;
    ar >> arity;
    ::new(t)dlplan::novelty::NoveltyBase(std::move(factors), num_atoms, arity);
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::novelty::TupleNode& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::novelty::TupleNode* t, const unsigned int /* version */ )
{
    ar << t->m_index;
    ar << t->m_tuple_index;
    ar << t->m_state_indices;
    ar << t->m_predecessors;
    ar << t->m_successors;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::novelty::TupleNode* t, const unsigned int /* version */ )
{
    dlplan::novelty::TupleNodeIndex index;
    dlplan::novelty::TupleIndex tuple_index;
    dlplan::state_space::StateIndices state_indices;
    dlplan::novelty::TupleNodeIndices predecessors;
    dlplan::novelty::TupleNodeIndices successors;
    ar >> index;
    ar >> tuple_index;
    ar >> state_indices;
    ar >> predecessors;
    ar >> successors;
    ::new(t)dlplan::novelty::TupleNode(index, tuple_index, std::move(state_indices), std::move(predecessors), std::move(successors));
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::novelty::TupleGraph& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::novelty::TupleGraph* t, const unsigned int /* version */ )
{
    ar << t->m_novelty_base;
    ar << t->m_state_space;
    ar << t->m_root_state_index;
    ar << t->m_nodes;
    ar << t->m_node_indices_by_distance;
    ar << t->m_state_indices_by_distance;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::novelty::TupleGraph* t, const unsigned int /* version */ )
{
    std::shared_ptr<const dlplan::novelty::NoveltyBase> novelty_base;
    std::shared_ptr<const dlplan::state_space::StateSpace> state_space;
    state_space::StateIndex root_state_index;
    dlplan::novelty::TupleNodes nodes;
    std::vector<dlplan::novelty::TupleNodeIndices> node_indices_by_distance;
    std::vector<dlplan::state_space::StateIndices> state_indices_by_distance;
    ar >> novelty_base;
    ar >> state_space;
    ar >> root_state_index;
    ar >> nodes;
    ar >> node_indices_by_distance;
    ar >> state_indices_by_distance;
    ::new(t)dlplan::novelty::TupleGraph(novelty_base, state_space, root_state_index, std::move(nodes), std::move(node_indices_by_distance), std::move(state_indices_by_distance));
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BaseCondition& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BaseCondition* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BaseCondition* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BaseEffect& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BaseEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BaseEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::Rule& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::Rule* t, const unsigned int /* version */ )
{
    ar << t->m_index;
    ar << t->m_conditions;
    ar << t->m_effects;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::Rule* t, const unsigned int /* version */ )
{
    dlplan::policy::RuleIndex index;
    dlplan::policy::Conditions conditions;
    dlplan::policy::Effects effects;
    ar >> index;
    ar >> conditions;
    ar >> effects;
    ::new(t)dlplan::policy::Rule(std::move(conditions), std::move(effects), index);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::Policy& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::policy::Policy* t, const unsigned int /* version */ )
{
    ar << t->m_index;
    ar << t->m_booleans;
    ar << t->m_numericals;
    ar << t->m_rules;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::policy::Policy* t, const unsigned int /* version */ )
{
    dlplan::policy::PolicyIndex index;
    dlplan::policy::Booleans booleans;
    dlplan::policy::Numericals numericals;
    dlplan::policy::Rules rules;
    ar >> index;
    ar >> booleans;
    ar >> numericals;
    ar >> rules;
    ::new(t)dlplan::policy::Policy(std::move(booleans), std::move(numericals), std::move(rules), index);
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyBuilder& t, const unsigned int /* version */ )
{
    ar & t.m_pImpl;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyBuilderImpl& t, const unsigned int /* version */ )
{
    ar & t.m_caches;
}

template<typename Archive>
void serialize(Archive& ar, dlplan::policy::Caches& t, const unsigned int /* version */ )
{
    ar & t.m_condition_cache;
    ar & t.m_effect_cache;
    ar & t.m_rule_cache;
    ar & t.m_policy_cache;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::serialization::Data& t, const unsigned int /* version */ )
{
    ar & t.vocabulary_infos;
    ar & t.instance_infos;
    ar & t.syntatic_element_factories;
    ar & t.denotations_caches;
    ar & t.state_spaces;
    ar & t.tuple_graphs;
    ar & t.policies;
    ar & t.policy_builders;
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
