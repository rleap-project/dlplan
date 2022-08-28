#include "condition.h"


namespace dlplan::policy {

BooleanCondition::BooleanCondition(std::shared_ptr<const core::Boolean> boolean)
    : BaseCondition(boolean), m_boolean(boolean) { }


NumericalCondition::NumericalCondition(std::shared_ptr<const core::Numerical> numerical)
    : BaseCondition(numerical), m_numerical(numerical) { }


PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanCondition(boolean_feature) { }

bool PositiveBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return source_context.cache.retrieve_or_evaluate(*m_boolean, source_context);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos " + std::to_string(m_boolean->get_index()) + ")";
}


NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanCondition(boolean_feature) { }

bool NegativeBooleanCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return !source_context.cache.retrieve_or_evaluate(*m_boolean, source_context);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg " + std::to_string(m_boolean->get_index()) + ")";
}


EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalCondition(numerical_feature) { }

bool EqualNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return source_context.cache.retrieve_or_evaluate(*m_numerical, source_context) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq " + std::to_string(m_numerical->get_index()) + ")";
}


GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalCondition(numerical_feature) { }

bool GreaterNumericalCondition::evaluate(evaluator::EvaluationContext& source_context) const {
    return source_context.cache.retrieve_or_evaluate(*m_numerical, source_context) > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt " + std::to_string(m_numerical->get_index()) + ")";
}

}
