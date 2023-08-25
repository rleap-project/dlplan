#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Concept;
}

namespace dlplan::core::parser {

class Concept : public Expression {
public:
    Concept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }
};

}

#endif
