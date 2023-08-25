#include "condition.h"


namespace dlplan::policy {

BooleanCondition::BooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index)
    : BaseCondition(index), m_boolean(boolean) { }

int BooleanCondition::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> BooleanCondition::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const core::Numerical> BooleanCondition::get_numerical() const {
    return nullptr;
}


NumericalCondition::NumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index)
    : BaseCondition(index), m_numerical(numerical) { }

int NumericalCondition::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const core::Boolean> NumericalCondition::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const core::Numerical> NumericalCondition::get_numerical() const {
    return m_numerical;
}



PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index)
    : BooleanCondition(boolean, index) { }

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


NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index)
    : BooleanCondition(boolean, index) { }

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


EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index)
    : NumericalCondition(numerical, index) { }

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


GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index)
    : NumericalCondition(numerical, index) { }

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

}
