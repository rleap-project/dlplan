#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveReflexiveClosureRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, int num_objects, RoleDenotation& result) const {
        result = denot;
        bool changed = false;
        do {
            RoleDenotation tmp_result = result;
            for (const auto pair_1 : tmp_result) {
                for (const auto pair_2 : tmp_result) {
                    if (pair_1.second == pair_2.first) {
                        result.insert(std::make_pair(pair_1.first, pair_2.second));
                    }
                }
            }
            changed = (result.size() != tmp_result.size());
        } while (changed);
        // add reflexive part
        for (int i = 0; i < num_objects; ++i) {
            result.insert(std::make_pair(i, i));
        }
    }

    std::unique_ptr<RoleDenotation> evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto denotation = std::make_unique<RoleDenotation>(
            RoleDenotation(state.get_instance_info()->get_objects().size()));
        compute_result(
            *m_role->evaluate(state, caches),
            state.get_instance_info()->get_objects().size(),
            *denotation);
        return denotation;
    }

    std::unique_ptr<RoleDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = std::make_unique<RoleDenotation>(
                RoleDenotation(states[i].get_instance_info()->get_objects().size()));
            compute_result(
                *(*role_denotations)[i],
                states[i].get_instance_info()->get_objects().size(),
                *denotation);
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)).first->get());
        }
       return denotations;
    }

protected:
    const Role_Ptr m_role;

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, role->is_static()), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_objects().size();
        RoleDenotation denotation(num_objects);
        compute_result(
            m_role->evaluate(state),
            num_objects,
            denotation);
        return denotation;
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
