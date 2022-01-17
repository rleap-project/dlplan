#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"

#include "../../elements/boolean.h"


namespace dlplan::core::parser {

class Boolean : public Expression {
protected:
    /**
     * Construct the Boolean.
     */
    virtual std::unique_ptr<element::Boolean> parse_boolean_impl(const VocabularyInfo& vocabulary, Caches &caches) const = 0;

public:
    Boolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Boolean.
     */
    virtual element::Boolean_Ptr parse_boolean(const VocabularyInfo& vocabulary, Caches &caches) const {
        return caches.m_boolean_cache->insert(parse_boolean_impl(vocabulary, caches)).first;
    }
};

}

#endif
