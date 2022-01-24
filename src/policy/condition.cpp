#include "condition.h"


#include "evaluation_cache.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {


std::unique_ptr<BaseCondition> PositiveBooleanCondition::clone_impl() const {
    return std::make_unique<PositiveBooleanCondition>(*this);
}

PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool PositiveBooleanCondition::evaluate(const State& state, EvaluationCaches& evaluation_caches) const {
    return get_feature()->evaluate(state, evaluation_caches);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseCondition> NegativeBooleanCondition::clone_impl() const {
    return std::make_unique<NegativeBooleanCondition>(*this);
}

NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool NegativeBooleanCondition::evaluate(const State& state, EvaluationCaches& evaluation_caches) const {
    return !get_feature()->evaluate(state, evaluation_caches);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseCondition> EqualNumericalCondition::clone_impl() const {
    return std::make_unique<EqualNumericalCondition>(*this);
}

EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool EqualNumericalCondition::evaluate(const State& state, EvaluationCaches& evaluation_caches) const {
    return get_feature()->evaluate(state, evaluation_caches) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<BaseCondition> GreaterNumericalCondition::clone_impl() const {
    return std::make_unique<GreaterNumericalCondition>(*this);
}

GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool GreaterNumericalCondition::evaluate(const State& state, EvaluationCaches& evaluation_caches) const {
    return get_feature()->evaluate(state, evaluation_caches) > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt " + std::to_string(get_feature()->get_index()) + ")";
}

}
