#include "feature_generator.h"

#include "../../include/dlp/generator.h"


namespace dlp {
namespace generator {

FeatureGeneratorImpl::FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit)
    : m_factory(factory), m_complexity(complexity), m_time_limit(time_limit) { }

FeatureCollection FeatureGeneratorImpl::generate(const States& states) const {
    /**
     * Generated features sorted by their complexity.
     */
    std::vector<std::vector<dlp::core::Concept>> concept_elements_by_complexity;
    std::vector<std::vector<dlp::core::Role>> role_elements_by_complexity;
    std::vector<std::vector<dlp::core::Numerical>> numerical_elements_by_complexity;
    std::vector<std::vector<dlp::core::Boolean>> boolean_elements_by_complexity;

    /**
     * For checking syntactic equivalence.
     */
    std::unordered_set<std::string> concept_element_cache;
    std::unordered_set<std::string> role_element_cache;
    std::unordered_set<std::string> numerical_element_cache;
    std::unordered_set<std::string> boolean_element_cache;

    /**
     * For checking sample state equivalence.
     */
    std::unordered_set<std::vector<bool>> boolean_feature_cache;
    //std::unordered_set<std::vector<int>> numerical_feature_cache;

}

}
}
