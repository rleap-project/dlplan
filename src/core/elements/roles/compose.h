#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class ComposeRole : public Role {
private:
    RoleDenotation compute_result(RoleDenotation&& left_denot, RoleDenotation&& right_denot, RoleDenotation&& result, int num_objects) const {
        const auto left_bitset = element::utils::role_denot_to_bitset(left_denot);
        const auto right_bitset = element::utils::role_denot_to_bitset(right_denot);
        for (int i = 0; i < num_objects; ++i) {  // source
            for (int j = 0; j < num_objects; ++j) {  // target
                for (int k = 0; k < num_objects; ++k) {  // middle
                    if (left_bitset.test(i * num_objects + k) && right_bitset.test(k * num_objects + j)) {
                        result.insert(std::make_pair(i, j));
                        break;
                    }
                }
            }
        }
        return result;
    }

protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    ComposeRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::ComposeRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        auto bot_role = RoleDenotation(num_objects);
        return compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            std::move(bot_role),
            num_objects);
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        int num_objects = state.get_instance_info()->get_num_objects();
        auto bot_role = RoleDenotation(num_objects);
        auto result = compute_result(
            m_role_left->evaluate(state, cache),
            m_role_right->evaluate(state, cache),
            std::move(bot_role),
            num_objects);
        cache.m_role_denotation_cache.insert(state, *this, result);
        return result;
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
        return "r_compose";
    }
};

}

#endif
