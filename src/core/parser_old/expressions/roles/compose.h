#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_COMPOSE_H_

#include "../role.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class ComposeRole : public Role {
private:
    static inline const std::string m_name = "r_compose";

public:
    ComposeRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("ComposeRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        auto role_left = m_children[0]->parse_role(factory);
        auto role_right = m_children[1]->parse_role(factory);
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::parse_role - children are not of type Role.");
        }
        // 2. Construct element
        return factory.make_compose_role(role_left, role_right);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
