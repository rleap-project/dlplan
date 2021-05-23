#include "ast_factory.h"

#include <algorithm>
#include <iostream>

#include "expressions/concepts/primitive.h"
#include "expressions/concepts/and.h"
#include "expressions/roles/primitive.h"
#include "expressions/numericals/count.h"

namespace dlp {
namespace lisp {


Expression_Ptr AST_Factory::make_ast(const std::string &name, std::vector<Expression_Ptr> &&children) {
    if (std::all_of(children.begin(), children.end(), [](const Expression_Ptr &c){ return c->is_leaf(); })) {
        // case 1: all children are leafs
        if (children.size() == 0) {
            // case 1.3: index
            return std::make_unique<lisp::Expression>(lisp::Expression(name, std::move(children)));
        } else if (children.size() == 1) {
            // case 1.2: primitive concept
            return std::make_unique<lisp::PrimitiveConceptExpression>(lisp::PrimitiveConceptExpression(name, std::move(children)));
        } else if (children.size() == 2) {
            // case 1.3: primitive role
            return std::make_unique<lisp::PrimitiveRoleExpression>(lisp::PrimitiveRoleExpression(name, std::move(children)));
        } else {
            throw std::runtime_error("AST_Factory::make_ast - invalid number ("s + std::to_string(children.size()) + ") of children in primitive ("s + name + ")");
        }
    } else if (!std::all_of(children.begin(), children.end(), [](const Expression_Ptr &c){ return c->is_leaf(); })) {
        // case 2: all childrens are inner nodes
        EXPRESSION_TYPE expression_type = name_to_expression_type(name);
        switch (expression_type) {
            case C_AND: {
                return std::make_unique<lisp::AndConceptExpression>(lisp::AndConceptExpression(name, std::move(children)));
            }
            case N_COUNT: {
                return std::make_unique<lisp::CountNumericalExpression>(lisp::CountNumericalExpression(name, std::move(children)));
            }
        }
    } else {
        throw std::runtime_error("AST_Factory::make_ast - children containing leafs and inner nodes not allowed.");
    }
}

std::unordered_map<std::string, EXPRESSION_TYPE> AST_Factory::m_name_to_expression_type = {
    {"c_and", C_AND },
    {"n_count", N_COUNT },
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
