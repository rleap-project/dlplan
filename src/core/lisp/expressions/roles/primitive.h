#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_ROLES_PRIMITIVE_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_ROLES_PRIMITIVE_H_

#include "../../expression.h"

namespace dlp {
namespace lisp {

class PrimitiveRoleExpression : public Expression {
public:
    PrimitiveRoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}
}

#endif
