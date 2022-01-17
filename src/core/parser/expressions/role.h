#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLE_H_

#include "expression.h"

#include "../../elements/role.h"


namespace dlplan::core::parser {

class Role : public Expression {
protected:
    /**
     * Construct the Role.
     */
    virtual std::unique_ptr<element::Role> parse_role_impl(const VocabularyInfo& vocabulary, Caches &caches) const = 0;

public:
    Role(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Role.
     */
    virtual element::Role_Ptr parse_role(const VocabularyInfo& vocabulary, Caches &caches) const {
        return caches.m_role_cache->insert(parse_role_impl(vocabulary, caches)).first;
    }
};

}

#endif
