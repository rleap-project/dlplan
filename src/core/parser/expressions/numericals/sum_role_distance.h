#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../../../elements/numericals/sum_role_distance.h"

namespace dlplan::core::parser {

class SumRoleDistanceNumerical : public Numerical {
protected:
    std::unique_ptr<dlplan::core::Numerical> parse_numerical_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical_impl - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Role> role_from = m_children[0]->parse_role(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Role> role = m_children[1]->parse_role(vocabulary_info, cache);
        std::shared_ptr<const dlplan::core::Role> role_to = m_children[2]->parse_role(vocabulary_info, cache);
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical_impl - child is not of type Role, Role, Role.");
        }
        return std::make_unique<dlplan::core::SumRoleDistanceNumerical>(vocabulary_info, role_from, role, role_to);
    }

public:
    SumRoleDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }
};

}

#endif
