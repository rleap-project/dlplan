#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_NUMERICAL_COUNT_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../../../elements/numericals/count.h"

namespace dlp {
namespace lisp {

class CountNumericalExpression : public NumericalExpression {
protected:
    virtual NumericalElement_Ptr make_numerical_element_impl(const TaskInfo& task_info, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            std::cout << "CountNumericalExpression::make_numerical_element - number of children (" << m_children.size() << " != 1)." << std::endl;
            throw std::exception();
        }
        ConceptElement_Ptr concept_element = m_children[0]->make_concept_element(task_info, cache);
        if (concept_element) {
            return std::make_shared<CountNumericalElement<ConceptElement_Ptr>>(concept_element);
        }
        RoleElement_Ptr role_element = m_children[0]->make_role_element(task_info, cache);
        if (role_element) {
            return std::make_shared<CountNumericalElement<RoleElement_Ptr>>(role_element);
        }
        std::cout << "CountNumericalExpression::make_numerical_element - unable to construct children elements." << std::endl;
        throw std::exception();
    }

public:
    CountNumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : NumericalExpression(name, std::move(children)) { }
};

}
}

#endif
