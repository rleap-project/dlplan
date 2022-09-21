#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SomeConcept : public Concept {
private:
    void compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, ConceptDenotation& result) const {
        // find examples a : exists b . (a,b) in R and b in C
        for (const auto& pair : role_denot) {
            if (concept_denot.contains(pair.second)) {
                result.insert(pair.first);
            }
        }
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
        auto denotation = ConceptDenotation(state.get_instance_info_ref().get_num_objects());
        compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    DENOTS<ConceptDenotation> evaluate(const States& states, DenotationsCaches& caches) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_role->evaluate(state, cache),
            *m_concept->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
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
