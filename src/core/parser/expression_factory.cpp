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

std::unique_ptr<Expression> ExpressionFactory::make_expression(const std::string& name, std::vector<std::unique_ptr<Expression>> &&children) {
    if (name == dlplan::core::EmptyBoolean<int>::get_name()) {
        return std::make_unique<EmptyBoolean>(name, std::move(children));
    } else if (name == dlplan::core::InclusionBoolean<int>::get_name()) {
        return std::make_unique<InclusionBoolean>(name, std::move(children));
    } else if (name == dlplan::core::NullaryBoolean::get_name()) {
        return std::make_unique<NullaryBoolean>(name, std::move(children));
    } else if (name == dlplan::core::AllConcept::get_name()) {
        return std::make_unique<AllConcept>(name, std::move(children));
    } else if (name == dlplan::core::AndConcept::get_name()) {
        return std::make_unique<AndConcept>(name, std::move(children));
    } else if (name == dlplan::core::BotConcept::get_name()) {
        return std::make_unique<BotConcept>(name, std::move(children));
    } else if (name == dlplan::core::DiffConcept::get_name()) {
        return std::make_unique<DiffConcept>(name, std::move(children));
    } else if (name == dlplan::core::EqualConcept::get_name()) {
        return std::make_unique<EqualConcept>(name, std::move(children));
    } else if (name == dlplan::core::NotConcept::get_name()) {
        return std::make_unique<NotConcept>(name, std::move(children));
    } else if (name == dlplan::core::OneOfConcept::get_name()) {
        return std::make_unique<OneOfConcept>(name, std::move(children));
    } else if (name == dlplan::core::OrConcept::get_name()) {
        return std::make_unique<OrConcept>(name, std::move(children));
    } else if (name == dlplan::core::PrimitiveConcept::get_name()) {
        return std::make_unique<PrimitiveConcept>(name, std::move(children));
    } else if (name == dlplan::core::ProjectionConcept::get_name()) {
        return std::make_unique<ProjectionConcept>(name, std::move(children));
    } else if (name == dlplan::core::SomeConcept::get_name()) {
        return std::make_unique<SomeConcept>(name, std::move(children));
    } else if (name == dlplan::core::SubsetConcept::get_name()) {
        return std::make_unique<SubsetConcept>(name, std::move(children));
    } else if (name == dlplan::core::TopConcept::get_name()) {
        return std::make_unique<TopConcept>(name, std::move(children));
    } else if (name == dlplan::core::ConceptDistanceNumerical::get_name()) {
        return std::make_unique<ConceptDistanceNumerical>(name, std::move(children));
    } else if (name == dlplan::core::CountNumerical<int>::get_name()) {
        return std::make_unique<CountNumerical>(name, std::move(children));
    } else if (name == dlplan::core::RoleDistanceNumerical::get_name()) {
        return std::make_unique<RoleDistanceNumerical>(name, std::move(children));
    } else if (name == dlplan::core::SumConceptDistanceNumerical::get_name()) {
        return std::make_unique<SumConceptDistanceNumerical>(name, std::move(children));
    } else if (name == dlplan::core::SumRoleDistanceNumerical::get_name()) {
        return std::make_unique<SumRoleDistanceNumerical>(name, std::move(children));
    } else if (name == dlplan::core::AndRole::get_name()) {
        return std::make_unique<AndRole>(name, std::move(children));
    } else if (name == dlplan::core::ComposeRole::get_name()) {
        return std::make_unique<ComposeRole>(name, std::move(children));
    } else if (name == dlplan::core::DiffRole::get_name()) {
        return std::make_unique<DiffRole>(name, std::move(children));
    } else if (name == dlplan::core::IdentityRole::get_name()) {
        return std::make_unique<IdentityRole>(name, std::move(children));
    } else if (name == dlplan::core::InverseRole::get_name()) {
        return std::make_unique<InverseRole>(name, std::move(children));
    } else if (name == dlplan::core::NotRole::get_name()) {
        return std::make_unique<NotRole>(name, std::move(children));
    } else if (name == dlplan::core::OrRole::get_name()) {
        return std::make_unique<OrRole>(name, std::move(children));
    } else if (name == dlplan::core::PrimitiveRole::get_name()) {
        return std::make_unique<PrimitiveRole>(name, std::move(children));
    } else if (name == dlplan::core::RestrictRole::get_name()) {
        return std::make_unique<RestrictRole>(name, std::move(children));
    } else if (name == dlplan::core::TopRole::get_name()) {
        return std::make_unique<TopRole>(name, std::move(children));
    } else if (name == dlplan::core::TransitiveClosureRole::get_name()) {
        return std::make_unique<TransitiveClosureRole>(name, std::move(children));
    } else if (name == dlplan::core::TransitiveReflexiveClosureRole::get_name()) {
        return std::make_unique<TransitiveReflexiveClosureRole>(name, std::move(children));
    }
    return std::make_unique<Expression>(name, std::move(children));
}


}
