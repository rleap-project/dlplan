#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_TOP_H_

#include "../concept.h"
#include "../../../elements/concepts/top.h"

namespace dlplan::core::parser {

class TopConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("TopConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return std::make_unique<element::TopConcept>(vocabulary);
    }

public:
    TopConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
