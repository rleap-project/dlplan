#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_

#include "expression.h"

#include "../../elements/numerical.h"


namespace dlp {
namespace core {
namespace parser {

class NumericalExpression : public Expression {
protected:
    /**
     * Construct the NumericalElement.
     */
    virtual element::NumericalElement_Ptr parse_numerical_element_impl(const InstanceInfoImpl& info, ElementCache &cache) const = 0;

public:
    NumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the NumericalElement.
     */
    virtual element::NumericalElement_Ptr parse_numerical_element(const InstanceInfoImpl& info, ElementCache &cache) const {
        element::NumericalElement_Ptr value = parse_numerical_element_impl(info, cache);
        return cache.numerical_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
    }
};

}
}
}

#endif
