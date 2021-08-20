#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SubsetConcept : public Concept {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    SubsetConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary, "c_subset"), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("SubsetConcept::SubsetConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const RoleDenotation left_vec = m_role_left->evaluate(state);
        const RoleDenotation right_vec = m_role_right->evaluate(state);
        std::vector<RoleDenotation> left_by_first(state.get_instance_info()->get_num_objects());
        for (const auto& role : left_vec) {
            left_by_first[role.first].push_back(role);
        }
        RoleDenotation_Set right_set(right_vec.begin(), right_vec.end());

        ConceptDenotation result;
        result.reserve(state.get_instance_info()->get_num_objects());
        for (int c = 0; c < state.get_instance_info()->get_num_objects(); ++c) {
            if (left_by_first[c].empty()) {
                // nothing that violates the condition (a,b) in R -> (a,b) in S
                result.push_back(c);
            } else {
                bool violated = false;
                for (const auto& role : left_by_first[c]) {
                    if (right_set.find(role) == right_set.end()) {
                        violated = true;
                        break;
                    }
                }
                if (!violated) {
                    result.push_back(c);
                }
            }
        }
        return result;
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
