#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_DIFF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_DIFF_H_

#include "../role.h"


namespace dlplan::core::element {

class DiffRole : public Role {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
        result = left_denot;
        result -= right_denot;
    }

protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    DiffRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("DiffRole::DiffRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    RoleDenotation* evaluate(const State& state, DenotationsCaches& caches) const override {
    }

    RoleDenotations* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_r_denots_mapping.find(get_index());
        if (cached != caches.m_r_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        auto denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        // get denotations of children
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = std::make_unique<RoleDenotation>(RoleDenotation(num_objects));
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                *denotation);
            // register denotation and append it to denotations.
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)).first->get());
        }
        // register denotations and return it.
        auto result_denotations = caches.m_r_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_r_denots_mapping.emplace(get_index(), result_denotations);
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
        return "r_diff";
    }
};

}

#endif
