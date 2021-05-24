#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../../elements/concepts/and.h"

namespace dlp {
namespace core {
namespace parser {

class AndConceptExpression : public ConceptExpression {
protected:
    virtual element::ConceptElement_Ptr make_concept_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConceptExpression::make_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        element::ConceptElement_Ptr l = m_children[0]->make_concept_element(task_info, cache);
        element::ConceptElement_Ptr r = m_children[1]->make_concept_element(task_info, cache);
        if (!(l && r)) {
            throw std::runtime_error("AndConceptExpression::make_concept_element - children are not of type ConceptElement.");
        }
        return std::make_shared<element::AndConceptElement>(task_info, l->goal() && r->goal(), l, r);
    }

public:
    AndConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, sort_children_lexicographically(std::move(children))) { }
};

}
}
}

#endif
