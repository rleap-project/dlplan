#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_

#include "../../expression.h"

namespace dlp {
namespace core {
namespace parser {

class PrimitiveRoleExpression : public Expression {
public:
    PrimitiveRoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}
}
}

#endif
