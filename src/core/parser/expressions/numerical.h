#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_

#include "expression.h"


namespace dlp {
namespace core {
namespace parser {

class NumericalExpression : public Expression {
protected:
    /**
     * Construct the NumericalElement.
     */
    virtual element::NumericalElement_Ptr make_numerical_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const = 0;

public:
    NumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the NumericalElement.
     */
    virtual element::NumericalElement_Ptr make_numerical_element(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const {
        std::string key = str();
        if (!cache.numerical_element_cache().exists(key)) {
            cache.numerical_element_cache().insert(key, make_numerical_element_impl(task_info, cache));
        }
        return cache.numerical_element_cache().get(key);
    }
};

}
}
}

#endif
