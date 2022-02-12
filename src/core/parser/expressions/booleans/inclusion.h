#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"
#include "../../../elements/booleans/inclusion.h"

namespace dlplan::core::parser {

class InclusionBoolean : public Boolean {
protected:
    std::unique_ptr<element::Boolean> parse_boolean_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("InclusionBoolean::parse_boolean_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse and construct children
        element::Concept_Ptr concept_left = m_children[0]->parse_concept(vocabulary, cache);
        element::Concept_Ptr concept_right = m_children[1]->parse_concept(vocabulary, cache);
        if (concept_left && concept_right) {
            return std::make_unique<element::InclusionBoolean<element::Concept>>(vocabulary, concept_left, concept_right);
        }
        element::Role_Ptr role_left = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr role_right = m_children[1]->parse_role(vocabulary, cache);
        if (role_left && role_right) {
            return std::make_unique<element::InclusionBoolean<element::Role>>(vocabulary, role_left, role_right);
        }
        // 2. If unsuccessful then throw a runtime error.
        throw std::runtime_error("EmptyBoolean::parse_boolean_impl - unable to construct children elements.");
    }

public:
    InclusionBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }
};

}

#endif
