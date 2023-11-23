#include "../../../../include/dlplan/core/elements/concepts/one_of.h"


namespace dlplan::core {
void OneOfConcept::compute_result(const State& state, ConceptDenotation& result) const {
    bool found = false;
    for (const auto& object : state.get_instance_info()->get_objects()) {
        if (object.get_name() == m_constant.get_name()) {
            result.insert(object.get_index());
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name() + ")");
    }
}

ConceptDenotation OneOfConcept::evaluate_impl(const State& state, DenotationsCaches&) const {
    ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        state,
        denotation);
    return denotation;
}

ConceptDenotations OneOfConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            states[i],
            denotation);
        denotations.push_back(caches.data.insert_unique(std::move(denotation)));
    }
    return denotations;
}

OneOfConcept::OneOfConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Constant& constant)
    : Concept(index, vocabulary_info, true), m_constant(constant) { }

bool OneOfConcept::are_equal_impl(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const OneOfConcept&>(other);
        return m_is_static == other_derived.m_is_static
            && m_constant == other_derived.m_constant;
    }
    return false;
}

size_t OneOfConcept::hash_impl() const {
    return hash_combine(m_is_static, m_constant);
}

ConceptDenotation OneOfConcept::evaluate(const State& state) const {
    ConceptDenotation result(state.get_instance_info()->get_objects().size());
    compute_result(state, result);
    return result;
}

int OneOfConcept::compute_complexity_impl() const {
    return 1;
}

void OneOfConcept::str_impl(std::stringstream& out) const {
    out << "c_one_of" << "(" << m_constant.get_name() << ")";
}

int OneOfConcept::compute_evaluate_time_score_impl() const {
    return SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::OneOfConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::OneOfConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::OneOfConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::OneOfConcept>::operator()(const dlplan::core::OneOfConcept& concept_) const {
        return concept_.hash();
    }
}