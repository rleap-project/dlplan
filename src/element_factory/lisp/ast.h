#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "expression.h"
#include "types.h"

namespace dlp {
namespace lisp {


enum EXPRESSION_TYPE {
    // concepts
    C_AND,
    C_OR,
    // roles
    R_AND,
    R_OR,
    // numerical
    N_COUNT,
    N_CONCEPT_DISTANCE,
    N_SUM_CONCEPT_DISTANCE,
    N_ROLE_DISTANCE,
    N_SUM_ROLE_DISTANCE,
    // boolean
    B_EMPTY,
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
