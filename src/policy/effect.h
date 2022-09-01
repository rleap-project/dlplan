#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include <string>
#include <memory>

#include "../../include/dlplan/policy.h"


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

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

class NegativeBooleanEffect : public BooleanEffect {
public:
    NegativeBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

class UnchangedBooleanEffect : public BooleanEffect {
public:
    UnchangedBooleanEffect(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

class IncrementNumericalEffect : public NumericalEffect {
public:
    IncrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

class DecrementNumericalEffect : public NumericalEffect {
public:
    DecrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

class UnchangedNumericalEffect : public NumericalEffect {
public:
    UnchangedNumericalEffect(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;

    std::shared_ptr<const BaseEffect> visit(PolicyBuilder& policy_builder) const override;
};

}
}

#endif