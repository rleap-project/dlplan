#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_

#include "../role.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class OrRole : public Role {
private:
    static inline const std::string m_name = "r_or";

public:
    OrRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, sort_children_lexicographically(std::move(children))) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("OrRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        auto left_role = m_children[0]->parse_role(factory);
        auto right_role = m_children[1]->parse_role(factory);
        if (!(left_role && right_role)) {
            throw std::runtime_error("OrRole::parse_role - children are not of type Concept.");
        }
        // 2. Construct element
        return factory.make_or_role(left_role, right_role);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
