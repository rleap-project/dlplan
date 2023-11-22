#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_TOP_H_

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
class TopConcept : public Concept {
private:
    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    TopConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info);

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
    struct less<std::shared_ptr<const dlplan::core::TopConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TopConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::TopConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::TopConcept>
    {
        std::size_t operator()(const dlplan::core::TopConcept& concept_) const;
    };
}

#endif
