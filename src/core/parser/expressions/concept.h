#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "expression.h"

#include "../../elements/concept.h"

namespace dlp {
namespace core {
namespace parser {

class ConceptExpression : public Expression {
protected:
    /**
     * Construct the ConceptElement.
     */
    virtual element::ConceptElement_Ptr parse_concept_element_impl(std::shared_ptr<InstanceInfoImpl> info, ElementCache &cache) const = 0;

public:
    ConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the ConceptElement.
     */
    virtual element::ConceptElement_Ptr parse_concept_element(std::shared_ptr<InstanceInfoImpl> info, ElementCache &cache) const {
        element::ConceptElement_Ptr value = parse_concept_element_impl(info, cache);
        return cache.concept_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
    }
};

}
}
}

#endif
