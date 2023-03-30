#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_

#include "../concept.h"
#include "../../../elements/concepts/one_of.h"
#include "../../utils.h"

namespace dlplan::core::parser {

class OneOfConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("OneOfConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        std::string constant_name = m_children[0]->get_name();
        return std::make_unique<element::OneOfConcept>(vocabulary, vocabulary.get_constant(constant_name));
    }

public:
    OneOfConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
