#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"
#include "../../../elements/numericals/sum_role_distance.h"

namespace dlp::core::parser {

class SumRoleDistanceNumerical : public Numerical {
protected:
    element::Numerical_Ptr parse_numerical_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical_impl - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        element::Role_Ptr role_from = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr role = m_children[1]->parse_role(vocabulary, cache);
        element::Role_Ptr role_to = m_children[2]->parse_role(vocabulary, cache);
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical_impl - child is not of type Role, Role, Role.");
        }
        return std::make_shared<element::SumRoleDistanceNumerical>(element::SumRoleDistanceNumerical(vocabulary, role_from, role, role_to));
    }

public:
    SumRoleDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }
};

}

#endif
