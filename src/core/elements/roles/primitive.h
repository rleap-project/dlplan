#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlp::core::element {

class PrimitiveRole : public Role {
protected:
    const Predicate& m_predicate;
    const int m_pos_1;
    const int m_pos_2;

protected:
    const RoleDenotation& evaluate_impl(const State& state) override {
        const InstanceInfo& info = *state.get_instance_info();
        RoleDenotation_Set result_set;
        for (int atom_idx : state.get_atom_idxs()) {
            const Atom& atom = info.get_atom(atom_idx);
            if (atom.get_predicate().get_index() == m_predicate.get_index()) {
                result_set.emplace(atom.get_object(m_pos_1).get_index(), atom.get_object(m_pos_2).get_index());
            }
        }
        m_result.clear();
        m_result.insert(m_result.end(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    PrimitiveRole(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos_1, int pos_2)
    : Role(vocabulary, predicate.get_name()), m_predicate(predicate), m_pos_1(pos_1), m_pos_2(pos_2) {
        if (m_pos_1 >= predicate.get_arity() || m_pos_2 >= predicate.get_arity()) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (predicate.get_vocabulary_info() != &vocabulary) {
            throw std::runtime_error("PrimitiveRole::PrimitiveRole - predicate does not come from same vocabulary.");
        }
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
        return ss.str();
    }
};

}

#endif
