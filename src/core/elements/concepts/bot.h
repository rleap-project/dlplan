#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::core::element {

class BotConcept : public Concept {
public:
    BotConcept(const VocabularyInfo& vocabulary)
    : Concept(vocabulary) { }

    ConceptDenotation evaluate(const State& state) const override {
        return ConceptDenotation(state.get_instance_info_ref().get_num_objects());
    }

    DENOTS<ConceptDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto concept_cache_entry = cache.m_concept_denotation_cache.find(state, *this);
        auto& status = concept_cache_entry->m_status;
        auto& denotation = concept_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        status = true;
        return &denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    static std::string get_name() {
        return "c_bot";
    }
};

}

#endif
