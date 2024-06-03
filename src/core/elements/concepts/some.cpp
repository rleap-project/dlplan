#include "../../../../include/dlplan/core/elements/concepts/some.h"


namespace dlplan::core {
void SomeConcept::compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, ConceptDenotation& result) const {
    // find examples a : exists b . (a,b) in R and b in C
    for (const auto& pair : role_denot.to_vector()) {
        if (concept_denot.contains(pair.second)) {
            result.insert(pair.first);
        }
    }
}

ConceptDenotation SomeConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations SomeConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    auto role_denotations = m_role->evaluate(states, caches);
    auto concept_denotations = m_concept->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            *(*role_denotations)[i],
            *(*concept_denotations)[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

SomeConcept::SomeConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_)
    : Concept(index, vocabulary_info, role->is_static() && concept_->is_static()), m_role(role), m_concept(concept_) { }

bool SomeConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const SomeConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t SomeConcept::hash_impl() const {
    return hash_combine(m_is_static, m_role, m_concept);
}

ConceptDenotation SomeConcept::evaluate(const State& state) const {
    auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int SomeConcept::compute_complexity_impl() const {
    return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
}

void SomeConcept::str_impl(std::stringstream& out) const {
    out << "c_some" << "(";
    m_role->str(out);
    out << ",";
    m_concept->str(out);
    out << ")";
}

int SomeConcept::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::SomeConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::SomeConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::SomeConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::SomeConcept>::operator()(const dlplan::core::SomeConcept& concept_) const {
        return concept_.hash();
    }
}
