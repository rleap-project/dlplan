#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Numerical;
}


namespace dlplan::core::parser {

class Numerical : public Expression {
public:
    Numerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}

#endif
