#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_

#include "rules/concepts/all.h"
#include "rules/concepts/and.h"
#include "rules/concepts/bot.h"
#include "rules/concepts/diff.h"
#include "rules/concepts/equal.h"
#include "rules/concepts/not.h"
#include "rules/concepts/one_of.h"
#include "rules/concepts/or.h"
#include "rules/concepts/primitive.h"
#include "rules/concepts/projection.h"
#include "rules/concepts/some.h"
#include "rules/concepts/subset.h"
#include "rules/concepts/top.h"

#include "rules/roles/and.h"
#include "rules/roles/compose.h"
#include "rules/roles/diff.h"
#include "rules/roles/identity.h"
#include "rules/roles/inverse.h"
#include "rules/roles/not.h"
#include "rules/roles/or.h"
#include "rules/roles/primitive.h"
#include "rules/roles/restrict.h"
#include "rules/roles/top.h"
#include "rules/roles/transitive_closure.h"
#include "rules/roles/transitive_reflexive_closure.h"

#include "rules/numericals/concept_distance.h"
#include "rules/numericals/count.h"

#include "rules/booleans/empty.h"
#include "rules/booleans/inclusion.h"
#include "rules/booleans/nullary.h"

#include "rules/rule.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <bitset>


namespace dlplan::generator {
struct GeneratorData;

using Rule_Ptr = std::shared_ptr<rules::Rule>;
using FeatureRepresentations = std::vector<std::string>;

class FeatureGeneratorImpl {
private:
    std::vector<Rule_Ptr> m_primitive_rules;
    /**
     * Construction rules in iteration i>=2.
     */
    std::vector<Rule_Ptr> m_concept_inductive_rules;
    std::vector<Rule_Ptr> m_role_inductive_rules;
    std::vector<Rule_Ptr> m_boolean_inductive_rules;
    std::vector<Rule_Ptr> m_numerical_inductive_rules;

    Rule_Ptr c_one_of;
    Rule_Ptr c_top;
    Rule_Ptr c_bot;
    Rule_Ptr c_primitive;
    Rule_Ptr r_top;
    Rule_Ptr r_primitive;

    Rule_Ptr b_nullary;
    Rule_Ptr b_empty;
    Rule_Ptr n_count;
    Rule_Ptr b_inclusion;
    Rule_Ptr n_concept_distance;

    Rule_Ptr c_and;
    Rule_Ptr c_or;
    Rule_Ptr c_not;
    Rule_Ptr c_diff;
    Rule_Ptr c_projection;
    Rule_Ptr c_equal;
    Rule_Ptr c_subset;
    Rule_Ptr c_some;
    Rule_Ptr c_all;

    Rule_Ptr r_and;
    Rule_Ptr r_or;
    Rule_Ptr r_not;
    Rule_Ptr r_diff;
    Rule_Ptr r_identity;
    Rule_Ptr r_inverse;
    Rule_Ptr r_restrict;
    Rule_Ptr r_compose;
    Rule_Ptr r_transitive_closure;
    Rule_Ptr r_transitive_reflexive_closure;

private:
    /**
     * Generates all Elements with complexity 1.
     */
    void generate_base(
        const core::States& states,
        GeneratorData& data,
        core::DenotationsCaches& caches);

    /**
     * Inductively generate Elements of higher complexity.
     */
    void generate_inductively(
        const core::States& states,
        int concept_complexity_limit,
        int role_complexity_limit,
        int boolean_complexity_limit,
        int count_numerical_complexity_limit,
        int distance_numerical_complexity_limit,
        GeneratorData& data,
        core::DenotationsCaches& caches);

    /**
     * Print some brief overview.
     */
    void print_statistics() const;

public:
    FeatureGeneratorImpl();
    FeatureGeneratorImpl(const FeatureGeneratorImpl& other);
    FeatureGeneratorImpl& operator=(const FeatureGeneratorImpl& other);
    FeatureGeneratorImpl(FeatureGeneratorImpl&& other);
    FeatureGeneratorImpl& operator=(FeatureGeneratorImpl&& other);
    ~FeatureGeneratorImpl();

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureRepresentations generate(
        core::SyntacticElementFactory& factory,
        const core::States& states,
        int concept_complexity_limit,
        int role_complexity_limit,
        int boolean_complexity_limit,
        int count_numerical_complexity_limit,
        int distance_numerical_complexity_limit,
        int time_limit,
        int feature_limit);

    /**
     * Set element generation on or off
     */
    void set_generate_empty_boolean(bool enable);
    void set_generate_inclusion_boolean(bool enable);
    void set_generate_nullary_boolean(bool enable);
    void set_generate_all_concept(bool enable);
    void set_generate_and_concept(bool enable);
    void set_generate_bot_concept(bool enable);
    void set_generate_diff_concept(bool enable);
    void set_generate_equal_concept(bool enable);
    void set_generate_not_concept(bool enable);
    void set_generate_one_of_concept(bool enable);
    void set_generate_or_concept(bool enable);
    void set_generate_primitive_concept(bool enable);
    void set_generate_projection_concept(bool enable);
    void set_generate_some_concept(bool enable);
    void set_generate_subset_concept(bool enable);
    void set_generate_top_concept(bool enable);
    void set_generate_concept_distance_numerical(bool enable);
    void set_generate_count_numerical(bool enable);
    void set_generate_and_role(bool enable);
    void set_generate_compose_role(bool enable);
    void set_generate_diff_role(bool enable);
    void set_generate_identity_role(bool enable);
    void set_generate_inverse_role(bool enable);
    void set_generate_not_role(bool enable);
    void set_generate_or_role(bool enable);
    void set_generate_primitive_role(bool enable);
    void set_generate_restrict_role(bool enable);
    void set_generate_top_role(bool enable);
    void set_generate_transitive_closure_role(bool enable);
    void set_generate_transitive_reflexive_closure_role(bool enable);
};

}

#endif
