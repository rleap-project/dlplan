#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class InclusionBoolean : public Boolean {
private:
    static inline const std::string m_name = "b_inclusion";

public:
    InclusionBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Boolean> parse_boolean(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("InclusionBoolean::parse_boolean - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse and construct children
        auto concept_left = m_children[0]->parse_concept(factory);
        auto concept_right = m_children[1]->parse_concept(factory);
        if (concept_left && concept_right) {
            return factory.make_inclusion_boolean(concept_left, concept_right);
        }
        auto role_left = m_children[0]->parse_role(factory);
        auto role_right = m_children[1]->parse_role(factory);
        if (role_left && role_right) {
            return factory.make_inclusion_boolean(role_left, role_right);
        }
        // 2. If unsuccessful then throw a runtime error.
        throw std::runtime_error("EmptyBoolean::parse_boolean - unable to construct children elements.");
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
