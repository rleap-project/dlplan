#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_AND_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_AND_H_

#include "../../expression.h"
#include "../../../elements/concepts/and.h"

namespace dlp {
namespace lisp {

class AndConceptExpression : public Expression {
public:
    AndConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    virtual ConceptElement_Ptr make_concept_element(const TaskInfo& task_info, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            std::cout << "AndConceptExpression::make_concept_element - number of children (" << m_children.size() << " != 1)." << std::endl;
            throw std::exception();
        }
        ConceptElement_Ptr l = m_children[0]->make_concept_element(task_info, cache);
        ConceptElement_Ptr r = m_children[1]->make_concept_element(task_info, cache);
        if (!(l && r)) {
            std::cout << "AndConceptExpression::make_concept_element - children are not of type ConceptElement." << std::endl;
            throw std::exception();
        }
        return std::make_shared<AndConceptElement>(l, r);
    }
};

}
}

#endif
