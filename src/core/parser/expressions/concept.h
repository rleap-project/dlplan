#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "expression.h"

#include "../../elements/concept.h"

namespace dlp {
namespace core {
namespace parser {

class Concept : public Expression {
protected:
    /**
     * Construct the Concept.
     */
    virtual element::Concept_Ptr parse_concept_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const = 0;

public:
    Concept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Concept.
     */
    virtual element::Concept_Ptr parse_concept(const VocabularyInfo& vocabulary, ElementCache &cache) const {
        element::Concept_Ptr value = parse_concept_impl(vocabulary, cache);
        return cache.concept_element_cache().emplace(value->compute_repr(), std::move(value)).first->second;
    }
};

}
}
}

#endif
