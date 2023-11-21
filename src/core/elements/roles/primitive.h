#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../../../../include/dlplan/core.h"

#include <sstream>
#include <memory>


namespace dlplan::utils {
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
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override;
    size_t hash() const;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;

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
