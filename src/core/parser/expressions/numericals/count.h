#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../../../elements/numericals/count.h"

namespace dlp {
namespace core {
namespace parser {

class CountNumericalExpression : public NumericalExpression {
protected:
    virtual element::Numerical_Ptr parse_numerical_element_impl(ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("CountNumericalExpression::parse_numerical_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Concept_Ptr concept_element = m_children[0]->parse_concept_element(cache);
        if (concept_element) {
            return std::make_shared<element::CountNumericalElement<element::Concept_Ptr>>(concept_element);
        }
        element::Role_Ptr role_element = m_children[0]->parse_role_element(cache);
        if (role_element) {
            return std::make_shared<element::CountNumericalElement<element::Role_Ptr>>(role_element);
        }
        // 2. Construct element
        throw std::runtime_error("CountNumericalExpression::parse_numerical_element - unable to construct children elements.");
    }

public:
    CountNumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : NumericalExpression(name, std::move(children)) { }
};

}
}
}

#endif
