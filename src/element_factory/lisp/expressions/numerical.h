#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_NUMERICAL_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_NUMERICAL_H_

#include "../expression.h"


namespace dlp {
namespace lisp {

class NumericalExpression : public Expression {
protected:
    virtual NumericalElement_Ptr make_numerical_element_impl(const TaskInfo& task_info, ElementCache &cache) const = 0;

public:
    NumericalExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    virtual NumericalElement_Ptr make_numerical_element(const TaskInfo& task_info, ElementCache &cache) const {
        std::string key = str();
        if (cache.numerical_element_cache().exists(key)) {
            return cache.numerical_element_cache().get(key);
        }
        cache.numerical_element_cache().insert(key, make_numerical_element_impl(task_info, cache));
        return cache.numerical_element_cache().get(key);
    }
};

}
}

#endif
