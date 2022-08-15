#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_H_

#include <unordered_set>
#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include "evaluator.h"
#include "core.h"
#include "pimpl.h"


namespace dlplan::policy {
class RuleImpl;
class PolicyImpl;
class PolicyBuilderImpl;
class PolicyReaderImpl;
class PolicyWriterImpl;
class EvaluationContext;


class PolicyRoot {
public:
    PolicyRoot();
    ~PolicyRoot();
};


/**
 * A BaseFeature is shared across all conditions and effects that use it.
 * The underlying type of feature is abstract.
 */
class BaseFeature {
private:
    std::shared_ptr<const PolicyRoot> m_root;
    int m_index;

protected:
    BaseFeature(std::shared_ptr<const PolicyRoot> root, int index);

public:
    // BaseFeature is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because BaseFeature is always const
    BaseFeature(const BaseFeature& other) = delete;
    BaseFeature& operator=(const BaseFeature& other) = delete;
    BaseFeature(BaseFeature&& other);
    BaseFeature& operator=(BaseFeature&& other);
    virtual ~BaseFeature();

    virtual std::string compute_repr() const = 0;

    int get_index() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};


template<typename T>
class Feature : public BaseFeature {
protected:
    Feature(std::shared_ptr<const PolicyRoot> root, int index);

public:
    Feature(const Feature& other) = delete;
    Feature& operator=(const Feature& other) = delete;
    Feature(Feature&& other) = default;
    Feature& operator=(Feature&& other) = default;
    ~Feature() {}

    virtual T evaluate(evaluator::EvaluationContext& context) const = 0;
};


class BooleanFeature : public Feature<bool> {
private:
    core::Boolean m_boolean;

private:
    BooleanFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Boolean&& boolean);
    friend class PolicyBuilderImpl;

public:
    // BooleanFeature is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because BooleanFeatures is always const
    BooleanFeature(const BooleanFeature& other) = delete;
    BooleanFeature& operator=(const BooleanFeature& other) = delete;
    BooleanFeature(BooleanFeature&& other);
    BooleanFeature& operator=(BooleanFeature&& other);
    ~BooleanFeature() override;

    bool evaluate(evaluator::EvaluationContext& context) const override;

    std::string compute_repr() const override;

    std::string str() const;
};


class NumericalFeature : public Feature<int> {
private:
    core::Numerical m_numerical;

private:
    NumericalFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Numerical&& numerical);
    friend class PolicyBuilderImpl;

public:
    // NumericalFeature is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because NumericalFeatures is always const
    NumericalFeature(const NumericalFeature& other) = delete;
    NumericalFeature& operator=(const NumericalFeature& other) = delete;
    NumericalFeature(NumericalFeature&& other);
    NumericalFeature& operator=(NumericalFeature&& other);
    ~NumericalFeature() override;

    int evaluate(evaluator::EvaluationContext& context) const override;

    std::string compute_repr() const override;

    std::string str() const;
};


/**
 * All different kinds of conditions.
 */
class BaseCondition {
private:
    std::shared_ptr<const PolicyRoot> m_root;
    std::shared_ptr<const BaseFeature> m_base_feature;

protected:
    BaseCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const BaseFeature> base_feature);

public:
    // Condition is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because Condition is always const
    BaseCondition(const BaseCondition& other) = delete;
    BaseCondition& operator=(const BaseCondition& other) = delete;
    BaseCondition(BaseCondition&& other);
    BaseCondition& operator=(BaseCondition&& other);
    virtual ~BaseCondition();

    virtual bool evaluate(evaluator::EvaluationContext& source_context) const = 0;

    virtual std::string compute_repr() const = 0;

    std::string str() const;

    std::shared_ptr<const BaseFeature> get_base_feature() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};


/**
 * All different kinds of effects.
 */
class BaseEffect {
private:
    std::shared_ptr<const PolicyRoot> m_root;
    std::shared_ptr<const BaseFeature> m_base_feature;

protected:
    BaseEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const BaseFeature> base_feature);

public:
    // Effect is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because Effect is always const
    BaseEffect(const BaseEffect& other) = delete;
    BaseEffect& operator=(const BaseEffect& other) = delete;
    BaseEffect(BaseEffect&& other);
    BaseEffect& operator=(BaseEffect&& other);
    virtual ~BaseEffect();

    virtual bool evaluate(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const = 0;

    virtual std::string compute_repr() const = 0;

    std::string str() const;

    std::shared_ptr<const BaseFeature> get_base_feature() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};


/**
 * A rule over Boolean and numerical features has form C -> E
 * where C is set of feature conditions and E is set of feature effects
 */
class Rule {
private:
    std::shared_ptr<const PolicyRoot> m_root;

    std::vector<std::shared_ptr<const BaseCondition>> m_conditions;
    std::vector<std::shared_ptr<const BaseEffect>> m_effects;

private:
    Rule(std::shared_ptr<const PolicyRoot> root,
        std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
        std::vector<std::shared_ptr<const BaseEffect>>&& effects);
    friend class PolicyBuilderImpl;

public:
    // Rule is not copieable because it must live in the cache.
    // For construction we need it to be moveable.
    // However, moving cannot be abused because Rule is always const
    Rule(const Rule& other) = delete;
    Rule& operator=(const Rule& other) = delete;
    Rule(Rule&& other);
    Rule& operator=(Rule&& other);
    ~Rule();

