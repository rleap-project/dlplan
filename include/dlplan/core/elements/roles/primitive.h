#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class PrimitiveRole : public Role {
private:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

    void compute_result(const State& state, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    PrimitiveRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos_1, int pos_2);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Role& other) const override;
    size_t hash_impl() const override;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;

    const Predicate& get_predicate() const;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::PrimitiveRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveRole>& left_role,
            const std::shared_ptr<const dlplan::core::PrimitiveRole>& right_role) const;
    };

    template<>
    struct hash<dlplan::core::PrimitiveRole>
    {
        std::size_t operator()(const dlplan::core::PrimitiveRole& role) const;
    };
}

#endif
