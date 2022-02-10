#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SomeConcept : public Concept {
protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    SomeConcept(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Concept(vocabulary, "c_some"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("SomeConcept::SomeConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& caches, ConceptDenotation result) const override {
        RoleDenotation r = m_role->evaluate(state, caches);
        dlplan::utils::BitsetView r_data = r.get_data();
        ConceptDenotation c = m_concept->evaluate(state, caches);
        dlplan::utils::BitsetView c_data = c.get_data();
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.reset();
        // find examples a : exists b . (a,b) in R and b in C
        int num_objects = state.get_instance_info()->get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (c_data.test(j)) {
                    if (r_data.test(i * num_objects + j)) {
                        result_data.set(i);
                        break;
                    }
                }
            }
        }
        return result;
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