    bool evaluate_conditions(evaluator::EvaluationContext& source_context) const;
    bool evaluate_effects(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context) const;

    std::string compute_repr() const;

    std::string str() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<const BaseCondition>> get_conditions() const;
    std::vector<std::shared_ptr<const BaseEffect>> get_effects() const;
};


/**
 * A policy is a set of rules over Boolean and numerical features.
 */
class Policy {
private:
    std::shared_ptr<const PolicyRoot> m_root;

    std::vector<std::shared_ptr<const BooleanFeature>> m_boolean_features;
    std::vector<std::shared_ptr<const NumericalFeature>> m_numerical_features;
    std::vector<std::shared_ptr<const Rule>> m_rules;

private:
    Policy(std::shared_ptr<const PolicyRoot> root,
           std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
           std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
           std::vector<std::shared_ptr<const Rule>>&& rules);
    friend class PolicyBuilderImpl;

public:
    Policy(const Policy& other);
    Policy& operator=(const Policy& other);
    Policy(Policy&& other);
    Policy& operator=(Policy&& other);
    ~Policy();

    /**
     * Approach 1: naive approach to evaluate (s,s')
     */
    std::shared_ptr<const Rule> evaluate_lazy(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context);

    /**
     * Approach 2: optimized approach for evaluating pairs with similar source state s, i.e., (s,s1), (s,s2), ..., (s,sn)
     */
    std::vector<std::shared_ptr<const Rule>> evaluate_conditions_eager(evaluator::EvaluationContext& source_context);
    std::shared_ptr<const Rule> evaluate_effects_lazy(evaluator::EvaluationContext& source_context, evaluator::EvaluationContext& target_context, const std::vector<std::shared_ptr<const Rule>>& rules);

    std::string compute_repr() const;

    std::string str() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<const Rule>> get_rules() const;
    std::vector<std::shared_ptr<const BooleanFeature>> get_boolean_features() const;
    std::vector<std::shared_ptr<const NumericalFeature>> get_numerical_features() const;
};


class PolicyBuilder {
private:
    pimpl<PolicyBuilderImpl> m_pImpl;

public:
    PolicyBuilder();
    PolicyBuilder(const PolicyBuilder& other);
    PolicyBuilder& operator=(const PolicyBuilder& other);
    PolicyBuilder(PolicyBuilder&& other);
    PolicyBuilder& operator=(PolicyBuilder&& other);
    ~PolicyBuilder();

    /**
     * Uniquely adds features.
     */
    std::shared_ptr<const BooleanFeature> add_boolean_feature(core::Boolean b);
    std::shared_ptr<const NumericalFeature> add_numerical_feature(core::Numerical n);

    /**
     * Uniquely adds a condition (resp. effect) to the policy and returns it.
     */
    std::shared_ptr<const BaseCondition> add_pos_condition(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseCondition> add_neg_condition(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseCondition> add_gt_condition(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseCondition> add_eq_condition(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_pos_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_neg_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_bot_effect(std::shared_ptr<const BooleanFeature> b);
    std::shared_ptr<const BaseEffect> add_inc_effect(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_dec_effect(std::shared_ptr<const NumericalFeature> n);
    std::shared_ptr<const BaseEffect> add_bot_effect(std::shared_ptr<const NumericalFeature> n);

    /**
     * Uniquely adds a rule to the policy and returns it.
     */
    std::shared_ptr<const Rule> add_rule(
        std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
        std::vector<std::shared_ptr<const BaseEffect>>&& effects);

    /**
     * TODO: - sort features by their runtime complexity.
     *       - sort rules by sum of runtime complexities of underlying features.
     *       - compute invariants
     */
    Policy get_result();
};


/**
 * PolicyReader for reading general policy from bytestream.
 */
class PolicyReader {
private:
    pimpl<PolicyReaderImpl> m_pImpl;

public:
    PolicyReader();
    PolicyReader(const PolicyReader& other);
    PolicyReader& operator=(const PolicyReader& other);
    PolicyReader(PolicyReader&& other);
    PolicyReader& operator=(PolicyReader&& other);
    ~PolicyReader();

    Policy read(const std::string& data, core::SyntacticElementFactory& factory) const;
};

/**
 * PolicyWriter for writing general policy to bytestream.
 */
class PolicyWriter {
    pimpl<PolicyWriterImpl> m_pImpl;

public:
    PolicyWriter();
    PolicyWriter(const PolicyWriter& other);
    PolicyWriter& operator=(const PolicyWriter& other);
    PolicyWriter(PolicyWriter&& other);
    PolicyWriter& operator=(PolicyWriter&& other);
    ~PolicyWriter();

    std::string write(const Policy& policy) const;
};

}

#include "policy.tpp"

#endif