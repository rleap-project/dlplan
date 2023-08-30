#ifndef DLPLAN_SRC_CORE_PARSER_UTILS_H_
#define DLPLAN_SRC_CORE_PARSER_UTILS_H_

#include "expressions/expression.h"

#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

using namespace std::string_literals;


namespace dlplan::core::parser {
extern int try_parse_number(const std::string& name);

extern bool is_number(const std::string& name);

/**
 * Sort child expressions lexicographically.
 * We apply this in commutative expressions
 * to obtain a canonical representation.
 */
extern std::vector<std::unique_ptr<Expression>> sort_children_lexicographically(std::vector<std::unique_ptr<Expression>> &&children);

}

#endif
