#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class RestrictRole : public Role {
protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    RestrictRole(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Role(vocabulary, "r_restrict"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("RestrictRole::RestrictRole - at least one child is a nullptr.");
        }
    }

    void evaluate(const State& state, PerElementEvaluationCache& caches, RoleDenotation& result) const override {
        const RoleDenotation r = m_role->evaluate(state, caches);
        const ConceptDenotation c = m_concept->evaluate(state, caches);
        const dlplan::utils::BitsetView& c_data = c.get_data();
        dlplan::utils::BitsetView& result_data = result.get_data();
        result_data.set(r.get_data());
        int num_objects = result.get_num_objects();
        for (int i = 0; i < num_objects; ++i) {
            for (int j = 0; j < num_objects; ++j) {
                if (!c_data.test(j)) {
                    result_data.reset(i * num_objects + j);
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
