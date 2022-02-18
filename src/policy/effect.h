#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include <string>
#include <memory>

#include "../include/dlplan/policy.h"


namespace dlplan {
namespace evaluator {
    struct EvaluationContext;
}
namespace policy {

template<typename T>
class Effect : public BaseEffect {
protected:
    const std::shared_ptr<const Feature<T>> m_feature;

protected:
    Effect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<T>> feature);

public:
    std::shared_ptr<const Feature<T>> get_feature() const;
};

class PositiveBooleanEffect : public Effect<bool> {
public:
    PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

class NegativeBooleanEffect : public Effect<bool> {
public:
    NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

class UnchangedBooleanEffect : public Effect<bool> {
public:
    UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

class IncrementNumericalEffect : public Effect<int> {
public:
    IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

class DecrementNumericalEffect : public Effect<int> {
public:
    DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

class UnchangedNumericalEffect : public Effect<int> {
public:
    UnchangedNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const override;

    std::string compute_repr() const override;
};

}
}

#include "effect.tpp"

#endif