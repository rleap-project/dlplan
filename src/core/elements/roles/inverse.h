#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlplan::core::element {

class InverseRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
        for (const auto& pair : denot) {
            result.insert(std::make_pair(pair.second, pair.first));
        }
    }

protected:
    const Role_Ptr m_role;

public:
    InverseRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("InverseRole::InverseRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(
            m_role->evaluate(state),
            denotation);
        return denotation;
    }

    const RoleDenotations& evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_r_denots_mapping.find(get_index());
        if (cached != caches.m_r_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        RoleDenotationsPtr denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        // get denotations of children
        auto role_denotations = m_role->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            RoleDenotationPtr denotation = std::make_unique<RoleDenotation>(RoleDenotation(num_objects));
            compute_result(
                role_denotations[i],
                *denotation);
            // register denotation and append it to denotations.
            denotations->push_back(std::cref(*caches.m_r_denot_cache.insert(std::move(denotation)).first->get()));
        }
        // register denotations and return it.
        auto result_denotations = std::cref(*caches.m_r_denots_cache.insert(std::move(denotations)).first->get());
        caches.m_r_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
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
        return "r_inverse";
    }
};

}

#endif
