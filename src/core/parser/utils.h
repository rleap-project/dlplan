#ifndef DLP_SRC_CORE_PARSER_UTILS_H_
#define DLP_SRC_CORE_PARSER_UTILS_H_

#include <string>
#include <string.h>
#include <algorithm>
#include <stdexcept>

using namespace std::string_literals;


namespace dlp {
namespace core {
namespace parser {

int try_parse(const std::string& name) {
    if (!std::all_of(name.begin(), name.end(), [](char c){ return isdigit(c); })) {
        throw std::runtime_error("try_parse - error parsing ("s + name + ") to int.");
    }
    return atoi(name.c_str());
}

bool is_goal_version(const std::string& name) {
    return (strncmp(name.c_str(), "g_", 2) == 0);
}

std::string strip_type_identifier(const std::string& name) {
    return name.substr(2);
}

/**
 * Sort child expressions lexicographically.
 * We apply this in commutative expressions
 * to obtain a canonical representation.
 */
std::vector<Expression_Ptr> sort_children_lexicographically(std::vector<Expression_Ptr> &&children) {
    std::sort(children.begin(), children.end(),
        [](const Expression_Ptr &l, const Expression_Ptr &r){ return l->name() < r->name(); });
    return std::move(children);
}


}
}
}

#endif
