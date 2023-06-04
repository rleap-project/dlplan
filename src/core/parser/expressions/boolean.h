#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"
#include "../../cache.h"


namespace dlplan::core {
class Boolean;
}

namespace dlplan::core::parser {

class Boolean : public Expression {
protected:
    /**
     * Construct the Boolean.
     */
    virtual std::unique_ptr<dlplan::core::Boolean> parse_boolean_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const = 0;

public:
    Boolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Boolean.
     */
    virtual std::shared_ptr<const dlplan::core::Boolean> parse_boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const {
        return caches.insert(parse_boolean_impl(vocabulary_info, caches));
    }
};

}

#endif
