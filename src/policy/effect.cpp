#include "effect.h"


namespace dlplan::policy {

BooleanEffect::BooleanEffect(std::shared_ptr<const core::Boolean> boolean)
    : BaseEffect(), m_boolean(boolean) { }

int BooleanEffect::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> BooleanEffect::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const core::Numerical> BooleanEffect::get_numerical() const {
    return nullptr;
}


NumericalEffect::NumericalEffect(std::shared_ptr<const core::Numerical> numerical)
    : BaseEffect(), m_numerical(numerical) { }

int NumericalEffect::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> NumericalEffect::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const core::Numerical> NumericalEffect::get_numerical() const {
    return m_numerical;
}


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

}
