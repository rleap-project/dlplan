#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_BOOLEAN_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_BOOLEAN_H_

#include "../expression.h"


namespace dlp {
namespace lisp {

class BooleanExpression : public Expression {
protected:
    /**
     * Construct the BooleanElement.
     */
    virtual BooleanElement_Ptr make_boolean_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const = 0;

public:
    BooleanExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the BooleanElement.
     */
    virtual BooleanElement_Ptr make_boolean_element(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const {
        std::string key = str();
        if (!cache.boolean_element_cache().exists(key)) {
            cache.boolean_element_cache().insert(key, make_boolean_element_impl(task_info, cache));
        }
        return cache.boolean_element_cache().get(key);
    }
};

}
}

#endif
