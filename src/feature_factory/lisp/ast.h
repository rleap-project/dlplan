#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/expression.h"
#include "types.h"

namespace dlp {
namespace lisp {



enum EXPRESSION_TYPE {
    // concepts
    C_PRIMITIVE,
    C_AND,
    C_OR,
    // roles
    R_PRIMITIVE,
    R_AND,
    R_OR,
    // features
    F_COUNT,
    F_EMPTY,
    F_CONCEPT_DISTANCE,
    F_SUM_CONCEPT_DISTANCE,
    F_ROLE_DISTANCE,
    F_SUM_ROLE_DISTANCE,
};

/**
 *
 */
class AST_Factory {
private:
    static std::unordered_map<std::string, EXPRESSION_TYPE> m_name_to_expression_type;

    static EXPRESSION_TYPE name_to_expression_type(const std::string &name);

public:
    Expression_Ptr make_ast(const std::string &name, std::vector<Expression_Ptr> &&children);
};

}
}

#endif
