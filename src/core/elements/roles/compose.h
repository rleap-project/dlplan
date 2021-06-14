#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_COMPOSE_H_

#include "../role.h"
#include <algorithm>


namespace dlplan::core::element {

class ComposeRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

protected:
    const RoleDenotation& evaluate_impl(const State& state) override {
        RoleDenotation l_vec = m_role_left->evaluate(state);
        RoleDenotation r_vec = m_role_right->evaluate(state);
        // complexity
        // sorted by second component
        std::sort(l_vec.begin(), l_vec.end(), [](const auto& l, const auto& r){ return l.second < r.second; });
        // sorted by first component
        std::sort(r_vec.begin(), r_vec.end(), [](const auto& l, const auto& r){ return l.first < r.first; });
        auto l_it = l_vec.begin();
        auto r_it = r_vec.begin();
        RoleDenotation_Set result_set;
        while (l_it != l_vec.end() && r_it != r_vec.end()) {
            if (l_it->second < r_it->first) {
                ++l_it;
            } else if (l_it->second > r_it->first) {
                ++r_it;
            } else {
                // The second component of pair in l_vec and first component of pair in r_vec comply for composition.
                auto r_it_2 = r_it;
                // Iterate all in r_vec that comply with the current one in l_vec.
                while (r_it_2 != r_vec.end() && l_it->second == r_it_2->first) {
                    result_set.emplace(l_it->first, r_it_2->second);
                    ++r_it_2;
                }
                ++l_it;
                ++r_it;
            }
        }
        m_result.clear();
        m_result.insert(m_result.begin(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    ComposeRole(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Role(vocabulary, "r_compose"), m_role_left(role_left), m_role_right(role_right)  {
        if (!(role_left && role_right)) {
            throw std::runtime_error("ComposeRole::ComposeRole - at least one child is a nullptr.");
        }
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
