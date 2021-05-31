#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_

#include "expression.h"

#include "../../elements/numerical.h"


namespace dlp {
namespace core {
namespace parser {

class Numerical : public Expression {
protected:
    /**
     * Construct the Numerical.
     */
    virtual element::Numerical_Ptr parse_numerical_element_impl(ElementCache &cache) const = 0;

public:
    Numerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Numerical.
     */
    virtual element::Numerical_Ptr parse_numerical_element(ElementCache &cache) const {
        element::Numerical_Ptr value = parse_numerical_element_impl(cache);
        return cache.numerical_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
    }
};

}
}
}

#endif
