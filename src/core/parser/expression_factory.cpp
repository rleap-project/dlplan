#include "expression_factory.h"

#include "expressions/booleans/empty.h"
#include "expressions/booleans/inclusion.h"
#include "expressions/booleans/nullary.h"
#include "expressions/concepts/all.h"
#include "expressions/concepts/and.h"
#include "expressions/concepts/bot.h"
#include "expressions/concepts/diff.h"
#include "expressions/concepts/equal.h"
#include "expressions/concepts/not.h"
#include "expressions/concepts/one_of.h"
#include "expressions/concepts/or.h"
#include "expressions/concepts/projection.h"
#include "expressions/concepts/primitive.h"
#include "expressions/concepts/some.h"
#include "expressions/concepts/subset.h"
#include "expressions/concepts/top.h"
#include "expressions/numericals/concept_distance.h"
#include "expressions/numericals/count.h"
#include "expressions/numericals/role_distance.h"
#include "expressions/numericals/sum_concept_distance.h"
#include "expressions/numericals/sum_role_distance.h"
#include "expressions/roles/and.h"
#include "expressions/roles/compose.h"
#include "expressions/roles/diff.h"
#include "expressions/roles/identity.h"
#include "expressions/roles/inverse.h"
#include "expressions/roles/not.h"
#include "expressions/roles/or.h"
#include "expressions/roles/primitive.h"
#include "expressions/roles/restrict.h"
#include "expressions/roles/top.h"
#include "expressions/roles/transitive_closure.h"
#include "expressions/roles/transitive_reflexive_closure.h"

#include "../elements/booleans/empty.h"
#include "../elements/booleans/inclusion.h"
#include "../elements/booleans/nullary.h"
#include "../elements/concepts/all.h"
#include "../elements/concepts/and.h"
#include "../elements/concepts/bot.h"
#include "../elements/concepts/diff.h"
#include "../elements/concepts/equal.h"
#include "../elements/concepts/not.h"
#include "../elements/concepts/one_of.h"
#include "../elements/concepts/or.h"
#include "../elements/concepts/primitive.h"
#include "../elements/concepts/projection.h"
#include "../elements/concepts/some.h"
#include "../elements/concepts/subset.h"
#include "../elements/concepts/top.h"
#include "../elements/numericals/concept_distance.h"
#include "../elements/numericals/count.h"
#include "../elements/numericals/role_distance.h"
#include "../elements/numericals/sum_concept_distance.h"
#include "../elements/numericals/sum_role_distance.h"
#include "../elements/roles/and.h"
#include "../elements/roles/compose.h"
#include "../elements/roles/diff.h"
#include "../elements/roles/identity.h"
#include "../elements/roles/inverse.h"
#include "../elements/roles/not.h"
#include "../elements/roles/or.h"
#include "../elements/roles/primitive.h"
#include "../elements/roles/restrict.h"
#include "../elements/roles/top.h"
#include "../elements/roles/transitive_closure.h"
#include "../elements/roles/transitive_reflexive_closure.h"

#include <algorithm>
#include <stdexcept>


