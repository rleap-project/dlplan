#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../../../elements/numericals/count.h"

namespace dlplan::core::parser {

class CountNumerical : public Numerical {
protected:
    std::unique_ptr<dlplan::core::Numerical> parse_numerical_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("CountNumerical::parse_numerical_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        std::shared_ptr<const dlplan::core::Concept> concept = m_children[0]->parse_concept(vocabulary_info, cache);
        if (concept) {
            return std::make_unique<dlplan::core::CountNumerical<dlplan::core::Concept>>(vocabulary_info, concept);
        }
        std::shared_ptr<const dlplan::core::Role> role = m_children[0]->parse_role(vocabulary_info, cache);
        if (role) {
            return std::make_unique<dlplan::core::CountNumerical<dlplan::core::Role>>(vocabulary_info, role);
        }
        // 2. Construct element
        throw std::runtime_error("CountNumerical::parse_numerical_impl - unable to construct children elements.");
    }

public:
    CountNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }
};

}

#endif
