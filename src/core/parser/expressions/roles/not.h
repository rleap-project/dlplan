#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_NOT_H_

#include "../role.h"
#include "../../../elements/roles/not.h"

namespace dlplan::core::parser {

class NotRole : public Role {
protected:
    std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NotRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> role = m_children[0]->parse_role(vocabulary_info, cache);
        if (!role) {
            throw std::runtime_error("NotRole::parse_role_impl - child is not of type Role.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::NotRole>(vocabulary_info, role);
    }

public:
    NotRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
