#include "ast_factory.h"

#include <algorithm>
#include <iostream>

#include "expressions/concepts/primitive.h"
#include "expressions/concepts/and.h"
#include "expressions/roles/primitive.h"
#include "expressions/numericals/count.h"

namespace dlp {
namespace core {
namespace parser {


Expression_Ptr AST_Factory::make_ast(const std::string &name, std::vector<Expression_Ptr> &&children) {
    if (std::all_of(children.begin(), children.end(), [](const Expression_Ptr &c){ return c->is_leaf(); })) {
        // case 1: all children are leafs
        if (children.size() == 0) {
            // case 1.3: index
            if (!is_number(name)) {
                throw std::runtime_error("AST_Factory::make_ast - leaves must be integer corresponding to positions in predicates");
            }
            return std::make_unique<Expression>(Expression(name, std::move(children)));
        } else if (children.size() == 1) {
            // case 1.2: primitive concept
            return std::make_unique<PrimitiveConcept>(PrimitiveConcept(name, std::move(children)));
        } else if (children.size() == 2) {
            // case 1.3: primitive role
            return std::make_unique<PrimitiveRole>(PrimitiveRole(name, std::move(children)));
        } else {
            throw std::runtime_error("AST_Factory::make_ast - invalid number ("s + std::to_string(children.size()) + ") of children in primitive ("s + name + ")");
        }
    } else if (!std::all_of(children.begin(), children.end(), [](const Expression_Ptr &c){ return c->is_leaf(); })) {
        // case 2: all childrens are inner nodes
        EXPRESSION_TYPE expression_type = name_to_expression_type(name);
        switch (expression_type) {
            case C_AND: {
                return std::make_unique<AndConcept>(AndConcept(name, std::move(children)));
            }
            case N_COUNT: {
                return std::make_unique<CountNumerical>(CountNumerical(name, std::move(children)));
            }
        }
    } else {
        throw std::runtime_error("AST_Factory::make_ast - children containing leafs and inner nodes not allowed.");
    }
}

std::unordered_map<std::string, EXPRESSION_TYPE> AST_Factory::m_name_to_expression_type = {
    {"c_all", C_ALL },
    {"c_and", C_AND },
    {"c_bot", C_BOT },
    {"c_difference", C_DIFFERENCE },
    {"c_not", C_NOT },
    {"c_one_of", C_ONE_OF },
    {"c_or", C_OR },
    {"c_some", C_SOME },
    {"c_subset", C_SUBSET},
    {"c_top", C_TOP },
    {"r_and", R_AND },
    {"r_compose", R_COMPOSE },
    {"r_difference", R_DIFFERENCE },
    {"r_identity", R_IDENTITY },
    {"r_inverse", R_INVERSE },
    {"r_not", R_NOT },
    {"r_or", R_OR },
    {"r_restrict", R_RESTRICT },
    {"r_top", R_TOP },
    {"r_transitive_closure", R_TRANSITIVE_CLOSURE },
    {"r_transitive_reflexive_closure", R_TRANSITIVE_REFLEXIVE_CLOSURE },
    {"n_count", N_COUNT },
    {"n_concept_distance", N_CONCEPT_DISTANCE },
    {"n_sum_concept_distance", N_SUM_CONCEPT_DISTANCE },
    {"n_role_distance", N_ROLE_DISTANCE },
    {"n_sum_role_distance", N_SUM_ROLE_DISTANCE },
    {"b_empty", B_EMPTY }
};

EXPRESSION_TYPE AST_Factory::name_to_expression_type(const std::string &name) {
    auto p = AST_Factory::m_name_to_expression_type.find(name);
    if (p == AST_Factory::m_name_to_expression_type.end()) {
        throw std::runtime_error("AST_Factory::name_to_expression_type - undefined expression name ("s + name + ").");
    }
    return p->second;
}


}
}
}
