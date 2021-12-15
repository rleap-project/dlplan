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

    ConceptDenotation evaluate(const State& state) const override {
        const RoleDenotation r = m_role->evaluate(state);
        const ConceptDenotation c = m_concept->evaluate(state);
        ConceptDenotation result = state.get_instance_info()->get_top_concept_vec();
        // find counterexamples b : exists b . (a,b) in R and b notin C
        int num_objects = state.get_instance_info()->get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (!c.test(j)) {
                    if (r.test(i * num_objects + j)) {
                        result.reset(i);
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

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << "," << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
