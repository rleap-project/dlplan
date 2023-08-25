#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Boolean;
}

namespace dlplan::core::parser {

class Boolean : public Expression {
public:
    Boolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}

#endif
