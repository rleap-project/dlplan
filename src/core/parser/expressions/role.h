#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Role;
}


namespace dlplan::core::parser {

class Role : public Expression {
public:
    Role(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}

#endif
