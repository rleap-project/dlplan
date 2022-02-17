#include "condition.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {


std::unique_ptr<const BaseCondition> PositiveBooleanCondition::clone_impl() const {
    return std::make_unique<PositiveBooleanCondition>(*this);
}

PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool PositiveBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseCondition> NegativeBooleanCondition::clone_impl() const {
    return std::make_unique<NegativeBooleanCondition>(*this);
}

NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool NegativeBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return !get_feature()->evaluate(source_context);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseCondition> EqualNumericalCondition::clone_impl() const {
    return std::make_unique<EqualNumericalCondition>(*this);
}

EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool EqualNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseCondition> GreaterNumericalCondition::clone_impl() const {
    return std::make_unique<GreaterNumericalCondition>(*this);
}

GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool GreaterNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context) > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt " + std::to_string(get_feature()->get_index()) + ")";
}

}
