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

    void evaluate(PerElementEvaluationContext& context, RoleDenotation& result) const override {
        const ConceptDenotation c = m_concept->evaluate(context);
        const dlplan::utils::BitsetView& c_data = c.get_data();
        dlplan::utils::BitsetView& result_data = result.get_data();
        result_data.reset();
        int num_objects = result.get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            if (c_data.test(i)) {
                result_data.set(i * num_objects + i);
            }
        }
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
