#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../role.h"


namespace dlplan::core::element {

class TopRole : public Role {
public:
    TopRole(const VocabularyInfo& vocabulary)
    : Role(vocabulary) {
    }

    RoleDenotation evaluate(const State& state) const override {
        return state.get_instance_info_ref().get_top_role_ref();
    }

    const RoleDenotations& evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_r_denots_mapping.find(get_index());
        if (cached != caches.m_r_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        RoleDenotationsPtr denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            RoleDenotationPtr denotation = std::make_unique<RoleDenotation>(RoleDenotation(num_objects));
            denotation->set();
            // register denotation and append it to denotations.
            denotations->push_back(std::cref(*caches.m_r_denot_cache.insert(std::move(denotation)).first->get()));
        }
        // register denotations and return it.
        auto result_denotations = std::cref(*caches.m_r_denots_cache.insert(std::move(denotations)).first->get());
        caches.m_r_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
    }
    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    static std::string get_name() {
        return "r_top";
    }
};

}

#endif
