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

    void evaluate(const State& state, PerElementEvaluationCache& caches, ConceptDenotation& result) const override {
        const RoleDenotation r = m_role_left->evaluate(state, caches);
        const dlplan::utils::BitsetView& r_data = r.get_data();
        const RoleDenotation s = m_role_right->evaluate(state, caches);
        const dlplan::utils::BitsetView& s_data = s.get_data();
        dlplan::utils::BitsetView& result_data = result.get_data();
        result_data.set();
        // find counterexamples a : exists b . (a,b) in R and (a,b) notin S
        int num_objects = result.get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                int index = i * num_objects + j;
                if (r_data.test(index) && !s_data.test(index)) {
                    result_data.reset(i);
                    break;
                }
            }
        }
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }
};

}

#endif
