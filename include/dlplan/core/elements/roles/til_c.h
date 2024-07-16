#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_TIL_C_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_TIL_C_H_

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
class TilCRole : public Role {
private:
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const RoleDenotation &role_denot, const ConceptDenotation &concept_denot, RoleDenotation &result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;
    TilCRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Role& other) const override;

    size_t hash_impl() const override;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::TilCRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TilCRole>& left_role,
            const std::shared_ptr<const dlplan::core::TilCRole>& right_role) const;
    };

    template<>
    struct hash<dlplan::core::TilCRole>
    {
        std::size_t operator()(const dlplan::core::TilCRole& role) const;
    };
}

#endif
