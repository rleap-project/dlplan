#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_ROLE_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_ROLE_H_

#include "../expression.h"


namespace dlp {
namespace lisp{

class NumericalExpression : public Expression {
protected:
    virtual RoleElement_Ptr make_role_element_impl(const TaskInfo& task_info, ElementCache &cache) const = 0;

public:
    RoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    virtual RoleElement_Ptr make_role_element(const TaskInfo& task_info, ElementCache &cache) const {
        std::string key = str();
        if (cache.role_element_cache().exists(key)) {
            return cache.role_element_cache().get(key);
        }
        cache.role_element_cache().insert(key, make_role_element_impl(task_info, cache));
        return cache.role_element_cache().get(key);
    }
};


}
}

#endif
