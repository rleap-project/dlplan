#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_OR_H_

#include "../role.h"
#include "../../../elements/roles/or.h"
#include "../../utils.h"


namespace dlp {
namespace core {
namespace parser {

class OrRole : public Role {
protected:
    virtual element::Role_Ptr parse_role_impl(const VocabularyInfoImpl& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("OrRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        element::Role_Ptr l = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr r = m_children[1]->parse_role(vocabulary, cache);
        if (!(l && r)) {
            throw std::runtime_error("OrRole::parse_role_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_shared<element::OrRole>(vocabulary, l, r);
    }

public:
    OrRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, sort_children_lexicographically(std::move(children))) { }
};

}
}
}

#endif
