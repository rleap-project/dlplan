#include "../../include/dlplan/policy/condition.h"

#include "../../include/dlplan/core/elements/utils.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

using namespace dlplan;


namespace dlplan::policy {

BooleanCondition::BooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BaseCondition(identifier), m_boolean(boolean) { }

int BooleanCondition::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> BooleanCondition::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const NamedNumerical> BooleanCondition::get_numerical() const {
    return nullptr;
}


NumericalCondition::NumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : BaseCondition(identifier), m_numerical(numerical) { }

int NumericalCondition::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> NumericalCondition::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const NamedNumerical> NumericalCondition::get_numerical() const {
    return m_numerical;
}


PositiveBooleanCondition::PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanCondition(identifier, boolean) { }

bool PositiveBooleanCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const PositiveBooleanCondition&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t PositiveBooleanCondition::hash() const {
    return hash_combine(m_boolean);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state) const {
    return m_boolean->get_boolean()->evaluate(source_state);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_boolean->get_boolean()->evaluate(source_state, caches);
}

std::string PositiveBooleanCondition::compute_repr() const {
    return "(:c_b_pos \"" + m_boolean->get_boolean()->str() + "\")";
}

std::string PositiveBooleanCondition::str() const {
    return "(:c_b_pos " + m_boolean->get_key() + ")";
}


NegativeBooleanCondition::NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanCondition(identifier, boolean) { }

bool NegativeBooleanCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const NegativeBooleanCondition&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t NegativeBooleanCondition::hash() const {
    return hash_combine(m_boolean);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state) const {
    return !m_boolean->get_boolean()->evaluate(source_state);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return !m_boolean->get_boolean()->evaluate(source_state, caches);
}

std::string NegativeBooleanCondition::compute_repr() const {
    return "(:c_b_neg \"" + m_boolean->get_boolean()->str() + "\")";
}

std::string NegativeBooleanCondition::str() const {
    return "(:c_b_neg " + m_boolean->get_key() + ")";
}


EqualNumericalCondition::EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalCondition(identifier, numerical) { }

bool EqualNumericalCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const EqualNumericalCondition&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t EqualNumericalCondition::hash() const {
    return hash_combine(m_numerical);
}

bool EqualNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state);
    if (eval == INF) return false;
    return eval == 0;
}

bool EqualNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval == 0;
}

std::string EqualNumericalCondition::compute_repr() const {
    return "(:c_n_eq \"" + m_numerical->get_numerical()->str() + "\")";
}

std::string EqualNumericalCondition::str() const {
    return "(:c_n_eq " + m_numerical->get_key() + ")";
}


GreaterNumericalCondition::GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalCondition(identifier, numerical) { }

bool GreaterNumericalCondition::operator==(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const GreaterNumericalCondition&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t GreaterNumericalCondition::hash() const {
    return hash_combine(m_numerical);
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state);
    if (eval == INF) return false;
    return eval > 0;
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval > 0;
}

std::string GreaterNumericalCondition::compute_repr() const {
    return "(:c_n_gt \"" + m_numerical->get_numerical()->str() + "\")";
}

std::string GreaterNumericalCondition::str() const {
    return "(:c_n_gt " + m_numerical->get_key() + ")";
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>>::operator()(
        const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& left_condition,
        const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& right_condition) const {
        return *left_condition < *right_condition;
    }

    bool less<std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>>::operator()(
        const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& left_condition,
        const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& right_condition) const {
        return *left_condition < *right_condition;
    }

    bool less<std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>>::operator()(
        const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& left_condition,
        const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& right_condition) const {
        return *left_condition < *right_condition;
    }

    bool less<std::shared_ptr<const dlplan::policy::EqualNumericalCondition>>::operator()(
        const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& left_condition,
        const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& right_condition) const {
        return *left_condition < *right_condition;
    }

    std::size_t hash<dlplan::policy::PositiveBooleanCondition>::operator()(
        const dlplan::policy::PositiveBooleanCondition& condition) const {
        return condition.hash();
    }

    std::size_t hash<dlplan::policy::NegativeBooleanCondition>::operator()(
        const dlplan::policy::NegativeBooleanCondition& condition) const {
        return condition.hash();
    }

    std::size_t hash<dlplan::policy::GreaterNumericalCondition>::operator()(
        const dlplan::policy::GreaterNumericalCondition& condition) const {
        return condition.hash();
    }

    std::size_t hash<dlplan::policy::EqualNumericalCondition>::operator()(
        const dlplan::policy::EqualNumericalCondition& condition) const {
        return condition.hash();
    }
}