#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_COMPOSE_H_

#include "../role.h"
#include "../../../elements/roles/compose.h"
#include "../../utils.h"

namespace dlplan::core::parser {

class ComposeRole : public Role {
protected:
    std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("ComposeRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> role_left = m_children[0]->parse_role(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Role> role_right = m_children[1]->parse_role(vocabulary_info, cache);
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::parse_role_impl - children are not of type Role.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::ComposeRole>(vocabulary_info, role_left, role_right);
    }

public:
    ComposeRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
