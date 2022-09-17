#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SomeConcept : public Concept {
private:
    ConceptDenotation compute_result(RoleDenotation&& role_denot, ConceptDenotation&& concept_denot, ConceptDenotation&& result) const {
        // find examples a : exists b . (a,b) in R and b in C
        for (const auto& pair : role_denot) {
            if (concept_denot.contains(pair.second)) {
                result.insert(pair.first);
            }
        }
        return result;
    }

protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    SomeConcept(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Concept(vocabulary), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("SomeConcept::SomeConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto bot_concept = ConceptDenotation(state.get_instance_info()->get_num_objects());
        return compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            std::move(bot_concept));
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_concept_denotation_cache.count(state, *this)) {
            return cache.m_concept_denotation_cache.find(state, *this);
        }
        auto bot_concept = ConceptDenotation(state.get_instance_info()->get_num_objects());
        auto result = compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            std::move(bot_concept));
        cache.m_concept_denotation_cache.insert(state, *this, result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ",";
        m_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "c_some";
    }
};

}

#endif
