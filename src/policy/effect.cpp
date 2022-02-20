#include "effect.h"


namespace dlplan::policy {

PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool PositiveBooleanEffect::evaluate(evaluator::EvaluationContext&, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(target_context);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool NegativeBooleanEffect::evaluate(evaluator::EvaluationContext&, evaluator::EvaluationContext& target_context) const{
    return !get_feature()->evaluate(target_context);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool UnchangedBooleanEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) == get_feature()->evaluate(target_context);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot " + std::to_string(get_feature()->get_index()) + ")";
}


IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool IncrementNumericalEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) < get_feature()->evaluate(target_context);
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc " + std::to_string(get_feature()->get_index()) + ")";
}


DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool DecrementNumericalEffect::evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const{
    return get_feature()->evaluate(source_context) > get_feature()->evaluate(target_context);
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec " + std::to_string(get_feature()->get_index()) + ")";
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
