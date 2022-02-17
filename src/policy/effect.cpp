#include "effect.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {

std::unique_ptr<const BaseEffect> PositiveBooleanEffect::clone_impl() const {
    return std::make_unique<const PositiveBooleanEffect>(*this);
}

PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool PositiveBooleanEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(target_context);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseEffect> NegativeBooleanEffect::clone_impl() const{
    return std::make_unique<const NegativeBooleanEffect>(*this);
}

NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool NegativeBooleanEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return !get_feature()->evaluate(target_context);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseEffect> UnchangedBooleanEffect::clone_impl() const{
    return std::make_unique<const UnchangedBooleanEffect>(*this);
}

UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool UnchangedBooleanEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) == get_feature()->evaluate(target_context);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseEffect> IncrementNumericalEffect::clone_impl() const{
    return std::make_unique<const IncrementNumericalEffect>(*this);
}

IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool IncrementNumericalEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) < get_feature()->evaluate(target_context);
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseEffect> DecrementNumericalEffect::clone_impl() const{
    return std::make_unique<const DecrementNumericalEffect>(*this);
}

DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool DecrementNumericalEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) > get_feature()->evaluate(target_context);
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec " + std::to_string(get_feature()->get_index()) + ")";
}


std::unique_ptr<const BaseEffect> UnchangedNumericalEffect::clone_impl() const{
    return std::make_unique<const UnchangedNumericalEffect>(*this);
}

UnchangedNumericalEffect::UnchangedNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool UnchangedNumericalEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) == get_feature()->evaluate(target_context);
}

std::string UnchangedNumericalEffect::compute_repr() const{
    return "(:e_n_bot " + std::to_string(get_feature()->get_index()) + ")";
}

}
