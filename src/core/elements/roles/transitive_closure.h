#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

// https://stackoverflow.com/questions/3517524/what-is-the-best-known-transitive-closure-algorithm-for-a-directed-graph
class TransitiveClosureRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
        result = denot;
        bool changed = false;
        do {
            RoleDenotation tmp_result = result;
            PairsOfObjectIndices pairs = tmp_result.to_vector();
            for (const auto& pair_1 : pairs) {
                for (const auto& pair_2 : pairs) {
                    if (pair_1.second == pair_2.first) {
                        result.insert(std::make_pair(pair_1.first, pair_2.second));
                    }
                }
            }
            changed = (result.size() != tmp_result.size());
        } while (changed);
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                denotation);
            denotations.push_back(caches.get_role_denotation_cache().insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

protected:
    const std::shared_ptr<const Role> m_role;

public:
    TransitiveClosureRole(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
    : Role(vocabulary_info, role->is_static()), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveClosureRole::TransitiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + SCORE_QUBIC;
    }

    static std::string get_name() {
        return "r_transitive_closure";
    }
};

}

#endif
