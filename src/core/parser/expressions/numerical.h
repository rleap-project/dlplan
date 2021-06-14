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
    virtual element::Numerical_Ptr parse_numerical_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const = 0;

public:
    Numerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Numerical.
     */
    virtual element::Numerical_Ptr parse_numerical(const VocabularyInfo& vocabulary, ElementCache &cache) const {
        element::Numerical_Ptr value = parse_numerical_impl(vocabulary, cache);
        return cache.numerical_element_cache().emplace(value->compute_repr(), std::move(value)).first->second;
    }
};

}

#endif
