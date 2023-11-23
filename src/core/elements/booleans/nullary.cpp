#include "../../../../include/dlplan/core/elements/booleans/nullary.h"


namespace dlplan::core {

void NullaryBoolean::compute_result(const State& state, bool& result) const {
    const auto& atoms = state.get_instance_info()->get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            result = true;
            return;
        }
    }
    for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            result = true;
            return;
        }
    }
    result = false;
}

bool NullaryBoolean::evaluate_impl(const State& state, DenotationsCaches&) const {
    return evaluate(state);
}

BooleanDenotations
NullaryBoolean::evaluate_impl(const States& states, DenotationsCaches&) const {
    BooleanDenotations denotations;
    for (size_t i = 0; i < states.size(); ++i) {
        denotations.push_back(evaluate(states[i]));
    }
    return denotations;
}

NullaryBoolean::NullaryBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate)
: Boolean(index, vocabulary_info, predicate.is_static()), m_predicate(predicate) {
    if (predicate.get_arity() != 0) {
        throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
    }
}

bool NullaryBoolean::are_equal_impl(const Boolean& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const NullaryBoolean&>(other);
        return m_is_static == other_derived.m_is_static
            && m_predicate == other_derived.m_predicate;
    }
    return false;
}

size_t NullaryBoolean::hash_impl() const {
    return hash_combine(m_is_static, m_predicate);
}

bool NullaryBoolean::evaluate(const State& state) const {
    bool denotation;
    compute_result(state, denotation);
    return denotation;
}

int NullaryBoolean::compute_complexity_impl() const {
    return 1;
}

void NullaryBoolean::str_impl(std::stringstream& out) const {
    out << "b_nullary" << "(" << m_predicate.get_name() << ")";
}

int NullaryBoolean::compute_evaluate_time_score_impl() const {
    return SCORE_LINEAR;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::NullaryBoolean>>::operator()(
        const std::shared_ptr<const dlplan::core::NullaryBoolean>& left_boolean,
        const std::shared_ptr<const dlplan::core::NullaryBoolean>& right_boolean) const {
        return *left_boolean < *right_boolean;
    }

    std::size_t hash<dlplan::core::NullaryBoolean>::operator()(const dlplan::core::NullaryBoolean& boolean) const {
        return boolean.hash();
    }
}

