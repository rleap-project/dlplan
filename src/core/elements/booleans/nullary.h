#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../boolean.h"



namespace dlplan::core::element {

class NullaryBoolean : public Boolean {
protected:
    const Predicate m_predicate;

public:
    NullaryBoolean(const VocabularyInfo& vocabulary, const Predicate& predicate)
    : Boolean(vocabulary), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
        if (!vocabulary.exists_predicate(predicate)) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - predicate does not exist in VocabularyInfo.");
        }
    }

    bool evaluate(const State& state) const override {
        const auto& per_predicate_idx_atom_idxs = state.get_per_predicate_idx_atom_idxs();
        auto it = per_predicate_idx_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_atom_idxs.end()) {
            return !it->second.empty();
        }
        const auto& per_predicate_idx_static_atom_idxs = state.get_instance_info()->get_per_predicate_idx_static_atom_idxs();
        it = per_predicate_idx_static_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_static_atom_idxs.end()) {
            return !it->second.empty();
        }
        return false;
    }

    bool evaluate(const State& state, EvaluationCaches&) const override {
        return evaluate(state);
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name() << ")";
    }

    static std::string get_name() {
        return "b_nullary";
    }
};

}

#endif
