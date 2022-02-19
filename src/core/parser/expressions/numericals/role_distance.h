#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../../../elements/numericals/role_distance.h"

namespace dlplan::core::parser {

class RoleDistanceNumerical : public Numerical {
protected:
    std::unique_ptr<element::Numerical> parse_numerical_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("RoleDistanceNumerical::parse_numerical_impl - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        element::Role_Ptr role_from = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr role = m_children[1]->parse_role(vocabulary, cache);
        element::Role_Ptr role_to = m_children[2]->parse_role(vocabulary, cache);
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("RoleDistanceNumerical::parse_numerical_impl - child is not of type Role, Role, Role.");
        }
        return std::make_unique<element::RoleDistanceNumerical>(vocabulary, role_from, role, role_to);
    }

public:
    RoleDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }
};

}

#endif
