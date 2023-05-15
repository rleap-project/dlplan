#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"
#include "../../../elements/roles/identity.h"


namespace dlplan::core::parser {

class IdentityRole : public Role {
protected:
    std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("IdentityRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Concept> concept = m_children[0]->parse_concept(vocabulary_info, cache);
        if (!concept) {
            throw std::runtime_error("IdentityRole::parse_role_impl - child is not of type concept");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::IdentityRole>(vocabulary_info, concept);
    }

public:
    IdentityRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
