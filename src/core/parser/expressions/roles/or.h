#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_

#include "../role.h"
#include "../../../elements/roles/or.h"
#include "../../utils.h"


namespace dlplan::core::parser {

class OrRole : public Role {
protected:
    std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("OrRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> left_role = m_children[0]->parse_role(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Role> right_role = m_children[1]->parse_role(vocabulary_info, cache);
        if (!(left_role && right_role)) {
            throw std::runtime_error("OrRole::parse_role_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::OrRole>(vocabulary_info, left_role, right_role);
    }

public:
    OrRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, sort_children_lexicographically(std::move(children))) { }
};

}

#endif
