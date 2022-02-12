#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ALL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ALL_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class AllConcept : public Concept {
protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    AllConcept(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Concept(vocabulary, "c_all"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("AllConcept::AllConcept - at least one child is a nullptr");
        }
    }

    void evaluate(const State& state, PerElementEvaluationCache& caches, ConceptDenotation& result) const override {
        const RoleDenotation r = m_role->evaluate(state, caches);
        const dlplan::utils::BitsetView& r_data = r.get_data();
        const ConceptDenotation c = m_concept->evaluate(state, caches);
        const dlplan::utils::BitsetView& c_data = c.get_data();
        dlplan::utils::BitsetView& result_data = result.get_data();
        result_data.set();
        // find counterexamples b : exists b . (a,b) in R and b notin C
        int num_objects = result.get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (!c_data.test(j)) {
                    if (r_data.test(i * num_objects + j)) {
                        result_data.reset(i);
                        break;
                    }
                }
            }
        }
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role->compute_repr(out);
        out << ",";
        m_concept->compute_repr(out);
        out << ")";
    }
};

}

#endif
