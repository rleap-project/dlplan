#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPT_H_

#include "expression.h"

#include "../../elements/concept.h"

namespace dlplan::core::parser {

class Concept : public Expression {
protected:
    /**
     * Construct the Concept.
     */
    virtual std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &caches) const = 0;

public:
    Concept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Concept.
     */
    virtual element::Concept_Ptr parse_concept(const VocabularyInfo& vocabulary, Caches &caches) const {
        return caches.m_concept_cache->insert(parse_concept_impl(vocabulary, caches)).first;
    }
};

}

#endif
