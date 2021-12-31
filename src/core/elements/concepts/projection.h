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

    ConceptDenotation evaluate(const State& state) const override {
        const auto r = m_role->evaluate(state);
        const auto& r_data = r.get_const_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        ConceptDenotation result(num_objects);
        auto& result_data = result.get_data();
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
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << "," << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}

#endif
