#include "feature_generator.h"

#include "../../include/dlp/generator.h"


namespace dlp {
namespace generator {

struct HashVector {
    std::size_t operator()(std::vector<int> const& vec) const {
        std::size_t seed = vec.size();
        for(auto& i : vec) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

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
    std::unordered_set<std::vector<int>, HashVector> numerical_feature_cache;

    FeatureCollection feature_collection;

    generate_base(states);
    generate_inductively(states);

    return feature_collection;
}


void FeatureGeneratorImpl::generate_base(const States& states) const {
    // 4. Construct base
    /*for (const auto& predicate: predicates) {
        // 4.1. PrimitiveConcept
        for (unsigned pos = 0; pos < predicate.get_arity(); ++pos) {
            dlp::core::Concept concept_element = factory.make_primitive_concept(predicate.get_name(), pos);
            bool unique = concept_element_cache.insert(concept_element.compute_repr()).second;
            if (unique) concept_elements_by_complexity[0].emplace_back(concept_element);
        }
        // 4.2. PrimitiveRole
        for (unsigned pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
            for (unsigned pos2 = 0; pos2 < predicate.get_arity(); ++pos2) {
                // TODO(dominik): add roles
                dlp::core::Role role_element = factory.make_primitive_role(predicate.get_name(), pos1, pos2);
                bool unique = role_element_cache.insert(role_element.compute_repr()).second;
                if (unique) role_elements_by_complexity[0].emplace_back(role_element);

            }
        }
        // TODO(dominik): Add other complexity 1 elements, e.g. top, bot,..
    }*/
}


void FeatureGeneratorImpl::generate_inductively(const States& states) const {
}

}
}
