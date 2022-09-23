#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

// https://stackoverflow.com/questions/3517524/what-is-the-best-known-transitive-closure-algorithm-for-a-directed-graph
class TransitiveClosureRole : public Role {
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
        }
        result = utils::bitset_to_role_denotation(role_bitset, num_objects);
    }

protected:
    const Role_Ptr m_role;

public:
    TransitiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveClosureRole::TransitiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info_ref().get_num_objects();
        RoleDenotation result(num_objects);
        compute_result(
            m_role->evaluate(state),
            num_objects,
            result);
        return result;
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
        auto role_denotations = m_role->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = std::make_unique<RoleDenotation>(RoleDenotation(num_objects));
            compute_result(
                *(*role_denotations)[i],
                num_objects,
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
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_transitive_closure";
    }
};

}

#endif
