#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_

#include <sstream>

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class ProjectionConcept : public Concept {
private:
    ConceptDenotation compute_result(RoleDenotation&& denot, ConceptDenotation&& result) const {
        for (const auto pair : denot) {
            if (m_pos == 0) result.insert(pair.first);
            else if (m_pos == 1) result.insert(pair.second);
        }
        return result;
    }

protected:
    const Role_Ptr m_role;
    const int m_pos;

public:
    ProjectionConcept(const VocabularyInfo& vocabulary, const Role_Ptr& role, int pos)
    : Concept(vocabulary), m_role(role), m_pos(pos) {
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
        if (!role) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto bot_concept = ConceptDenotation(state.get_instance_info()->get_num_objects());
        return compute_result(
            m_role->evaluate(state),
            std::move(bot_concept));
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_concept_denotation_cache.count(state, *this)) {
            return cache.m_concept_denotation_cache.find(state, *this);
        }
        auto bot_concept = ConceptDenotation(state.get_instance_info()->get_num_objects());
        auto result = compute_result(
            m_role->evaluate(state, cache),
            std::move(bot_concept));
        cache.m_concept_denotation_cache.insert(state, *this, result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << "," << std::to_string(m_pos) << ")";
    }

    static std::string get_name() {
        return "c_projection";
    }
};

}

#endif
