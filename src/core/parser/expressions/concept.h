#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Concept;
}

namespace dlplan::core::parser {

class Concept : public Expression {
protected:
    /**
     * Construct the Concept.
     */
    virtual std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const = 0;

public:
    Concept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Concept.
     */
    virtual std::shared_ptr<const dlplan::core::Concept> parse_concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const {
        return caches.insert(parse_concept_impl(vocabulary_info, caches));
    }
};

}

#endif
