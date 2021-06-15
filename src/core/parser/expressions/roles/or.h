#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_

#include "../role.h"
#include "../../../elements/roles/or.h"
#include "../../utils.h"


namespace dlplan::core::parser {

class OrRole : public Role {
protected:
    std::unique_ptr<element::Role> parse_role_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("OrRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        element::Role_Ptr left_role = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr right_role = m_children[1]->parse_role(vocabulary, cache);
        if (!(left_role && right_role)) {
            throw std::runtime_error("OrRole::parse_role_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_unique<element::OrRole>(vocabulary, left_role, right_role);
    }

public:
    OrRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, sort_children_lexicographically(std::move(children))) { }
};

}

#endif
