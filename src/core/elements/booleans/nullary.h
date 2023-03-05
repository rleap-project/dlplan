#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../boolean.h"



namespace dlplan::core::element {

class NullaryBoolean : public Boolean {
private:
    void compute_result(const State& state, bool& result) const {
        const auto& atoms = state.get_instance_info_ref().get_atoms_ref();
        for (int atom_idx : state.get_atom_idxs_ref()) {
            const auto& atom = atoms[atom_idx];
            if (atom.get_predicate_ref().get_index() == m_predicate.get_index()) {
                result = true;
                return;
            }
        }
        const auto& per_predicate_idx_static_atom_idxs = state.get_instance_info_ref().get_per_predicate_idx_static_atom_idxs_ref();
        auto it = per_predicate_idx_static_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_static_atom_idxs.end()) {
            result = !it->second.empty();
            return;
        }
        result = false;
    }

    bool evaluate_impl(const State& state, DenotationsCaches&) const override {
        return evaluate(state);
    }

    std::unique_ptr<BooleanDenotations>
    evaluate_impl(const States& states, DenotationsCaches&) const override {
        auto denotations = std::make_unique<BooleanDenotations>();
        for (size_t i = 0; i < states.size(); ++i) {
            denotations->push_back(evaluate(states[i]));
        }
        return denotations;
    }

protected:
    const Predicate m_predicate;

public:
    NullaryBoolean(const VocabularyInfo& vocabulary, const Predicate& predicate)
    : Boolean(vocabulary, predicate.get_is_static()), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
        if (!vocabulary.exists_predicate(predicate)) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - predicate does not exist in VocabularyInfo.");
        }
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name_ref() << ")";
    }

    static std::string get_name() {
        return "b_nullary";
    }
};

}

#endif
