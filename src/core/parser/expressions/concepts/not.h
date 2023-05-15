#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_

#include "../concept.h"
#include "../../../elements/concepts/not.h"

namespace dlplan::core::parser {

class NotConcept : public Concept {
protected:
    std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NotConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Concept> l = m_children[0]->parse_concept(vocabulary_info, cache);
        if (!l) {
            throw std::runtime_error("NotConcept::parse_concept_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::NotConcept>(vocabulary_info, l);
    }

public:
    NotConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
