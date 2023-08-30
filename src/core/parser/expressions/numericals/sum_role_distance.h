#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class SumRoleDistanceNumerical : public Numerical {
private:
    static inline const std::string m_name = "n_sum_role_distance";

public:
    SumRoleDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Numerical> parse_numerical(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        auto role_from = m_children[0]->parse_role(factory);
        auto role = m_children[1]->parse_role(factory);
        auto role_to = m_children[2]->parse_role(factory);
        if (!(role_from && role && role_to)) {
            throw std::runtime_error("SumRoleDistanceNumerical::parse_numerical - child is not of type Role, Role, Role.");
        }
        return factory.make_sum_role_distance_numerical(role_from, role, role_to);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
