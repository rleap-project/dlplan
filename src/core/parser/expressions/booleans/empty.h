#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_

#include "../boolean.h"
#include "../../../elements/booleans/empty.h"

namespace dlplan::core::parser {

class EmptyBoolean : public Boolean {
protected:
    element::Boolean_Ptr parse_boolean_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("EmptyBoolean::parse_boolean_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Concept_Ptr concept_element = m_children[0]->parse_concept(vocabulary, cache);
        if (concept_element) {
            return std::make_shared<element::EmptyBoolean<element::Concept_Ptr>>(vocabulary, concept_element);
        }
        element::Role_Ptr role_element = m_children[0]->parse_role(vocabulary, cache);
        if (role_element) {
            return std::make_shared<element::EmptyBoolean<element::Role_Ptr>>(vocabulary, role_element);
        }
        // 2. Construct element
        throw std::runtime_error("EmptyBoolean::parse_boolean_impl - unable to construct children elements.");
    }

public:
    EmptyBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }
};

}

#endif
