#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../../elements/concepts/and.h"

namespace dlp {
namespace core {
namespace parser {

class AndConceptExpression : public ConceptExpression {
protected:
    virtual element::ConceptElement_Ptr parse_concept_element_impl(ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConceptExpression::parse_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::ConceptElement_Ptr l = m_children[0]->parse_concept_element(cache);
        element::ConceptElement_Ptr r = m_children[1]->parse_concept_element(cache);
        if (!(l && r)) {
            throw std::runtime_error("AndConceptExpression::parse_concept_element - children are not of type ConceptElement.");
        }
        // 2. Construct element
        return std::make_shared<element::AndConceptElement>(l, r);
    }

public:
    AndConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, sort_children_lexicographically(std::move(children))) { }
};

}
}
}

#endif
