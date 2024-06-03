#include "../../../../include/dlplan/core/elements/roles/identity.h"


namespace dlplan::core {
void IdentityRole::compute_result(const ConceptDenotation& denot, RoleDenotation& result) const {
    for (const auto& single : denot.to_vector()) {
        result.insert(std::make_pair(single, single));
    }
}

RoleDenotation IdentityRole::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

RoleDenotations IdentityRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

IdentityRole::IdentityRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_)
    : Role(index, vocabulary_info, concept_->is_static()), m_concept(concept_) { }

bool IdentityRole::are_equal_impl(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const IdentityRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t IdentityRole::hash_impl() const {
    return hash_combine(m_is_static, m_concept);
}

RoleDenotation IdentityRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int IdentityRole::compute_complexity_impl() const {
    return m_concept->compute_complexity() + 1;
}

void IdentityRole::str_impl(std::stringstream& out) const {
    out << "r_identity" << "(";
    m_concept->str(out);
    out << ")";
}

int IdentityRole::compute_evaluate_time_score_impl() const {
    return m_concept->compute_evaluate_time_score() + SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::IdentityRole>>::operator()(
        const std::shared_ptr<const dlplan::core::IdentityRole>& left_role,
        const std::shared_ptr<const dlplan::core::IdentityRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::IdentityRole>::operator()(const dlplan::core::IdentityRole& role) const {
        return role.hash();
    }
}
