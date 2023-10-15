#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class IdentityRole : public Role {
private:
    static inline const std::string m_name = "r_identity";

public:
    IdentityRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("IdentityRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        auto concept = m_children[0]->parse_concept(factory);
        if (!concept) {
            throw std::runtime_error("IdentityRole::parse_role - child is not of type concept");
        }
        // 2. Construct element
        return factory.make_identity_role(concept);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
