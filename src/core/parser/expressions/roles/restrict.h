#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_RESTRICT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_RESTRICT_H_

#include "../role.h"
#include "../concept.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class RestrictRole : public Role {
private:
    static inline const std::string m_name = "r_restrict";

public:
    RestrictRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("RestrictRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        auto role = m_children[0]->parse_role(factory);
        auto concept = m_children[1]->parse_concept(factory);
        if (!(role && concept)) {
            throw std::runtime_error("RestrictRole::parse_role - children are not of type Role.");
        }
        // 2. Construct element
        return factory.make_restrict_role(role, concept);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
