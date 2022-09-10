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
    : Concept(vocabulary), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const auto role_left_denot = m_role_left->evaluate(state);
        const auto role_right_denot = m_role_right->evaluate(state);
        ConceptDenotation result = state.get_instance_info()->get_top_concept();
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        for (const auto& pair : role_left_denot) {
            if (!role_right_denot.contains(pair)) result.erase(pair.first);
        }
        for (const auto& pair : role_right_denot) {
            if (!role_left_denot.contains(pair)) result.erase(pair.first);
        }
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
        return "c_equal";
    }
};

}

#endif
