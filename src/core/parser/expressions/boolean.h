#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"

#include "../../elements/boolean.h"


namespace dlp::core::parser {

class Boolean : public Expression {
protected:
    /**
     * Construct the Boolean.
     */
    virtual element::Boolean_Ptr parse_boolean_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const = 0;

public:
    Boolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Boolean.
     */
    virtual element::Boolean_Ptr parse_boolean(const VocabularyInfo& vocabulary, ElementCache &cache) const {
        element::Boolean_Ptr value = parse_boolean_impl(vocabulary, cache);
        return cache.boolean_element_cache().emplace(value->compute_repr(), std::move(value)).first->second;
    }
};

}

#endif
