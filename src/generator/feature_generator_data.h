#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_DATA_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_DATA_H_

#include <memory>

#include "hash_tables/hash_table.h"
#include "types.h"

#include "../../include/dlplan/core.h"
#include "../utils/countdown_timer.h"


namespace dlplan {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}
namespace generator {

/**
 * Stores byproducts and the result of the feature generation.
 */
class FeatureGeneratorData {
private:
    const std::shared_ptr<core::SyntacticElementFactory> m_factory;
    const int m_complexity;
    const int m_time_limit;
    const int m_feature_limit;

    /**
     * Elements by iteration.
     */
    std::vector<std::vector<dlplan::core::Concept>> m_concept_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Role>> m_role_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Numerical>> m_numerical_elements_by_complexity;
    std::vector<std::vector<dlplan::core::Boolean>> m_boolean_elements_by_complexity;

    /**
     * For uniqueness checking
     */
    std::unique_ptr<HashTable> m_hash_table;

    /**
     * Terminate generation if timer expired.
     */
    utils::CountdownTimer m_timer;

    /**
     * Some statistics.
     */
    int m_count_features = 0;

    /**
     * Store the result.
     */
    FeatureRepresentations m_feature_reprs;

public:
    FeatureGeneratorData(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit, int feature_limit);

    /**
     * Returns true iff the limiting constraints are satisfied, i.e.,
     * time limit or feature limit reached
     */
    bool reached_limit() const;

    /**
     * Adds the respective element to the result if it is syntactically and empirically unique.
     */
    bool add_concept(const States& states, core::Concept&& concept);
    bool add_role(const States& states, core::Role&& role);
    bool add_numerical(const States& states, core::Numerical&& numerical);
    bool add_boolean(const States& states, core::Boolean&& boolean);

    void print_statistics() const;
    void print_overall_statistics() const;

    /**
     * Getters
     */
    core::SyntacticElementFactory& get_factory() const;
    const std::vector<std::vector<dlplan::core::Concept>>& get_concept_elements_by_complexity() const;
    const std::vector<std::vector<dlplan::core::Role>>& get_role_elements_by_complexity() const;
    const std::vector<std::vector<dlplan::core::Numerical>>& get_numerical_elements_by_complexity() const;
    const std::vector<std::vector<dlplan::core::Boolean>>& get_boolean_elements_by_complexity() const;
    FeatureRepresentations get_feature_reprs() const;
};

}
}

#endif
