#ifndef DLP_SRC_CORE_PARSER_AST_FACTORY_H_
#define DLP_SRC_CORE_PARSER_AST_FACTORY_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/expression.h"
#include "types.h"

namespace dlp {
namespace core {
namespace parser {


enum EXPRESSION_TYPE {
    // concepts
    C_ALL,
    C_AND,
    C_BOT,
    C_DIFFERENCE,
    C_NOT,
    C_ONE_OF,
    C_OR,
    C_SOME,
    C_SUBSET,
    C_TOP,
    // roles
    R_AND,
    R_COMPOSE,
    R_DIFFERENCE,
    R_IDENTITY,
    R_INVERSE,
    R_NOT,
    R_OR,
    R_RESTRICT,
    R_TOP,
    R_TRANSITIVE_CLOSURE,
    R_TRANSITIVE_REFLEXIVE_CLOSURE,
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
}

#endif
