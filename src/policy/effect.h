#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include <string>
#include <memory>

#include "../include/dlplan/policy.h"


namespace dlplan::policy {
template<typename T>
class Feature;

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
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

class NegativeBooleanEffect : public Effect<bool> {
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

class UnchangedBooleanEffect : public Effect<bool> {
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

class IncrementNumericalEffect : public Effect<int> {
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

class DecrementNumericalEffect : public Effect<int> {
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

class UnchangedNumericalEffect : public Effect<int> {
protected:
    std::unique_ptr<BaseEffect> clone_impl() const override;

public:
    UnchangedNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const override;

    std::string compute_repr() const override;
};

}

#include "effect.tpp"

#endif