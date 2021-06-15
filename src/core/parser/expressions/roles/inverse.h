#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_INVERSE_H_

#include "../role.h"
#include "../concept.h"
#include "../../../elements/roles/inverse.h"


namespace dlplan::core::parser {

class InverseRole : public Role {
protected:
    std::unique_ptr<element::Role> parse_role_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("InverseRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Role_Ptr role = m_children[0]->parse_role(vocabulary, cache);
        if (!role) {
            throw std::runtime_error("InverseRole::parse_role_impl - child is not of type role");
        }
        // 2. Construct element
        return std::make_unique<element::InverseRole>(vocabulary, role);
    }

public:
    InverseRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
