#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_AND_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../../elements/concepts/and.h"

namespace dlp {
namespace lisp {

class AndConceptExpression : public ConceptExpression {
protected:
    virtual ConceptElement_Ptr make_concept_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConceptExpression::make_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        ConceptElement_Ptr l = m_children[0]->make_concept_element(task_info, cache);
        ConceptElement_Ptr r = m_children[1]->make_concept_element(task_info, cache);
        if (!(l && r)) {
            throw std::runtime_error("AndConceptExpression::make_concept_element - children are not of type ConceptElement.");
        }
        return std::make_shared<AndConceptElement>(task_info, l->goal() && r->goal(), l, r);
    }

public:
    AndConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, std::move(children)) { }
};

}
}

#endif
