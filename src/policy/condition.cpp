#include "condition.h"


namespace dlplan::policy {

BooleanCondition::BooleanCondition(std::shared_ptr<const core::Boolean> boolean)
    : BaseCondition(boolean), m_boolean(boolean) { }


NumericalCondition::NumericalCondition(std::shared_ptr<const core::Numerical> numerical)
    : BaseCondition(numerical), m_numerical(numerical) { }


PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanCondition(boolean_feature) { }

bool PositiveBooleanCondition::evaluate(const core::State& source_state) const {
    return m_boolean->evaluate(source_state);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_boolean->evaluate(source_state, caches);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos \"" + m_boolean->compute_repr() + "\")";
}

std::string PositiveBooleanCondition::str() const {
    return "(:c_b_pos " + std::to_string(m_boolean->get_index()) + ")";
}

std::shared_ptr<const BaseCondition> PositiveBooleanCondition::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_pos_condition(policy_builder.add_boolean_feature(*m_boolean));
}


NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanCondition(boolean_feature) { }

bool NegativeBooleanCondition::evaluate(const core::State& source_state) const {
    return !m_boolean->evaluate(source_state);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return !m_boolean->evaluate(source_state, caches);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg \"" + m_boolean->compute_repr() + "\")";
}

std::string NegativeBooleanCondition::str() const {
    return "(:c_b_neg " + std::to_string(m_boolean->get_index()) + ")";
}

std::shared_ptr<const BaseCondition> NegativeBooleanCondition::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_neg_condition(policy_builder.add_boolean_feature(*m_boolean));
}


EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalCondition(numerical_feature) { }

bool EqualNumericalCondition::evaluate(const core::State& source_state) const {
    return m_numerical->evaluate(source_state) == 0;
}

bool EqualNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq \"" + m_numerical->compute_repr() + "\")";
}

std::string EqualNumericalCondition::str() const {
    return "(:c_n_eq " + std::to_string(m_numerical->get_index()) + ")";
}

std::shared_ptr<const BaseCondition> EqualNumericalCondition::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_eq_condition(policy_builder.add_numerical_feature(*m_numerical));
}


GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalCondition(numerical_feature) { }

bool GreaterNumericalCondition::evaluate(const core::State& source_state) const {
    return m_numerical->evaluate(source_state) > 0;
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt \"" + m_numerical->compute_repr() + "\")";
}

std::string GreaterNumericalCondition::str() const {
    return "(:c_n_gt " + std::to_string(m_numerical->get_index()) + ")";
}

std::shared_ptr<const BaseCondition> GreaterNumericalCondition::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_gt_condition(policy_builder.add_numerical_feature(*m_numerical));
}

}
