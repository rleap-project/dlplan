#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "../expression.h"

namespace dlp {
namespace core {
namespace parser {

class ConceptExpression : public Expression {
protected:
    /**
     * Construct the ConceptElement.
     */
    virtual element::ConceptElement_Ptr make_concept_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const = 0;

public:
    ConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the ConceptElement.
     */
    virtual element::ConceptElement_Ptr make_concept_element(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const {
        std::string key = str();
        if (!cache.concept_element_cache().exists(key)) {
            cache.concept_element_cache().insert(key, make_concept_element_impl(task_info, cache));
        }
        return cache.concept_element_cache().get(key);
    }
};

}
}
}

#endif
