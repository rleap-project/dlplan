#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../../elements/concepts/and.h"
#include "../../utils.h"

namespace dlplan::core::parser {

class AndConcept : public Concept {
protected:
    std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Concept> l = m_children[0]->parse_concept(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Concept> r = m_children[1]->parse_concept(vocabulary_info, cache);
        if (!(l && r)) {
            throw std::runtime_error("AndConcept::parse_concept_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::AndConcept>(vocabulary_info, l, r);
    }

public:
    AndConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, sort_children_lexicographically(std::move(children))) { }
};

}

#endif
