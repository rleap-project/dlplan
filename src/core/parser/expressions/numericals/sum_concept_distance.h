#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_SUM_CONCEPT_DISTANCE_H_

#include "../numerical.h"
#include "../../../elements/numericals/sum_concept_distance.h"


namespace dlplan::core::parser {

class SumConceptDistanceNumerical : public Numerical {
protected:
    std::unique_ptr<element::Numerical> parse_numerical_impl(const VocabularyInfo& vocabulary, Caches &cache) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("SumConceptDistanceNumerical::parse_numerical_impl - number of children ("s + std::to_string(m_children.size()) + " != 3).");
        }
        // 1. Parse children
        element::Concept_Ptr concept_from = m_children[0]->parse_concept(vocabulary, cache);
        element::Role_Ptr role = m_children[1]->parse_role(vocabulary, cache);
        element::Concept_Ptr concept_to = m_children[2]->parse_concept(vocabulary, cache);
        if (!(concept_from && role && concept_to)) {
            throw std::runtime_error("SumConceptDistanceNumerical::parse_numerical_impl - child is not of type Concept, Role, Concept.");
        }
        return std::make_unique<element::SumConceptDistanceNumerical>(vocabulary, concept_from, role, concept_to);
    }

public:
    SumConceptDistanceNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }
};

}

#endif
