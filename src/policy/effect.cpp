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

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval < target_eval;
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval < target_eval;
}

void IncrementNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_inc " + m_named_element->get_key() + ")";
}

void IncrementNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}



IncrementOrUnchangedNumericalEffect::IncrementOrUnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool IncrementOrUnchangedNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const IncrementOrUnchangedNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool IncrementOrUnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval <= target_eval;
}

bool IncrementOrUnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval <= target_eval;
}

void IncrementOrUnchangedNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_inc_bot " + m_named_element->get_key() + ")";
}

void IncrementOrUnchangedNumericalEffect::accept(BaseEffectVisitor& visitor) const {
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

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval > target_eval;
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval > target_eval;
}

void DecrementNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_dec " + m_named_element->get_key() + ")";
}

void DecrementNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


DecrementOrUnchangedNumericalEffect::DecrementOrUnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool DecrementOrUnchangedNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const DecrementOrUnchangedNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool DecrementOrUnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state);
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval >= target_eval;
}

bool DecrementOrUnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval >= target_eval;
}

void DecrementOrUnchangedNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_dec_bot " + m_named_element->get_key() + ")";
}

void DecrementOrUnchangedNumericalEffect::accept(BaseEffectVisitor& visitor) const {
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

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_named_element->get_element()->evaluate(source_state) == m_named_element->get_element()->evaluate(target_state);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches);
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval == target_eval;
}

void UnchangedNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_bot " + m_named_element->get_key() + ")";
}

void UnchangedNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


GreaterNumericalEffect::GreaterNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool GreaterNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const GreaterNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool GreaterNumericalEffect::evaluate(const core::State&, const core::State& target_state) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (target_eval == INF) return false;
    return target_eval > 0;
}

bool GreaterNumericalEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (target_eval == INF) return false;
    return target_eval > 0;
}

void GreaterNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_gt " + m_named_element->get_key() + ")";
}

void GreaterNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


EqualNumericalEffect::EqualNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NamedElementEffect<NamedNumerical>(identifier, numerical) {}

bool EqualNumericalEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const EqualNumericalEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool EqualNumericalEffect::evaluate(const core::State&, const core::State& target_state) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state);
    //if (target_eval == INF) return false;
    return target_eval == 0;
}

bool EqualNumericalEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches);
    //if (target_eval == INF) return false;
    return target_eval == 0;
}

void EqualNumericalEffect::str_impl(std::stringstream& out) const {
    out << "(:e_n_eq " + m_named_element->get_key() + ")";
}

void EqualNumericalEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


IncrementConceptEffect::IncrementConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_)
    : NamedElementEffect<NamedConcept>(identifier, concept_) {}

bool IncrementConceptEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const IncrementConceptEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool IncrementConceptEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state).size();
    int target_eval = m_named_element->get_element()->evaluate(target_state).size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval < target_eval;
}

bool IncrementConceptEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches)->size();
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches)->size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval < target_eval;
}

void IncrementConceptEffect::str_impl(std::stringstream& out) const {
    out << "(:e_c_inc " + m_named_element->get_key() + ")";
}

void IncrementConceptEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


DecrementConceptEffect::DecrementConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_)
    : NamedElementEffect<NamedConcept>(identifier, concept_) {}

bool DecrementConceptEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const DecrementConceptEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool DecrementConceptEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state).size();
    int target_eval = m_named_element->get_element()->evaluate(target_state).size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval > target_eval;
}

bool DecrementConceptEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches)->size();
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches)->size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval > target_eval;
}

void DecrementConceptEffect::str_impl(std::stringstream& out) const {
    out << "(:e_c_dec " + m_named_element->get_key() + ")";
}

void DecrementConceptEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


UnchangedConceptEffect::UnchangedConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_)
    : NamedElementEffect<NamedConcept>(identifier, concept_) {}

