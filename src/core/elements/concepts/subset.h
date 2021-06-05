#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_

#include "../concept.h"
#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class SubsetConcept : public Concept {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

    ConceptDenotation_Set m_top;

protected:
    virtual const ConceptDenotation& evaluate_impl(const State& state) override {
        /*
        // TODO(dominik): implement improved version by making use of role denotations sorted by their first component.
        RoleDenotation left_vec = m_role_left->evaluate(state);
        RoleDenotation right_vec = m_role_right->evaluate(state);
        std::sort(left_vec.begin(), left_vec.end(), [](const std::pair<int, int>& l, const std::pair<int, int>& r){ return l.first < r.first;});
        std::sort(right_vec.begin(), right_vec.end(), [](const std::pair<int, int>& l, const std::pair<int, int>& r){ return l.first < r.first;});
        int i = 0;
        int j = 0;
        for (int object_idx = 0; object_idx < state.get_instance_info()->get_num_objects(); ++object_idx) {
        }
        */
        const RoleDenotation& left_vec = m_role_left->evaluate(state);
        const RoleDenotation& right_vec = m_role_right->evaluate(state);
        RoleDenotation_Set left_set(left_vec.begin(), left_vec.end());
        RoleDenotation_Set right_set(right_vec.begin(), right_vec.end());
        // 1. Collect all element that occur somewhere at a and b
        // because these elements are candidates to be checked
        ConceptDenotation_Set a_set;
        ConceptDenotation_Set b_set;
        for (const auto& role : left_vec) {
            a_set.insert(role.first);
            b_set.insert(role.second);
        }
        for (const auto& role : right_vec) {
            a_set.insert(role.first);
            b_set.insert(role.second);
        }
        // 2. Initialize result to contain the whole universe
        if (m_top.empty()) {
            unsigned num_objects = state.get_instance_info()->get_num_objects();
            for (unsigned object_idx = 0; object_idx < num_objects; ++object_idx) {
                m_top.insert(object_idx);
            }
        }
        ConceptDenotation_Set result_set = m_top;
        // 3. Exclude concepts from result that fail the test: forall b:(a,b)\in R -> (a,b)\in S
        for (int a : a_set) {
            for (int b : b_set) {
                std::pair<int, int> r(a, b);
                bool left_exists = (left_set.find(r) != left_set.end());
                bool right_exists = (right_set.find(r) != right_set.end());
                if (left_exists && !right_exists) {
                    result_set.erase(a);
                    break;
                }
            }
        }
        m_result.clear();
        m_result.insert(m_result.begin(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    SubsetConcept(const VocabularyInfoImpl& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary, "c_subset"), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("SubsetConcept::SubsetConcept - at least one child is a nullptr");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
