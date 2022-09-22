#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../boolean.h"



namespace dlplan::core::element {

class NullaryBoolean : public Boolean {
private:
    void compute_result(const State& state, bool& result) const {
        const auto& per_predicate_idx_atom_idxs = state.get_per_predicate_idx_atom_idxs_ref();
        auto it = per_predicate_idx_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_atom_idxs.end()) {
            result = !it->second.empty();
            return;
        }
        const auto& per_predicate_idx_static_atom_idxs = state.get_instance_info_ref().get_per_predicate_idx_static_atom_idxs_ref();
        it = per_predicate_idx_static_atom_idxs.find(m_predicate.get_index());
        if (it != per_predicate_idx_static_atom_idxs.end()) {
            result = !it->second.empty();
            return;
        }
        result = false;
    }

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
        bool denotation;
        compute_result(state, denotation);
        return denotation;
    }

    DENOTS<bool>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto cached = caches.m_b_denots_cache.find(get_index());
        if (cached) return cached;
        auto denotations = caches.m_b_denots_cache.get_new_denotations();
        for (size_t i = 0; i < states.size(); ++i) {
            bool denotation;
            compute_result(
                states[i],
                denotation);
            denotations->push_back(denotation);
        }
        return caches.m_b_denots_cache.insert(std::move(denotations), get_index());
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
