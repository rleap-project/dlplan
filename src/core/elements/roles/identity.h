#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class IdentityRole : public Role {
protected:
    const Concept_Ptr m_concept;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const ConceptDenotation& c_vec = m_concept->evaluate(state);
        m_result.clear();
        m_result.reserve(c_vec.size());
        for (int c : c_vec) {
            m_result.push_back(std::make_pair(c, c));
        }
        return m_result;
    }

public:
    IdentityRole(const VocabularyInfoImpl& vocabulary, Concept_Ptr concept)
    : Role(vocabulary, "r_identity"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    virtual unsigned compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif
