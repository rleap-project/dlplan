#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include "../../include/dlplan/policy.h"

#include <string>
#include <memory>

using namespace dlplan;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::policy {

class BooleanCondition : public BaseCondition {
protected:
    const std::shared_ptr<const NamedBoolean> m_boolean;

    BooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;

    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class NumericalCondition : public BaseCondition {
protected:
    const std::shared_ptr<const NamedNumerical> m_numerical;

    NumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class PositiveBooleanCondition : public BooleanCondition {
private:
    PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanCondition : public BooleanCondition {
private:
    NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class EqualNumericalCondition : public NumericalCondition {
private:
    EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class GreaterNumericalCondition : public NumericalCondition {
private:
    GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::EqualNumericalCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& right_condition) const;
    };

    template<>
    struct hash<dlplan::policy::PositiveBooleanCondition>
    {
        std::size_t operator()(const dlplan::policy::PositiveBooleanCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::NegativeBooleanCondition>
    {
        std::size_t operator()(const dlplan::policy::NegativeBooleanCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::GreaterNumericalCondition>
    {
        std::size_t operator()(const dlplan::policy::GreaterNumericalCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::EqualNumericalCondition>
    {
        std::size_t operator()(const dlplan::policy::EqualNumericalCondition& condition) const;
    };
}

#endif