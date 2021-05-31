#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_

#include "expression.h"

#include "../../elements/role.h"

namespace dlp {
namespace core {
namespace parser {

class RoleExpression : public Expression {
protected:
    /**
     * Construct the Role.
     */
    virtual element::Role_Ptr parse_role_element_impl(ElementCache &cache) const = 0;

public:
    RoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Role.
     */
    virtual element::Role_Ptr parse_role_element(ElementCache &cache) const {
        element::Role_Ptr value = parse_role_element_impl(cache);
        return cache.role_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
    }
};

}
}
}

#endif
