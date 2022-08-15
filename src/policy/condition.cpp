#include "condition.h"


namespace dlplan::policy {

PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const BooleanFeature> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool PositiveBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos " + std::to_string(get_feature()->get_index()) + ")";
}


NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const BooleanFeature> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool NegativeBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return !get_feature()->evaluate(source_context);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg " + std::to_string(get_feature()->get_index()) + ")";
}


EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const NumericalFeature> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool EqualNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq " + std::to_string(get_feature()->get_index()) + ")";
}


GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const NumericalFeature> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool GreaterNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return get_feature()->evaluate(source_context) > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt " + std::to_string(get_feature()->get_index()) + ")";
}

}
