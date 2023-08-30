#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_PRIMITIVE_H_

#include "../role.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class PrimitiveRole : public Role {
private:
    static inline const std::string m_name = "r_primitive";

public:
    PrimitiveRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("PrimitiveRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        const auto& predicate_name = m_children[0]->get_name();
        int pos_1 = try_parse_number(m_children[1]->get_name());
        int pos_2 = try_parse_number(m_children[2]->get_name());
        // 2. Construct element
        return factory.make_primitive_role(factory.get_vocabulary_info()->get_predicate(predicate_name), pos_1, pos_2);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
