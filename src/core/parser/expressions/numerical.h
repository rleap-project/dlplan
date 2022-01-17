#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_H_

#include "expression.h"

#include "../../elements/numerical.h"


namespace dlplan::core::parser {

class Numerical : public Expression {
protected:
    /**
     * Construct the Numerical.
     */
    virtual std::unique_ptr<element::Numerical> parse_numerical_impl(const VocabularyInfo& vocabulary, Caches &caches) const = 0;

public:
    Numerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Numerical.
     */
    virtual element::Numerical_Ptr parse_numerical(const VocabularyInfo& vocabulary, Caches &caches) const {
        return caches.m_numerical_cache->insert(parse_numerical_impl(vocabulary, caches)).first;
    }
};

}

#endif
