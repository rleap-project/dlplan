#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {
class BotConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::BotConcept& concept, const unsigned int version);
}


namespace dlplan::core {
class BotConcept : public Concept {
private:
    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        return ConceptDenotation(state.get_instance_info()->get_objects().size());
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotations.push_back(caches.get_concept_denotation_cache().insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BotConcept& concept, const unsigned int version);

public:
    BotConcept() : Concept() { }
    BotConcept(std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : Concept(vocabulary_info, true) { }

    ConceptDenotation evaluate(const State& state) const override {
        return ConceptDenotation(state.get_instance_info()->get_objects().size());
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }

    static std::string get_name() {
        return "c_bot";
    }
};

}

#endif
