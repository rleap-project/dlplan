#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_BOT_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class BotConcept : public Concept {
private:
    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    BotConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Concept& other) const override;

    size_t hash_impl() const override;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::BotConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::BotConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::BotConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::BotConcept>
    {
        std::size_t operator()(const dlplan::core::BotConcept& concept_) const;
    };
}

#endif
