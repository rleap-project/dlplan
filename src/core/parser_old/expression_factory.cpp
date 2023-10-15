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

#include <algorithm>
#include <stdexcept>


namespace dlplan::core::parser {

std::unique_ptr<Expression> ExpressionFactory::make_expression(const std::string& name, std::vector<std::unique_ptr<Expression>> &&children) {
    if (name == EmptyBoolean::get_name()) {
        return std::make_unique<EmptyBoolean>(name, std::move(children));
    } else if (name == InclusionBoolean::get_name()) {
        return std::make_unique<InclusionBoolean>(name, std::move(children));
    } else if (name == NullaryBoolean::get_name()) {
        return std::make_unique<NullaryBoolean>(name, std::move(children));
    } else if (name == AllConcept::get_name()) {
        return std::make_unique<AllConcept>(name, std::move(children));
    } else if (name == AndConcept::get_name()) {
        return std::make_unique<AndConcept>(name, std::move(children));
    } else if (name == BotConcept::get_name()) {
        return std::make_unique<BotConcept>(name, std::move(children));
    } else if (name == DiffConcept::get_name()) {
        return std::make_unique<DiffConcept>(name, std::move(children));
    } else if (name == EqualConcept::get_name()) {
        return std::make_unique<EqualConcept>(name, std::move(children));
    } else if (name == NotConcept::get_name()) {
        return std::make_unique<NotConcept>(name, std::move(children));
    } else if (name == OneOfConcept::get_name()) {
        return std::make_unique<OneOfConcept>(name, std::move(children));
    } else if (name == OrConcept::get_name()) {
        return std::make_unique<OrConcept>(name, std::move(children));
    } else if (name == PrimitiveConcept::get_name()) {
        return std::make_unique<PrimitiveConcept>(name, std::move(children));
    } else if (name == ProjectionConcept::get_name()) {
        return std::make_unique<ProjectionConcept>(name, std::move(children));
    } else if (name == SomeConcept::get_name()) {
        return std::make_unique<SomeConcept>(name, std::move(children));
    } else if (name == SubsetConcept::get_name()) {
        return std::make_unique<SubsetConcept>(name, std::move(children));
    } else if (name == TopConcept::get_name()) {
        return std::make_unique<TopConcept>(name, std::move(children));
    } else if (name == ConceptDistanceNumerical::get_name()) {
        return std::make_unique<ConceptDistanceNumerical>(name, std::move(children));
    } else if (name == CountNumerical::get_name()) {
        return std::make_unique<CountNumerical>(name, std::move(children));
    } else if (name == RoleDistanceNumerical::get_name()) {
        return std::make_unique<RoleDistanceNumerical>(name, std::move(children));
    } else if (name == SumConceptDistanceNumerical::get_name()) {
        return std::make_unique<SumConceptDistanceNumerical>(name, std::move(children));
    } else if (name == SumRoleDistanceNumerical::get_name()) {
        return std::make_unique<SumRoleDistanceNumerical>(name, std::move(children));
    } else if (name == AndRole::get_name()) {
        return std::make_unique<AndRole>(name, std::move(children));
    } else if (name == ComposeRole::get_name()) {
        return std::make_unique<ComposeRole>(name, std::move(children));
    } else if (name == DiffRole::get_name()) {
        return std::make_unique<DiffRole>(name, std::move(children));
    } else if (name == IdentityRole::get_name()) {
        return std::make_unique<IdentityRole>(name, std::move(children));
    } else if (name == InverseRole::get_name()) {
        return std::make_unique<InverseRole>(name, std::move(children));
    } else if (name == NotRole::get_name()) {
        return std::make_unique<NotRole>(name, std::move(children));
    } else if (name == OrRole::get_name()) {
        return std::make_unique<OrRole>(name, std::move(children));
    } else if (name == PrimitiveRole::get_name()) {
        return std::make_unique<PrimitiveRole>(name, std::move(children));
    } else if (name == RestrictRole::get_name()) {
        return std::make_unique<RestrictRole>(name, std::move(children));
    } else if (name == TopRole::get_name()) {
        return std::make_unique<TopRole>(name, std::move(children));
    } else if (name == TransitiveClosureRole::get_name()) {
        return std::make_unique<TransitiveClosureRole>(name, std::move(children));
    } else if (name == TransitiveReflexiveClosureRole::get_name()) {
        return std::make_unique<TransitiveReflexiveClosureRole>(name, std::move(children));
    }
    return std::make_unique<Expression>(name, std::move(children));
}


}
