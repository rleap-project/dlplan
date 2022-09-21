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

    DENOTS<RoleDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto cached = caches.m_r_denots_cache.find(get_index());
        if (cached) return cached;
        auto denotations = caches.m_r_denots_cache.get_new_denotations();
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_r_denot_cache.get_new_denotation(num_objects);
            denotation->set();
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)));
        }
        return caches.m_r_denots_cache.insert(std::move(denotations), get_index());
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
