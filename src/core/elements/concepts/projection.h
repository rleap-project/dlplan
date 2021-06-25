#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_

#include <sstream>

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class ProjectionConcept : public Concept {
protected:
    const Role_Ptr m_role;
    const int m_pos;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        const RoleDenotation& role_result = m_role->evaluate(state);
        ConceptDenotation_Set result_set;
        for (const auto& r : role_result) {
            if (m_pos == 0) {
                result_set.insert(r.first);
            } else if (m_pos == 1) {
                result_set.insert(r.second);
            }
        }
        m_result.clear();
        m_result.insert(m_result.end(), result_set.begin(), result_set.end());
        return m_result;
    }

public:
    ProjectionConcept(const VocabularyInfo& vocabulary, const Role_Ptr& role, int pos)
    : Concept(vocabulary, "c_projection"), m_role(role), m_pos(pos) {
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
        if (!role) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - child is a nullptr.");
        }
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}

#endif
