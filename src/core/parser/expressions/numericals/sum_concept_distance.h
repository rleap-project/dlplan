#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class SumConceptDistanceNumerical : public Numerical {
private:
    static inline const std::string m_name = "n_sum_concept_distance";

public:
    SumConceptDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Numerical> parse_numerical(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("SumConceptDistanceNumerical::parse_numerical - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        auto concept_from = m_children[0]->parse_concept(factory);
        auto role = m_children[1]->parse_role(factory);
        auto concept_to = m_children[2]->parse_concept(factory);
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("SumConceptDistanceNumerical::parse_numerical - child is not of type Concept, Role, Concept.");
        }
        return factory.make_sum_concept_distance_numerical(concept_from, role, concept_to);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
