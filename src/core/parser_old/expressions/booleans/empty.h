#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_

#include "../boolean.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class EmptyBoolean : public Boolean {
private:
    static inline const std::string m_name = "b_empty";

public:
    EmptyBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Boolean> parse_boolean(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("EmptyBoolean::parse_boolean - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse and construct children
        auto concept_element = m_children[0]->parse_concept(factory);
        if (concept_element) {
            return factory.make_empty_boolean(concept_element);
        }
        auto role_element = m_children[0]->parse_role(factory);
        if (role_element) {
            return factory.make_empty_boolean(role_element);
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
