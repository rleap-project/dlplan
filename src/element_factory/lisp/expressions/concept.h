#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPT_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_CONCEPT_H_

#include "../expression.h"

namespace dlp {
namespace lisp {

class ConceptExpression : public Expression {
protected:
    virtual ConceptElement_Ptr make_concept_element_impl(const TaskInfo& task_info, ElementCache &cache) const = 0;

public:
    ConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    virtual ConceptElement_Ptr make_concept_element(const TaskInfo& task_info, ElementCache &cache) const {
        std::string key = str();
        if (cache.concept_element_cache().exists(key)) {
            return cache.concept_element_cache().get(key);
        }
        cache.concept_element_cache().insert(key, make_concept_element_impl(task_info, cache));
        return cache.concept_element_cache().get(key);
    }
};

}
}

#endif
