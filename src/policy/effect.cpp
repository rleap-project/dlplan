#include "effect.h"


namespace dlplan::policy {

BooleanEffect::BooleanEffect(std::shared_ptr<const core::Boolean> boolean)
    : BaseEffect(boolean), m_boolean(boolean) { }


NumericalEffect::NumericalEffect(std::shared_ptr<const core::Numerical> numerical)
    : BaseEffect(numerical), m_numerical(numerical) { }


PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanEffect(boolean_feature) {}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return m_boolean->evaluate(target_state);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->evaluate(target_state, caches);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos \"" + m_boolean->compute_repr() + "\")";
}

std::string PositiveBooleanEffect::str() const {
    return "(:e_b_pos " + std::to_string(m_boolean->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> PositiveBooleanEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_pos_effect(policy_builder.add_boolean_feature(*m_boolean));
}


NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanEffect(boolean_feature) {}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return !m_boolean->evaluate(target_state);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return !m_boolean->evaluate(target_state, caches);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg \"" + m_boolean->compute_repr() + "\")";
}

std::string NegativeBooleanEffect::str() const {
    return "(:e_b_neg " + std::to_string(m_boolean->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> NegativeBooleanEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_neg_effect(policy_builder.add_boolean_feature(*m_boolean));
}


UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature)
    : BooleanEffect(boolean_feature) {}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_boolean->evaluate(source_state) == m_boolean->evaluate(target_state);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->evaluate(source_state, caches) == m_boolean->evaluate(target_state, caches);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot \"" + m_boolean->compute_repr() + "\")";
}

std::string UnchangedBooleanEffect::str() const {
    return "(:e_b_bot " + std::to_string(m_boolean->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> UnchangedBooleanEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_bot_effect(policy_builder.add_boolean_feature(*m_boolean));
}


IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalEffect(numerical_feature) {}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) < m_numerical->evaluate(target_state);
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) < m_numerical->evaluate(target_state, caches);
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc \"" + m_numerical->compute_repr() + "\")";
}

std::string IncrementNumericalEffect::str() const {
    return "(:e_n_inc " + std::to_string(m_numerical->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> IncrementNumericalEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_inc_effect(policy_builder.add_numerical_feature(*m_numerical));
}


DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalEffect(numerical_feature) {}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) > m_numerical->evaluate(target_state);
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) > m_numerical->evaluate(target_state, caches);
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec \"" + m_numerical->compute_repr() + "\")";
}

std::string DecrementNumericalEffect::str() const {
    return "(:e_n_dec " + std::to_string(m_numerical->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> DecrementNumericalEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_dec_effect(policy_builder.add_numerical_feature(*m_numerical));
}


UnchangedNumericalEffect::UnchangedNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature)
    : NumericalEffect(numerical_feature) {}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->evaluate(source_state) == m_numerical->evaluate(target_state);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_numerical->evaluate(source_state, caches) == m_numerical->evaluate(target_state, caches);
}

std::string UnchangedNumericalEffect::compute_repr() const{
    return "(:e_n_bot \"" + m_numerical->compute_repr() + "\")";
}

std::string UnchangedNumericalEffect::str() const {
    return "(:e_n_bot " + std::to_string(m_numerical->get_index()) + ")";
}

std::shared_ptr<const BaseEffect> UnchangedNumericalEffect::copy_to_builder(PolicyBuilder& policy_builder) const {
    return policy_builder.add_bot_effect(policy_builder.add_numerical_feature(*m_numerical));
}

}
