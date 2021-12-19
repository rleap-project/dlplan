#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class EqualConcept : public Concept {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    EqualConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary, "c_equal"), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const RoleDenotation left_vec = m_role_left->evaluate(state);
        const RoleDenotation right_vec = m_role_right->evaluate(state);
        const RoleDenotation_Set left_set(left_vec.begin(), left_vec.end());
        const RoleDenotation_Set right_set(right_vec.begin(), right_vec.end());
        ConceptDenotation_Set result_set = state.get_instance_info()->get_top_concept_set();
        // Find counterexample: (a,b) in R and (a,b) not in S => remove a
        for (const auto& r : left_vec) {  // (a,b) in R
            if (right_set.find(r) == right_set.end()) {  // (a,b) notin S
                result_set.erase(r.first);
            }
        }
        // Find counterexample: (a,b) in S and (a,b) not in R => remove a
        for (const auto& r : right_vec) {  // (a,b) in S
            if (left_set.find(r) == left_set.end()) {  // (a,b) notin R
                result_set.erase(r.first);
            }
        }
        return ConceptDenotation(result_set.begin(), result_set.end());
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
