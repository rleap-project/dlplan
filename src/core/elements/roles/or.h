#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_

#include "../role.h"


namespace dlplan::core::element {

class OrRole : public Role {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
        result = left_denot;
        result |= right_denot;
    }

protected:
    Role_Ptr m_role_left;
    Role_Ptr m_role_right;

public:
    OrRole(const VocabularyInfo& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary),
      m_role_left(role_1),
      m_role_right(role_2) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("OrRole::OrRole - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_role_left->compute_repr(ss1);
        std::stringstream ss2;
        m_role_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_role_left, m_role_right);
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    DENOTS<RoleDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_r_denots_mapping.find(get_index());
        if (cached) return cached;
        // allocate memory for new denotations
        auto denotations = caches.m_r_denots_cache.get_new_entry(states.size());
        // get denotations of children
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_r_denot_cache.get_new_entry(num_objects);
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
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
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_or";
    }
};

}

#endif
