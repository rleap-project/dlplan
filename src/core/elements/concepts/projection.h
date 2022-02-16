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

    void evaluate(PerElementEvaluationContext& context, ConceptDenotation& result) const override {
        const RoleDenotation r = m_role->evaluate(context);
        const dlplan::utils::BitsetView& r_data = r.get_data();
        dlplan::utils::BitsetView& result_data = result.get_data();
        result_data.reset();
        int num_objects = result.get_num_objects();
        if (m_pos == 0) {
            for (int i = 0; i < num_objects; ++i) {
                for (int j = 0; j < num_objects; ++j) {
                    int index = i * num_objects + j;
                    if (r_data.test(index)) {
                        result_data.set(i);
                        break;
                    }
                }
            }
        } else if (m_pos == 1) {
            for (int j = 0; j < num_objects; ++j) {
                for (int i = 0; i < num_objects; ++i) {
                    int index = i * num_objects + j;
                    if (r_data.test(index)) {
                        result_data.set(j);
                        break;
                    }
                }
            }
        }
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role->compute_repr(out);
        out << "," << std::to_string(m_pos) << ")";
    }
};

}

#endif
