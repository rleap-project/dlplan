#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlplan::core::element {

class PrimitiveRole : public Role {
private:
    void compute_result(const State& state, RoleDenotation& result) const {
        const auto& instance_info = *state.get_instance_info();
        const auto& atoms = instance_info.get_atoms();
        for (int atom_idx : state.get_atom_indices()) {
            const auto& atom = atoms[atom_idx];
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
                assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
                result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]));
            }
        }
        for (const auto& atom : state.get_instance_info()->get_static_atoms()) {
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
                assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
                result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]));
            }
        }
    }

    std::unique_ptr<RoleDenotation> evaluate_impl(const State& state, DenotationsCaches&) const override {
        auto denotation = std::make_unique<RoleDenotation>(
            RoleDenotation(state.get_instance_info()->get_objects().size()));
        compute_result(
            state,
            *denotation);
        return denotation;
    }

    std::unique_ptr<RoleDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = std::make_unique<RoleDenotation>(
                RoleDenotation(states[i].get_instance_info()->get_objects().size()));
            compute_result(
                states[i],
                *denotation);
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)).first->get());
        }
        return denotations;
    }

protected:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

public:
    PrimitiveRole(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos_1, int pos_2)
    : Role(vocabulary, predicate.is_static()), m_predicate(predicate), m_pos_1(pos_1), m_pos_2(pos_2) {
        if (m_pos_1 >= m_predicate.get_arity() || m_pos_2 >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate.get_arity()) + ").");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name() << "," << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
    }

    static std::string get_name() {
        return "r_primitive";
    }

    const Predicate& get_predicate() const {
        return m_predicate;
    }
};

}

#endif
