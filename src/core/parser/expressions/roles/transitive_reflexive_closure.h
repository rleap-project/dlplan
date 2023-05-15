#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../../../elements/roles/transitive_reflexive_closure.h"

namespace dlplan::core::parser {

class TransitiveReflexiveClosureRole : public Role {
protected:
    std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> role = m_children[0]->parse_role(vocabulary_info, cache);
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::parse_role_impl - child is not of type Role.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::TransitiveReflexiveClosureRole>(vocabulary_info, role);
    }

public:
    TransitiveReflexiveClosureRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
