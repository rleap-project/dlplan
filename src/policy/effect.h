#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include "../../include/dlplan/policy.h"

#include <string>
#include <memory>


namespace dlplan {
namespace evaluator {
    struct EvaluationContext;
}
namespace policy {

class BooleanEffect : public BaseEffect {
protected:
    const std::shared_ptr<const core::Boolean> m_boolean;

protected:
    BooleanEffect(std::shared_ptr<const core::Boolean> boolean);
};


class NumericalEffect : public BaseEffect {
protected:
    const std::shared_ptr<const core::Numerical> m_numerical;

protected:
    NumericalEffect(std::shared_ptr<const core::Numerical> numerical);
};


class PositiveBooleanEffect : public BooleanEffect {
public:
    PositiveBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

class NegativeBooleanEffect : public BooleanEffect {
public:
    NegativeBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

class UnchangedBooleanEffect : public BooleanEffect {
public:
    UnchangedBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

class IncrementNumericalEffect : public NumericalEffect {
public:
    IncrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

class DecrementNumericalEffect : public NumericalEffect {
public:
    DecrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

class UnchangedNumericalEffect : public NumericalEffect {
public:
    UnchangedNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;

    std::shared_ptr<const BaseEffect> copy_to_builder(PolicyBuilder& policy_builder) const override;
};

}
}

#endif