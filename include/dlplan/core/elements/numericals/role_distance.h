#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class RoleDistanceNumerical : public Numerical {
private:
    const std::shared_ptr<const Role> m_role_from;
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Role> m_role_to;

    void compute_result(const RoleDenotation& role_from_denot, const RoleDenotation& role_denot, const RoleDenotation& role_to_denot, int& result) const;

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    RoleDistanceNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_from, std::shared_ptr<const Role> role, std::shared_ptr<const Role> role_to);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Numerical& other) const override;

    size_t hash() const;

    int evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::RoleDistanceNumerical>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& left_numerical,
            const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& right_numerical) const;
    };

    template<>
    struct hash<dlplan::core::RoleDistanceNumerical>
    {
        std::size_t operator()(const dlplan::core::RoleDistanceNumerical& numerical) const;
    };
}


#endif
