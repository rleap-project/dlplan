#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../../../elements/numericals/count.h"

namespace dlp {
namespace core {
namespace parser {

class CountNumericalExpression : public NumericalExpression {
protected:
    virtual NumericalElement_Ptr make_numerical_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("CountNumericalExpression::make_numerical_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        ConceptElement_Ptr concept_element = m_children[0]->make_concept_element(task_info, cache);
        if (concept_element) {
            return std::make_shared<CountNumericalElement<ConceptElement_Ptr>>(task_info, concept_element->goal(), concept_element);
        }
        RoleElement_Ptr role_element = m_children[0]->make_role_element(task_info, cache);
        if (role_element) {
            return std::make_shared<CountNumericalElement<RoleElement_Ptr>>(task_info, role_element->goal(), role_element);
        }
        throw std::runtime_error("CountNumericalExpression::make_numerical_element - unable to construct children elements.");
    }

public:
    CountNumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : NumericalExpression(name, std::move(children)) { }
};

}
}
}

#endif
