#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../boolean.h"

namespace dlplan::core::element {

class NullaryBoolean : public Boolean {
protected:
    const Predicate m_predicate;

public:
    NullaryBoolean(const VocabularyInfo& vocabulary, const Predicate& predicate)
    : Boolean(vocabulary, "b_nullary"), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
        if (!vocabulary.exists_predicate(predicate)) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - predicate does not exist in VocabularyInfo.");
        }
    }

    bool evaluate(const State& state) const override {
        const InstanceInfo& info = *state.get_instance_info();
        for (int atom_idx : state.get_atom_idxs()) {
            const Atom& atom = info.get_atom(atom_idx);
            if (atom.get_predicate().get_index() == m_predicate.get_index()) {
                return true;
            }
        }
        return false;
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_predicate.get_name() << ")";
        return ss.str();
    }
};

}

#endif
