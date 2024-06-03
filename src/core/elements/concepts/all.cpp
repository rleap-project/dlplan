#include "../../../../include/dlplan/core/elements/concepts/all.h"


namespace dlplan::core {
void AllConcept::compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, ConceptDenotation& result) const {
    // find counterexamples b : exists b . (a,b) in R and b notin C
    result.set();
    for (const auto& pair : role_denot.to_vector()) {
        if (!concept_denot.contains(pair.second)) {
            result.erase(pair.first);
        }
    }
}

ConceptDenotation AllConcept::evaluate_impl(const State& state, DenotationsCaches& caches) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        *m_role->evaluate(state, caches),
        *m_concept->evaluate(state, caches),
        denotation);
    return denotation;
}

ConceptDenotations AllConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
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
        denotations.push_back(caches.data.insert_unique<ConceptDenotation>(std::move(denotation)));
    }
    return denotations;
}

AllConcept::AllConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_)
    : Concept(index, vocabulary_info, role->is_static() && concept_->is_static()), m_role(role), m_concept(concept_) { }

bool AllConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const AllConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_role == other_derived.m_role
            && m_concept == other_derived.m_concept;
    }
    return false;
}

size_t AllConcept::hash_impl() const {
    return hash_combine(m_is_static, m_role, m_concept);
}

ConceptDenotation AllConcept::evaluate(const State& state) const {
    auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
    compute_result(
        m_role->evaluate(state),
        m_concept->evaluate(state),
        denotation);
    return denotation;
}

int AllConcept::compute_complexity_impl() const {
    return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
}

void AllConcept::str_impl(std::stringstream& out) const {
    out << "c_all" << "(";
    m_role->str(out);
    out << ",";
    m_concept->str(out);
    out << ")";
}

int AllConcept::compute_evaluate_time_score_impl() const {
    return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::AllConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::AllConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::AllConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::AllConcept>::operator()(const dlplan::core::AllConcept& concept_) const {
        return concept_.hash();
    }
}