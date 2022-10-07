#ifndef DLPLAN_SRC_CORE_PARSER_UTILS_H_
#define DLPLAN_SRC_CORE_PARSER_UTILS_H_

#include "types.h"
#include "expressions/expression.h"

#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std::string_literals;


namespace dlplan::core::parser {
int try_parse_number(const std::string& name) {
    if (!std::all_of(name.begin(), name.end(), [](char c){ return isdigit(c); })) {
        throw std::runtime_error("try_parse_number - error parsing ("s + name + ") to int.");
    }
    return atoi(name.c_str());
}


bool is_number(const std::string& name) {
    return std::all_of(name.begin(), name.end(), [](char c){ return isdigit(c); });
}

/**
 * Sort child expressions lexicographically.
 * We apply this in commutative expressions
 * to obtain a canonical representation.
 */
std::vector<Expression_Ptr> sort_children_lexicographically(std::vector<Expression_Ptr> &&children) {
    std::sort(children.begin(), children.end(),
        [](const Expression_Ptr &l, const Expression_Ptr &r){ return l->get_name() < r->get_name(); });
    return std::move(children);
}


}

#endif
