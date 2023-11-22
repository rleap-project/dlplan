#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_

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
class ProjectionConcept : public Concept {
private:
    std::shared_ptr<const Role> m_role;
    int m_pos;

    void compute_result(const RoleDenotation& denot, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    ProjectionConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const std::shared_ptr<const Role>& role, int pos);

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
    struct less<std::shared_ptr<const dlplan::core::ProjectionConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::ProjectionConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::ProjectionConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::ProjectionConcept>
    {
        std::size_t operator()(const dlplan::core::ProjectionConcept& concept_) const;
    };
}

#endif