bool UnchangedConceptEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const UnchangedConceptEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool UnchangedConceptEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state).size();
    int target_eval = m_named_element->get_element()->evaluate(target_state).size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval == target_eval;
}

bool UnchangedConceptEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_named_element->get_element()->evaluate(source_state, caches)->size();
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches)->size();
    //if (source_eval == INF) return false;
    //if (target_eval == INF) return false;
    return source_eval == target_eval;
}

void UnchangedConceptEffect::str_impl(std::stringstream& out) const {
    out << "(:e_c_bot " + m_named_element->get_key() + ")";
}

void UnchangedConceptEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


GreaterConceptEffect::GreaterConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_)
    : NamedElementEffect<NamedConcept>(identifier, concept_) {}

bool GreaterConceptEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const GreaterConceptEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool GreaterConceptEffect::evaluate(const core::State&, const core::State& target_state) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state).size();
    //if (target_eval == INF) return false;
    return target_eval > 0;
}

bool GreaterConceptEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches)->size();
    //if (target_eval == INF) return false;
    return target_eval > 0;
}

void GreaterConceptEffect::str_impl(std::stringstream& out) const {
    out << "(:e_c_gt " + m_named_element->get_key() + ")";
}

void GreaterConceptEffect::accept(BaseEffectVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}


EqualConceptEffect::EqualConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_)
    : NamedElementEffect<NamedConcept>(identifier, concept_) {}

bool EqualConceptEffect::are_equal_impl(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const EqualConceptEffect&>(other);
        return m_named_element == other_derived.m_named_element;
    }
    return false;
}

bool EqualConceptEffect::evaluate(const core::State&, const core::State& target_state) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state).size();
    //if (target_eval == INF) return false;
    return target_eval == 0;
}

bool EqualConceptEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    int target_eval = m_named_element->get_element()->evaluate(target_state, caches)->size();
    //if (target_eval == INF) return false;
    return target_eval == 0;
}

void EqualConceptEffect::str_impl(std::stringstream& out) const {
    out << "(:e_c_eq " + m_named_element->get_key() + ")";
}

void EqualConceptEffect::accept(BaseEffectVisitor& visitor) const {
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

    bool less<std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::EqualNumericalEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::EqualNumericalEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::EqualNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::IncrementConceptEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::IncrementConceptEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::IncrementConceptEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::DecrementConceptEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::DecrementConceptEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::DecrementConceptEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::GreaterConceptEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::GreaterConceptEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::GreaterConceptEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

    bool less<std::shared_ptr<const dlplan::policy::EqualConceptEffect>>::operator()(
        const std::shared_ptr<const dlplan::policy::EqualConceptEffect>& left_effect,
        const std::shared_ptr<const dlplan::policy::EqualConceptEffect>& right_effect) const {
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

    std::size_t hash<dlplan::policy::IncrementOrUnchangedNumericalEffect>::operator()(
        const dlplan::policy::IncrementOrUnchangedNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::DecrementNumericalEffect>::operator()(
        const dlplan::policy::DecrementNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::DecrementOrUnchangedNumericalEffect>::operator()(
        const dlplan::policy::DecrementOrUnchangedNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::UnchangedNumericalEffect>::operator()(
        const dlplan::policy::UnchangedNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::GreaterNumericalEffect>::operator()(
        const dlplan::policy::GreaterNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::EqualNumericalEffect>::operator()(
        const dlplan::policy::EqualNumericalEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::IncrementConceptEffect>::operator()(
        const dlplan::policy::IncrementConceptEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::DecrementConceptEffect>::operator()(
        const dlplan::policy::DecrementConceptEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::UnchangedConceptEffect>::operator()(
        const dlplan::policy::UnchangedConceptEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::GreaterConceptEffect>::operator()(
        const dlplan::policy::GreaterConceptEffect& effect) const {
        return effect.hash();
    }

    std::size_t hash<dlplan::policy::EqualConceptEffect>::operator()(
        const dlplan::policy::EqualConceptEffect& effect) const {
        return effect.hash();
    }
}
