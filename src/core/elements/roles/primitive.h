#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlplan::core::element {

static void collect_roles(
    const phmap::flat_hash_map<int, std::vector<int>>& per_predicate_idx_atom_idxs,
    const std::vector<Atom>& atoms,
    const Predicate& predicate,
    int pos_1,
    int pos_2,
    RoleDenotation& result) {
    auto it = per_predicate_idx_atom_idxs.find(predicate.get_index());
    if (it != per_predicate_idx_atom_idxs.end()) {
        for (int atom_idx : it->second) {
            const auto& atom = atoms[atom_idx];
            result.insert(std::make_pair(atom.get_object_ref(pos_1).get_index(), atom.get_object_ref(pos_2).get_index()));
        }
    }
}

class PrimitiveRole : public Role {
private:
    void compute_result(const State& state, RoleDenotation& result) const {
        const auto& atoms = state.get_instance_info_ref().get_atoms_ref();
        const auto& static_atoms = state.get_instance_info_ref().get_static_atoms_ref();
        collect_roles(state.get_per_predicate_idx_atom_idxs_ref(), atoms, m_predicate, m_pos_1, m_pos_2, result);
        collect_roles(state.get_instance_info_ref().get_per_predicate_idx_static_atom_idxs_ref(), static_atoms, m_predicate, m_pos_1, m_pos_2, result);
    }

protected:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

public:
    PrimitiveRole(const VocabularyInfo& vocabulary, const Predicate& predicate, int pos_1, int pos_2)
    : Role(vocabulary), m_predicate(predicate), m_pos_1(pos_1), m_pos_2(pos_2) {
        if (m_pos_1 >= m_predicate.get_arity() || m_pos_2 >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate.get_arity()) + ").");
        }
        if (!vocabulary.exists_predicate(m_predicate)) {
            throw std::runtime_error("PrimitiveRole::PrimitiveRole - predicate does not exist in VocabularyInfo.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(state, denotation);
        return denotation;
    }

    DENOTS<RoleDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_r_denots_mapping.find(get_index());
        if (cached) return cached;
        // allocate memory for new denotations
        auto denotations = caches.m_r_denots_cache.get_new_entry(states.size());
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_r_denot_cache.get_new_entry(num_objects);
            compute_result(
                state,
                *denotation);
            // register denotation and append it to denotations.
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)).first->get());
        }
        // register denotations and return it.
        auto result_denotations = caches.m_r_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_r_denots_mapping.insert(result_denotations, get_index());
        return result_denotations;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name_ref() << "," << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
    }

    static std::string get_name() {
        return "r_primitive";
    }

    const Predicate& get_predicate_ref() const {
        return m_predicate;
    }
};

}

#endif
