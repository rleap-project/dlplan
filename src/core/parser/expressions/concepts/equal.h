#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_EQUAL_H_

#include "../concept.h"
#include "../../../elements/concepts/equal.h"

namespace dlplan::core::parser {

class EqualConcept : public Concept {
protected:
    std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("EqualConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> role_left = m_children[0]->parse_role(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Role> role_right = m_children[1]->parse_role(vocabulary_info, cache);
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::parse_concept_impl - at least one children is a nullptr");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::EqualConcept>(vocabulary_info, role_left, role_right);
    }

public:
    EqualConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
