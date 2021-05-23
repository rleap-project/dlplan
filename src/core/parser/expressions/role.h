#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_

#include "../expression.h"


namespace dlp {
namespace core {
namespace parser {

class RoleExpression : public Expression {
protected:
    /**
     * Construct the RoleElement.
     */
    virtual RoleElement_Ptr make_role_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const = 0;

public:
    RoleExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the RoleElement.
     */
    virtual RoleElement_Ptr make_role_element(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const {
        std::string key = str();
        if (!cache.role_element_cache().exists(key)) {
            cache.role_element_cache().insert(key, make_role_element_impl(task_info, cache));
        }
        return cache.role_element_cache().get(key);
    }
};

}
}
}

#endif
