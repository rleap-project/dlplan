#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../utils.h"
#include "../../../elements/concepts/primitive.h"


namespace dlplan::core::parser {

class PrimitiveConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("PrimitiveConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        const std::string& predicate_name = m_children[0]->get_name();
        int pos = try_parse_number(m_children[1]->get_name());
        // 2. Construct element
        return std::make_unique<element::PrimitiveConcept>(vocabulary, vocabulary.get_predicate(predicate_name), pos);
    }

public:
    PrimitiveConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
