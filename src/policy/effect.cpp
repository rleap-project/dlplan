#include "../../include/dlplan/policy/effect.h"

#include "../../include/dlplan/core/elements/utils.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"


using namespace dlplan;


namespace dlplan::policy {


PositiveBooleanEffect::PositiveBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : NamedElementEffect<NamedBoolean>(identifier, boolean) {}

bool PositiveBooleanEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const PositiveBooleanEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t PositiveBooleanEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return m_named_element->get_element()->evaluate(target_state);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_named_element->get_element()->evaluate(target_state, caches);
}

void PositiveBooleanEffect::str_impl(std::stringstream& out) const {
    out << "(:e_b_pos " + m_named_element->get_key() + ")";
}

void PositiveBooleanEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


NegativeBooleanEffect::NegativeBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : NamedElementEffect<NamedBoolean>(identifier, boolean) {}

bool NegativeBooleanEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const NegativeBooleanEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t NegativeBooleanEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return !m_named_element->get_element()->evaluate(target_state);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return !m_named_element->get_element()->evaluate(target_state, caches);
}

void NegativeBooleanEffect::str_impl(std::stringstream& out) const {
    out << "(:e_b_neg " + m_named_element->get_key() + ")";
}

void NegativeBooleanEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


UnchangedBooleanEffect::UnchangedBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : NamedElementEffect<NamedBoolean>(identifier, boolean) {}

bool UnchangedBooleanEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const UnchangedBooleanEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t UnchangedBooleanEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_named_element->get_element()->evaluate(source_state) == m_named_element->get_element()->evaluate(target_state);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_named_element->get_element()->evaluate(source_state, caches) == m_named_element->get_element()->evaluate(target_state, caches);
}

void UnchangedBooleanEffect::str_impl(std::stringstream& out) const {
    out << "(:e_b_bot " + m_named_element->get_key() + ")";
}

void UnchangedBooleanEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


IncrementNumericalEffect::IncrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool IncrementNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const IncrementNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t IncrementNumericalEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval < target_eval;
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval < target_eval;
}

void IncrementNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_inc " + m_named_element->get_key() + ")";
}

void IncrementNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


DecrementNumericalEffect::DecrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool DecrementNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const DecrementNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t DecrementNumericalEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval > target_eval;
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval > target_eval;
}

void DecrementNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_dec " + m_named_element->get_key() + ")";
}

void DecrementNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


UnchangedNumericalEffect::UnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool UnchangedNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const UnchangedNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

size_t UnchangedNumericalEffect::hash_impl() const {
    return hash_combine(m_named_element);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_named_element->get_element()->evaluate(source_state) == m_named_element->get_element()->evaluate(target_state);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    return source_eval == target_eval;
}

void UnchangedNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_bot " + m_named_element->get_key() + ")";
}

void UnchangedNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

}


namespace std {
        bool less<std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }


        bool less<std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        std::size_t hash<dlplan::policy::PositiveBooleanEffect>::operator()(
            const dlplan::policy::PositiveBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::NegativeBooleanEffect>::operator()(
            const dlplan::policy::NegativeBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::UnchangedBooleanEffect>::operator()(
            const dlplan::policy::UnchangedBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::IncrementNumericalEffect>::operator()(
            const dlplan::policy::IncrementNumericalEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::DecrementNumericalEffect>::operator()(
            const dlplan::policy::DecrementNumericalEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::UnchangedNumericalEffect>::operator()(
            const dlplan::policy::UnchangedNumericalEffect& effect) const {
        return effect.hash();
    }
}
