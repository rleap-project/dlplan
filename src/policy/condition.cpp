#include "../../include/dlplan/policy/condition.h"

#include "../../include/dlplan/core/elements/utils.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

using namespace dlplan;


namespace dlplan::policy {


PositiveBooleanCondition::PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : NamedElementCondition<NamedBoolean>(identifier, boolean) { }

bool PositiveBooleanCondition::are_equal_impl(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const PositiveBooleanCondition&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t PositiveBooleanCondition::hash_impl() const {
    return hash_combine(m_named_element);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state) const {
    return m_named_element->get_element()->evaluate(source_state);
}

bool PositiveBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return m_named_element->get_element()->evaluate(source_state, caches);
}

void PositiveBooleanCondition::str_impl(std::stringstream& out) const {
    out << "(:c_b_pos " + m_named_element->get_key() + ")";
}

void PositiveBooleanCondition::accept(BaseConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


NegativeBooleanCondition::NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : NamedElementCondition<NamedBoolean>(identifier, boolean) { }

bool NegativeBooleanCondition::are_equal_impl(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const NegativeBooleanCondition&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t NegativeBooleanCondition::hash_impl() const {
    return hash_combine(m_named_element);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state) const {
    return !m_named_element->get_element()->evaluate(source_state);
}

bool NegativeBooleanCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    return !m_named_element->get_element()->evaluate(source_state, caches);
}

void NegativeBooleanCondition::str_impl(std::stringstream& out) const {
    out << "(:c_b_neg " + m_named_element->get_key() + ")";
}

void NegativeBooleanCondition::accept(BaseConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


EqualNumericalCondition::EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementCondition<NamedNumerical>(identifier, numerical) { }

bool EqualNumericalCondition::are_equal_impl(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const EqualNumericalCondition&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t EqualNumericalCondition::hash_impl() const {
    return hash_combine(m_named_element);
}

bool EqualNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_named_element->get_element()->evaluate(source_state);
    if (eval == INF) return false;
    return eval == 0;
}

bool EqualNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_named_element->get_element()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval == 0;
}

void EqualNumericalCondition::str_impl(std::stringstream& out) const {
    out << "(:c_n_eq " + m_named_element->get_key() + ")";
}

void EqualNumericalCondition::accept(BaseConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


GreaterNumericalCondition::GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementCondition<NamedNumerical>(identifier, numerical) { }

bool GreaterNumericalCondition::are_equal_impl(const BaseCondition& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const GreaterNumericalCondition&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t GreaterNumericalCondition::hash_impl() const {
    return hash_combine(m_named_element);
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state) const {
    int eval = m_named_element->get_element()->evaluate(source_state);
    if (eval == INF) return false;
    return eval > 0;
}

bool GreaterNumericalCondition::evaluate(const core::State& source_state, core::DenotationsCaches& caches) const {
    int eval = m_named_element->get_element()->evaluate(source_state, caches);
    if (eval == INF) return false;
    return eval > 0;
}

void GreaterNumericalCondition::str_impl(std::stringstream& out) const {
    out << "(:c_n_gt " + m_named_element->get_key() + ")";
}

void GreaterNumericalCondition::accept(BaseConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
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