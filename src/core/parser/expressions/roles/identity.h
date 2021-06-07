#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"
#include "../../../elements/roles/identity.h"


namespace dlp {
namespace core {
namespace parser {

class IdentityRole : public Role {
protected:
    virtual element::Role_Ptr parse_role_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("IdentityRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Concept_Ptr concept = m_children[0]->parse_concept(vocabulary, cache);
        if (!concept) {
            throw std::runtime_error("IdentityRole::parse_role_impl - child is not of type concept");
        }
        // 2. Construct element
        return std::make_shared<element::IdentityRole>(vocabulary, concept);
    }

public:
    IdentityRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}
}
}

#endif
