#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include <string>
#include <memory>

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

class BooleanCondition : public BaseCondition {
protected:
    const std::shared_ptr<const core::Boolean> m_boolean;

protected:
    BooleanCondition(std::shared_ptr<const core::Boolean> boolean);
};


class NumericalCondition : public BaseCondition {
protected:
    const std::shared_ptr<const core::Numerical> m_numerical;

protected:
    NumericalCondition(std::shared_ptr<const core::Numerical> numerical);
};


class PositiveBooleanCondition : public BooleanCondition {
public:
    PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class NegativeBooleanCondition : public BooleanCondition {
public:
    NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class EqualNumericalCondition : public NumericalCondition {
public:
    EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

class GreaterNumericalCondition : public NumericalCondition {
public:
    GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(evaluator::EvaluationContext& source_context) const override;

    std::string compute_repr() const override;
};

}

#endif