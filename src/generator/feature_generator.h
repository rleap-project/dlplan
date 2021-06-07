#ifndef DLP_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_
#define DLP_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_

#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "../../include/dlp/core.h"

#include "types.h"


namespace dlp {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
    class SyntacticElementFactory;
}
namespace generator {
class FeatureCollection;

/**
 * https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
 */
struct HashNumericalDenotation {
    std::size_t operator()(std::vector<int> const& numerical_denotation) const {
        std::size_t seed = numerical_denotation.size();
        for(const auto & i : numerical_denotation) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
struct HashConceptDenotation {
    std::size_t operator()(std::vector<std::vector<int>> const& concept_denotation) const {
        std::size_t seed = concept_denotation.size();
        for (const auto& v : concept_denotation) {
            for(const auto& i : v) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
        }
        return seed;
    }
};
struct HashRoleDenotation {
    std::size_t operator()(std::vector<std::vector<std::pair<int,int>>> const& role_denotation) const {
        std::size_t seed = role_denotation.size();
        for (const auto& v : role_denotation) {
            for(const auto& i : v) {
                seed ^= i.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= i.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
        }
        return seed;
    }
};

class FeatureGeneratorImpl {
private:
    const std::shared_ptr<core::SyntacticElementFactory> m_factory;
    const int m_complexity;
    const int m_time_limit;

    /**
     * Generated features sorted by their complexity.
     */
    std::vector<std::vector<dlp::core::Concept>> m_concept_elements_by_complexity;
    std::vector<std::vector<dlp::core::Role>> m_role_elements_by_complexity;
    std::vector<std::vector<dlp::core::Numerical>> m_numerical_elements_by_complexity;
    std::vector<std::vector<dlp::core::Boolean>> m_boolean_elements_by_complexity;

    /**
     * For checking syntactic equivalence.
     */
    std::unordered_set<std::string> m_concept_element_cache;
    std::unordered_set<std::string> m_role_element_cache;
    std::unordered_set<std::string> m_numerical_element_cache;
    std::unordered_set<std::string> m_boolean_element_cache;

    /**
     * For checking sample state equivalence.
     */
    std::unordered_set<std::vector<bool>> m_boolean_denotation_cache;
    std::unordered_set<std::vector<int>, HashNumericalDenotation> m_numerical_denotation_cache;
    std::unordered_set<std::vector<core::ConceptDenotation>, HashConceptDenotation> m_concept_denotation_cache;
    std::unordered_set<std::vector<core::RoleDenotation>, HashRoleDenotation> m_role_denotation_cache;

private:
    /**
     * Generates all Elements with complexity 1.
     */
    void generate_base(const States& states);

    void generate_primitive_concepts(const States& states);
    void generate_primitive_roles(const States& states);
    void generate_top_role();
    void generate_bot_concept();
    void generate_top_concept();

    /**
     * Inductively generate Elements of higher complexity.
     */
    void generate_inductively(const States& states);

    void generate_empty_boolean(const States& states, int iteration);
    void generator_all_concept(const States& states, int iteration);
    void generator_and_concept(const States& states, int iteration);
    void generator_diff_concept(const States& states, int iteration);
    void generator_not_concept(const States& states, int iteration);
    void generator_one_of_concept(const States& states, int iteration);
    void generator_or_concept(const States& states, int iteration);
    void generator_some_concept(const States& states, int iteration);
    void generator_subset_concept(const States& states, int iteration);

    void generator_concept_distance_numerical(const States& states, int iteration);
    void generator_count_numerical(const States& states, int iteration);
    void generator_role_distance_numerical(const States& states, int iteration);
    void generator_sum_concept_distance_numerical(const States& states, int iteration);
    void generator_sum_role_distance_numerical(const States& states, int iteration);

    void generate_and_role(const States& states, int iteration);
    void generate_compose_role(const States& states, int iteration);
    void generate_diff_role(const States& states, int iteration);
    void generate_identity_role(const States& states, int iteration);
    void generate_inverse_role(const States& states, int iteration);
    void generate_not_role(const States& states, int iteration);
    void generate_or_role(const States& states, int iteration);
    void generate_restrict_role(const States& states, int iteration);
    void generate_transitive_closure_role(const States& states, int iteration);
    void generate_transitive_reflexive_closure_role(const States& states, int iteration);

public:
    FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit);

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureCollection generate(const States& states);
};

}
}


#endif
