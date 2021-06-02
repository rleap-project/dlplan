#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_

#include "../role.h"
#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class RestrictRole : public Role {
protected:
    Role_Ptr m_role;
    Concept_Ptr m_concept;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const RoleDenotation& r_vec = m_role->evaluate(state);
        const ConceptDenotation& c_vec = m_concept->evaluate(state);
        ConceptDenotation_Set c_set(c_vec.begin(), c_vec.end());
        m_result.clear();
        for (const auto& r : r_vec) {
            if (c_set.find(r.second) != c_set.end()) {
                m_result.push_back(r);
            }
        }
        return m_result;
    }

public:
    RestrictRole(const VocabularyInfoImpl& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Role(vocabulary, "r_restrict"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("RestrictRole::RestrictRole - at least one child is a nullptr.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << "," << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
