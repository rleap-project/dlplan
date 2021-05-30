#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_

#include "../role.h"

namespace dlp {
namespace core {
namespace parser {

class PrimitiveRoleExpression : public RoleExpression {
protected:
    virtual element::RoleElement_Ptr parse_role_element_impl(ElementCache &cache) const override {
        // TODO(dominik): implement
    }

public:
    PrimitiveRoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : RoleExpression(name, std::move(children)) { }
};

}
}
}

#endif
