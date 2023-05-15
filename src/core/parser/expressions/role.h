#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_

#include "expression.h"

namespace dlplan::core {
class Role;
}


namespace dlplan::core::parser {

class Role : public Expression {
protected:
    /**
     * Construct the Role.
     */
    virtual std::unique_ptr<dlplan::core::Role> parse_role_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const = 0;

public:
    Role(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Role.
     */
    virtual std::shared_ptr<const dlplan::core::Role> parse_role(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const {
        return caches.m_role_cache->insert(parse_role_impl(vocabulary_info, caches)).first;
    }
};

}

#endif
