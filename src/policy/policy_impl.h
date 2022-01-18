#ifndef DLPLAN_SRC_POLICY_POLICY_IMPL_H_
#define DLPLAN_SRC_POLICY_POLICY_IMPL_H_

#include <unordered_set>
#include <vector>
#include <memory>

#include "cache.h"


namespace dlplan::policy {
class Rule;
template<typename T>
class Feature;
class PolicyRoot;


class PolicyImpl {
private:
    const std::shared_ptr<const PolicyRoot> m_root;

    Caches m_caches;

    std::vector<std::shared_ptr<Feature<bool>>> m_boolean_features;
    std::vector<std::shared_ptr<Feature<int>>> m_numerical_features;
    std::vector<std::shared_ptr<Rule>> m_rules;

public:
    PolicyImpl();

    std::shared_ptr<Feature<bool>> add_boolean_feature();
    std::shared_ptr<Feature<int>> add_numerical_feature();

    std::shared_ptr<const BaseCondition> add_b_pos_condition(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseCondition> add_b_neg_condition(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseCondition> add_n_gt_condition(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseCondition> add_n_eq_condition(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_b_pos_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_b_neg_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_b_bot_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_n_inc_effect(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_n_dec_effect(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_n_bot_effect(std::shared_ptr<const Feature<int>> n);

    std::shared_ptr<const Rule> add_rule(
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);

    void reset_cached_source_evaluations();
    void reset_cached_target_evaluations();

    bool evaluate_lazy(const core::State& source, const core::State& target);

    std::string str() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<Feature<bool>>> get_boolean_features() const;
    std::vector<std::shared_ptr<Feature<int>>> get_numerical_features() const;
};

}

#endif
