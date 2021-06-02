#include "ast_factory.h"

#include <algorithm>
#include <iostream>

#include "expressions/booleans/empty.h"
#include "expressions/concepts/all.h"
#include "expressions/concepts/and.h"
#include "expressions/concepts/bot.h"
#include "expressions/concepts/diff.h"
#include "expressions/concepts/not.h"
#include "expressions/concepts/or.h"
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


namespace dlp {
namespace core {
namespace parser {

Expression_Ptr AST_Factory::make_ast(const VocabularyInfoImpl& vocabulary_info, const std::string &name, std::vector<Expression_Ptr> &&children) {
    // case 1: name is in alphabet of elements
    if (vocabulary_info.exists_element_name(name)) {
        EXPRESSION_TYPE expression_type = vocabulary_info.element_name_to_expression_type(name);
        switch (expression_type) {
            case B_EMPTY: {
                return std::make_unique<EmptyBoolean>(EmptyBoolean(name, std::move(children)));
            }
            case C_ALL: {
                return std::make_unique<AllConcept>(AllConcept(name, std::move(children)));
            }
            case C_AND: {
                return std::make_unique<AndConcept>(AndConcept(name, std::move(children)));
            }
            case C_BOT: {
                return std::make_unique<BotConcept>(BotConcept(name, std::move(children)));
            }
            case C_DIFF: {
                return std::make_unique<DiffConcept>(DiffConcept(name, std::move(children)));
            }
            case C_NOT: {
                return std::make_unique<NotConcept>(NotConcept(name, std::move(children)));
            }
            case C_OR: {
                return std::make_unique<OrConcept>(OrConcept(name, std::move(children)));
            }
            case C_SOME: {
                return std::make_unique<SomeConcept>(SomeConcept(name, std::move(children)));
            }
            case C_SUBSET: {
                return std::make_unique<SubsetConcept>(SubsetConcept(name, std::move(children)));
            }
            case C_TOP: {
                return std::make_unique<TopConcept>(TopConcept(name, std::move(children)));
            }
            case N_CONCEPT_DISTANCE: {
                return std::make_unique<ConceptDistanceNumerical>(ConceptDistanceNumerical(name, std::move(children)));
            }
            case N_COUNT: {
                return std::make_unique<CountNumerical>(CountNumerical(name, std::move(children)));
            }
            case N_ROLE_DISTANCE: {
                return std::make_unique<RoleDistanceNumerical>(RoleDistanceNumerical(name, std::move(children)));
            }
            case N_SUM_CONCEPT_DISTANCE: {
                return std::make_unique<SumConceptDistanceNumerical>(SumConceptDistanceNumerical(name, std::move(children)));
            }
            case N_SUM_ROLE_DISTANCE: {
                return std::make_unique<SumRoleDistanceNumerical>(SumRoleDistanceNumerical(name, std::move(children)));
            }
            case R_AND: {
                return std::make_unique<AndRole>(AndRole(name, std::move(children)));
            }
            case R_COMPOSE: {
                return std::make_unique<ComposeRole>(ComposeRole(name, std::move(children)));
            }
            case R_DIFF: {
                return std::make_unique<DiffRole>(DiffRole(name, std::move(children)));
            }
            case R_IDENTITY: {
                return std::make_unique<IdentityRole>(IdentityRole(name, std::move(children)));
            }
            case R_INVERSE: {
                return std::make_unique<InverseRole>(InverseRole(name, std::move(children)));
            }
            case R_NOT: {
                return std::make_unique<NotRole>(NotRole(name, std::move(children)));
            }
            case R_OR: {
                return std::make_unique<OrRole>(OrRole(name, std::move(children)));
            }
            case R_RESTRICT: {
                return std::make_unique<RestrictRole>(RestrictRole(name, std::move(children)));
            }
            case R_TOP: {
                return std::make_unique<TopRole>(TopRole(name, std::move(children)));
            }
            case R_TRANSITIVE_CLOSURE: {
                return std::make_unique<TransitiveClosureRole>(TransitiveClosureRole(name, std::move(children)));
            }
            case R_TRANSITIVE_REFLEXIVE_CLOSURE: {
                return std::make_unique<TransitiveReflexiveClosureRole>(TransitiveReflexiveClosureRole(name, std::move(children)));
            }
        }
    }
    // case 2: name is in alphabet of VocabularyInfo
    else if (vocabulary_info.exists_predicate_name(name)) {
        if (children.size() == 2) {
            return std::make_unique<PrimitiveRole>(PrimitiveRole(name, std::move(children)));
        } else if (children.size() == 1) {
            return std::make_unique<PrimitiveConcept>(PrimitiveConcept(name, std::move(children)));
        } else if (children.size() == 0) {
            throw std::runtime_error("AST_Factory::make_ast - unspecified projection for predicate ("s + name + "). Is this supposed to be a concept or a role?");
        }
    }
    // case 3: name is an integer indicating projection
    else if (is_number(name)) {
        return std::make_unique<Expression>(Expression(name, std::move(children)));
    }
    // case 4: wrong syntax
    throw std::runtime_error("AST_Factory::make_ast - missing implementation for word ("s + name + ").");
}


}
}
}
