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

        int num_objects = state.get_instance_info()->get_num_objects();
        ConceptDenotation result;
        result.reserve(num_objects);
        for (int i = 0; i < num_objects; ++i) {
            // If the set of y such that (x, y) in sd1 is equal to the set of z such that (x, z) in sd2,
            // then x makes it into the denotation of this concept
            bool in_denotation = true;
            for (int j = 0; j < num_objects; ++j) {
                std::pair<int, int> r(i, j);
                bool in_left = (left_set.find(r) != left_set.end());
                bool in_right = (right_set.find(r) != right_set.end());
                // std::cout << i << " " << j << " : " << in_left << " " << in_right << std::endl;
                if (in_left != in_right) in_denotation = false;
            }
            if (in_denotation) result.push_back(i);
        }
        result.shrink_to_fit();
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
