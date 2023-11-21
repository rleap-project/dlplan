#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_EFFECT_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_EFFECT_H_

#include "../policy.h"

#include <utility>
#include <string>
#include <memory>


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::policy {
class BooleanEffect : public BaseEffect {
protected:
    std::shared_ptr<const NamedBoolean> m_boolean;

    BooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class NumericalEffect : public BaseEffect {
protected:
    std::shared_ptr<const NamedNumerical> m_numerical;

    NumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class PositiveBooleanEffect : public BooleanEffect {
private:
    PositiveBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanEffect : public BooleanEffect {
private:
    NegativeBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedBooleanEffect : public BooleanEffect {
private:
    UnchangedBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class IncrementNumericalEffect : public NumericalEffect {
private:
    IncrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class DecrementNumericalEffect : public NumericalEffect {
private:
    DecrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedNumericalEffect : public NumericalEffect {
private:
    UnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const;
    };

    template<>
    struct hash<dlplan::policy::PositiveBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::PositiveBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::NegativeBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::NegativeBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::IncrementNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::IncrementNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::DecrementNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::DecrementNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedNumericalEffect& effect) const;
    };
}


#endif