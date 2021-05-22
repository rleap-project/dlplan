#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../../elements/concepts/primitive.h"


namespace dlp {
namespace lisp {

class PrimitiveConceptExpression : public ConceptExpression {
protected:
    virtual ConceptElement_Ptr make_concept_element_impl(const TaskInfo& task_info, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            std::cout << "PrimitiveConceptExpression::make_concept_element - number of children (" << m_children.size() << " != 1)." << std::endl;
            throw std::exception();
        }
        if (!task_info.exists_predicate_name(m_name)) {
            std::cout << "PrimitiveConceptExpression::make_concept_element - predicate is missing in TaskInfo." << std::endl;
            throw std::exception();
        }
        unsigned predicate_idx = task_info.predicate_idx(m_name);
        unsigned predicate_arity = task_info.predicate_arity(predicate_idx);
        // TODO(dominik): Add check that this works and add error message
        unsigned object_idx = atoi(m_children[0]->str().c_str());
        if (object_idx > predicate_arity) {
            std::cout << "PrimitiveConceptExpression::make_concept_element - object index does not match predicate arity (" << object_idx << " > " << predicate_arity << ")." << std::endl;
            throw std::exception();
        }
        return std::make_shared<PrimitiveConceptElement>(predicate_idx, object_idx);
    }

public:
    PrimitiveConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, std::move(children)) { }
};

}
}

#endif
