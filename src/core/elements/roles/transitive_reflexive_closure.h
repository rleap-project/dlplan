#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveReflexiveClosureRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, int num_objects, RoleDenotation& result) const {
        auto role_bitset = utils::role_denot_to_bitset(denot);
        for (int k = 0; k < num_objects; ++k) {
            for (int i = 0; i < num_objects; ++i) {
                for (int j = 0; j < num_objects; ++j) {
                    if (role_bitset.test(i * num_objects + k) && role_bitset.test(k * num_objects + j)) {
                        role_bitset.set(i * num_objects + j);
                    }
                }
            }
            role_bitset.set(k * num_objects + k);
        }
        result = utils::bitset_to_role_denotation(role_bitset, num_objects);
    }

protected:
    const Role_Ptr m_role;

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info_ref().get_num_objects();
        RoleDenotation denotation(num_objects);
        compute_result(
            m_role->evaluate(state),
            num_objects,
            denotation);
        return denotation;
    }

    std::vector<RoleDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto role_cache_entry = caches.m_r_denots_cache.find(get_index());
        if (role_cache_entry) return role_cache_entry;
        // todo new denotations will leak. should we return unique_ptr instead?
        auto denotations = caches.m_r_denots_cache.get_new_denotations();
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = caches.m_r_denot_cache.get_new_denotation();
            const auto& state = states[i];
            compute_result(
                *((*role_denotations)[i]),
                state.get_instance_info_ref().get_num_objects(),
                *denotation);
            denotation = caches.m_r_denot_cache.insert(denotation);
            denotations->push_back(denotation);
        }
        return denotations;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_transitive_reflexive_closure";
    }
};

}

#endif
