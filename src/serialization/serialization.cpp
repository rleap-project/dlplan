#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/utility.hpp>

#include "../core/element_factory.h"
#include "../core/elements/booleans/empty.h"
#include "../core/elements/booleans/inclusion.h"
#include "../core/elements/booleans/nullary.h"
#include "../core/elements/concepts/all.h"
#include "../core/elements/concepts/bot.h"
#include "../core/elements/concepts/and.h"
#include "../core/elements/concepts/diff.h"
#include "../core/elements/concepts/equal.h"
#include "../core/elements/concepts/not.h"
#include "../core/elements/concepts/one_of.h"
#include "../core/elements/concepts/or.h"
#include "../core/elements/concepts/projection.h"
#include "../core/elements/concepts/primitive.h"
#include "../core/elements/concepts/some.h"
#include "../core/elements/concepts/subset.h"
#include "../core/elements/concepts/top.h"
#include "../core/elements/numericals/concept_distance.h"
#include "../core/elements/numericals/count.h"
#include "../core/elements/numericals/role_distance.h"
#include "../core/elements/numericals/sum_concept_distance.h"
#include "../core/elements/numericals/sum_role_distance.h"
#include "../core/elements/roles/and.h"
#include "../core/elements/roles/compose.h"
#include "../core/elements/roles/diff.h"
#include "../core/elements/roles/identity.h"
#include "../core/elements/roles/inverse.h"
#include "../core/elements/roles/not.h"
#include "../core/elements/roles/or.h"
#include "../core/elements/roles/primitive.h"
#include "../core/elements/roles/restrict.h"
#include "../core/elements/roles/top.h"
#include "../core/elements/roles/transitive_closure.h"
#include "../core/elements/roles/transitive_reflexive_closure.h"

#include "../../include/dlplan/serialization.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/novelty.h"
#include "../../include/dlplan/policy.h"
#include "../../include/dlplan/state_space.h"

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
BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")
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
