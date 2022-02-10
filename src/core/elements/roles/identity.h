#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class IdentityRole : public Role {
protected:
    const Concept_Ptr m_concept;

public:
    IdentityRole(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Role(vocabulary, "r_identity"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& caches, RoleDenotation result) const override {
        ConceptDenotation c = m_concept->evaluate(state, caches);
        dlplan::utils::BitsetView c_data = c.get_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        dlplan::utils::BitsetView result_data = result.get_data();
        result_data.reset();
        for (int i = 0; i < num_objects; ++i) {
            if (c_data.test(i)) {
                result_data.set(i * num_objects + i);
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_concept->compute_repr(out);
        out << ")";
    }
};

}

#endif
