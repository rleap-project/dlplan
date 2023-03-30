#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_

#include "../role.h"
#include "../../utils.h"
#include "../../../elements/roles/primitive.h"

namespace dlplan::core::parser {

class PrimitiveRole : public Role {
protected:
    std::unique_ptr<element::Role> parse_role_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("PrimitiveRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        const std::string& predicate_name = m_children[0]->get_name();
        int pos_1 = try_parse_number(m_children[1]->get_name());
        int pos_2 = try_parse_number(m_children[2]->get_name());
        // 2. Construct element
        return std::make_unique<element::PrimitiveRole>(vocabulary, vocabulary.get_predicate(predicate_name), pos_1, pos_2);
    }

public:
    PrimitiveRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
