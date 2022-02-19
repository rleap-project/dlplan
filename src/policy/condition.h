#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include <string>
#include <memory>

#include "../include/dlplan/policy.h"


namespace dlplan::policy {
template<typename T>
class Feature;


template<typename T>
class Condition : public BaseCondition {
protected:
    const std::shared_ptr<const Feature<T>> m_feature;

protected:
    Condition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<T>> feature);

public:
    std::shared_ptr<const Feature<T>> get_feature() const;
};


class PositiveBooleanCondition : public Condition<bool> {
public:
    PositiveBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class NegativeBooleanCondition : public Condition<bool> {
public:
    NegativeBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class EqualNumericalCondition : public Condition<int> {
public:
    EqualNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class GreaterNumericalCondition : public Condition<int> {
public:
    GreaterNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

}

#include "condition.tpp"

#endif