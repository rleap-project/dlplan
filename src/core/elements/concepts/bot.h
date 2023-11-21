#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
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
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    BotConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
    : Concept(vocabulary_info, index, true) { }

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const BotConcept&>(other);
            return m_is_static == other_derived.m_is_static;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static);
    }

    ConceptDenotation evaluate(const State& state) const override {
        return ConceptDenotation(state.get_instance_info()->get_objects().size());
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_bot";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::BotConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::BotConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::BotConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::BotConcept>
    {
        std::size_t operator()(const dlplan::core::BotConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
