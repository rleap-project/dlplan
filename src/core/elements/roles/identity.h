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

    const RoleDenotation& evaluate(const State& state) override {
        const ConceptDenotation& c_vec = m_concept->evaluate(state);
        m_result.clear();
        for (int c : c_vec) {
            m_result.emplace_back(c, c);
        }
        return m_result;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
