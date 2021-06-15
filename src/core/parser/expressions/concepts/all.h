#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ALL_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ALL_H_

#include "../concept.h"
#include "../../../elements/concepts/all.h"

namespace dlplan::core::parser {

class AllConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AllConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        element::Role_Ptr role = m_children[0]->parse_role(vocabulary, cache);
        element::Concept_Ptr concept = m_children[1]->parse_concept(vocabulary, cache);
        if (!(role && concept)) {
            throw std::runtime_error("AllConcept::parse_concept_impl - at least one child is a nullptr");
        }
        // 2. Construct element
        return std::make_unique<element::AllConcept>(vocabulary, role, concept);
    }

public:
    AllConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