namespace dlplan::core::parser {

Expression_Ptr ExpressionFactory::make_expression(const std::string& name, std::vector<Expression_Ptr> &&children) {
    if (name == element::EmptyBoolean<int>::get_name()) {
        return std::make_unique<EmptyBoolean>(EmptyBoolean(name, std::move(children)));
    } else if (name == element::InclusionBoolean<int>::get_name()) {
        return std::make_unique<InclusionBoolean>(InclusionBoolean(name, std::move(children)));
    } else if (name == element::NullaryBoolean::get_name()) {
        return std::make_unique<NullaryBoolean>(NullaryBoolean(name, std::move(children)));
    } else if (name == element::AllConcept::get_name()) {
        return std::make_unique<AllConcept>(AllConcept(name, std::move(children)));
    } else if (name == element::AndConcept::get_name()) {
        return std::make_unique<AndConcept>(AndConcept(name, std::move(children)));
    } else if (name == element::BotConcept::get_name()) {
        return std::make_unique<BotConcept>(BotConcept(name, std::move(children)));
    } else if (name == element::DiffConcept::get_name()) {
        return std::make_unique<DiffConcept>(DiffConcept(name, std::move(children)));
    } else if (name == element::EqualConcept::get_name()) {
        return std::make_unique<EqualConcept>(EqualConcept(name, std::move(children)));
    } else if (name == element::NotConcept::get_name()) {
        return std::make_unique<NotConcept>(NotConcept(name, std::move(children)));
    } else if (name == element::OneOfConcept::get_name()) {
        return std::make_unique<OneOfConcept>(OneOfConcept(name, std::move(children)));
    } else if (name == element::OrConcept::get_name()) {
        return std::make_unique<OrConcept>(OrConcept(name, std::move(children)));
    } else if (name == element::PrimitiveConcept::get_name()) {
        return std::make_unique<PrimitiveConcept>(PrimitiveConcept(name, std::move(children)));
    } else if (name == element::ProjectionConcept::get_name()) {
        return std::make_unique<ProjectionConcept>(ProjectionConcept(name, std::move(children)));
    } else if (name == element::SomeConcept::get_name()) {
        return std::make_unique<SomeConcept>(SomeConcept(name, std::move(children)));
    } else if (name == element::SubsetConcept::get_name()) {
        return std::make_unique<SubsetConcept>(SubsetConcept(name, std::move(children)));
    } else if (name == element::TopConcept::get_name()) {
        return std::make_unique<TopConcept>(TopConcept(name, std::move(children)));
    } else if (name == element::ConceptDistanceNumerical::get_name()) {
        return std::make_unique<ConceptDistanceNumerical>(ConceptDistanceNumerical(name, std::move(children)));
    } else if (name == element::CountNumerical<int>::get_name()) {
        return std::make_unique<CountNumerical>(CountNumerical(name, std::move(children)));
    } else if (name == element::RoleDistanceNumerical::get_name()) {
        return std::make_unique<RoleDistanceNumerical>(RoleDistanceNumerical(name, std::move(children)));
    } else if (name == element::SumConceptDistanceNumerical::get_name()) {
        return std::make_unique<SumConceptDistanceNumerical>(SumConceptDistanceNumerical(name, std::move(children)));
    } else if (name == element::SumRoleDistanceNumerical::get_name()) {
        return std::make_unique<SumRoleDistanceNumerical>(SumRoleDistanceNumerical(name, std::move(children)));
    } else if (name == element::AndRole::get_name()) {
        return std::make_unique<AndRole>(AndRole(name, std::move(children)));
    } else if (name == element::ComposeRole::get_name()) {
        return std::make_unique<ComposeRole>(ComposeRole(name, std::move(children)));
    } else if (name == element::DiffRole::get_name()) {
        return std::make_unique<DiffRole>(DiffRole(name, std::move(children)));
    } else if (name == element::IdentityRole::get_name()) {
        return std::make_unique<IdentityRole>(IdentityRole(name, std::move(children)));
    } else if (name == element::InverseRole::get_name()) {
        return std::make_unique<InverseRole>(InverseRole(name, std::move(children)));
    } else if (name == element::NotRole::get_name()) {
        return std::make_unique<NotRole>(NotRole(name, std::move(children)));
    } else if (name == element::OrRole::get_name()) {
        return std::make_unique<OrRole>(OrRole(name, std::move(children)));
    } else if (name == element::PrimitiveRole::get_name()) {
        return std::make_unique<PrimitiveRole>(PrimitiveRole(name, std::move(children)));
    } else if (name == element::RestrictRole::get_name()) {
        return std::make_unique<RestrictRole>(RestrictRole(name, std::move(children)));
    } else if (name == element::TopRole::get_name()) {
        return std::make_unique<TopRole>(TopRole(name, std::move(children)));
    } else if (name == element::TransitiveClosureRole::get_name()) {
        return std::make_unique<TransitiveClosureRole>(TransitiveClosureRole(name, std::move(children)));
    } else if (name == element::TransitiveReflexiveClosureRole::get_name()) {
        return std::make_unique<TransitiveReflexiveClosureRole>(TransitiveReflexiveClosureRole(name, std::move(children)));
    }
    return std::make_unique<Expression>(Expression(name, std::move(children)));
}


}
