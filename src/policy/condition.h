#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include "../../include/dlplan/policy.h"

#include <string>
#include <memory>


namespace dlplan::policy {

class BooleanCondition : public BaseCondition {
protected:
    const std::shared_ptr<const core::Boolean> m_boolean;

protected:
    BooleanCondition(std::shared_ptr<const core::Boolean> boolean);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;
    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class NumericalCondition : public BaseCondition {
protected:
    const std::shared_ptr<const core::Numerical> m_numerical;

protected:
    NumericalCondition(std::shared_ptr<const core::Numerical> numerical);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;
    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class PositiveBooleanCondition : public BooleanCondition {
public:
    PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanCondition : public BooleanCondition {
public:
    NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean_feature);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class EqualNumericalCondition : public NumericalCondition {
public:
    EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class GreaterNumericalCondition : public NumericalCondition {
public:
    GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical_feature);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}

#endif