#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_H_

#include <unordered_set>
#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include "core.h"
#include "pimpl.h"


namespace dlplan {
namespace core {
    template<typename T>
    class Element;
}
namespace policy {
class RuleImpl;
class PolicyImpl;
class PolicyReaderImpl;
class PolicyWriterImpl;


class PolicyRoot {
private:
    PolicyRoot();
    friend class PolicyImpl;

public:
    ~PolicyRoot();
};




class ByStateInformation {
private:
    std::vector<int> m_data;
};


/**
 * A Feature is shared across all conditions and effects that use it.
 */
template<typename T>
class Feature {
private:
    // no pimpl to save indirection.
    const std::shared_ptr<const PolicyRoot> m_root;
    const int m_index;

    bool m_cached_source;
    bool m_cached_target;
    T m_source_evaluation;
    T m_target_evaluation;

protected:
    Feature(std::shared_ptr<const PolicyRoot> root, int index);

public:
    virtual ~Feature();

    void reset_cached_source_evaluation();
    void reset_cached_target_evaluation();

    virtual T get_source_evaluation(const core::State& source) = 0;
    virtual T get_target_evaluation(const core::State& target) = 0;

    int get_index() const;

    std::string str() const;

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};

// we need to derive to store core::Boolean because core::Element<bool> is abstract type
class BooleanFeature : public Feature<bool> {
private:
    const core::Boolean m_boolean;

private:
    BooleanFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Boolean&& boolean)
      : Feature<bool>(root, index), m_boolean(std::move(boolean)) { }
    friend class PolicyImpl;

public:
    bool get_source_evaluation(const core::State& source) override;
    bool get_target_evaluation(const core::State& target) override;
};

// we need to derive to store core::Numerical because core::Element<int> is abstract type
class NumericalFeature : public Feature<int> {
private:
    const core::Numerical m_numerical;

private:
    NumericalFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Numerical&& numerical)
      : Feature<int>(root, index), m_numerical(std::move(numerical)) { }
    friend class PolicyImpl;

public:
    int get_source_evaluation(const core::State& source) override;
    int get_target_evaluation(const core::State& target) override;
};


/**
 * All different kinds of conditions.
 */
class BaseCondition {
protected:
    virtual std::unique_ptr<BaseCondition> clone_impl() const = 0;

public:
    virtual ~BaseCondition() = default;

    virtual bool is_satisfied(const core::State& state) const = 0;

    virtual std::string str() const = 0;

    virtual std::string compute_repr() const;

    virtual std::unique_ptr<BaseCondition> clone() const;
};


/**
 * All different kinds of effects.
 */
class BaseEffect {
protected:
    virtual std::unique_ptr<BaseEffect> clone_impl() const = 0;

public:
    virtual ~BaseEffect() = default;

    virtual bool is_satisfied(const core::State& source, const core::State& target) const = 0;

    virtual std::string str() const = 0;

    virtual std::string compute_repr() const;

    virtual std::unique_ptr<BaseEffect> clone() const;
};


/**
 * A rule over Boolean and numerical features has form C -> E
 * where C is set of feature conditions and E is set of feature effects
 */
class Rule {
private:
    pimpl<RuleImpl> m_pImpl;

private:
    explicit Rule(
        std::shared_ptr<const PolicyRoot> root,
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);
    friend class PolicyImpl;

public:
    Rule(const Rule& other);
    Rule& operator=(const Rule& other);
    ~Rule();

    bool is_condition_satisfied(const core::State& source) const;
    bool is_effect_satisfied(const core::State& source, const core::State& target) const;

    std::string str() const;

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};

/**
 * A policy is a set of rules over Boolean and numerical features.
 */
class Policy {
private:
    pimpl<PolicyImpl> m_pImpl;

public:
    Policy();
    Policy(const Policy& other);
    Policy& operator=(const Policy& other);
    ~Policy();

    std::shared_ptr<Feature<bool>> add_boolean_feature(core::Boolean b);
    std::shared_ptr<Feature<int>> add_numerical_feature(core::Numerical n);

    /**
     * Uniquely adds a condition (resp. effect) to the policy and returns it.
     * TODO: we must check whether the underlying Feature is part of the policy
     * by comparing the PolicyRoot.
     */
    std::shared_ptr<const BaseCondition> add_b_pos_condition(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseCondition> add_b_neg_condition(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseCondition> add_n_gt_condition(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseCondition> add_n_eq_condition(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_b_pos_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_b_neg_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_b_bot_effect(std::shared_ptr<const Feature<bool>> b);
    std::shared_ptr<const BaseEffect> add_n_inc_effect(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_n_dec_effect(std::shared_ptr<const Feature<int>> n);
    std::shared_ptr<const BaseEffect> add_n_bot_effect(std::shared_ptr<const Feature<int>> n);

    /**
     * Uniquely adds a rule to the policy and returns it.
     * TODO: we must check whether the underlying BaseConditions
     * and BaseEffects are part of the policy by comparing the PolicyRoot.
     */
    std::shared_ptr<const Rule> add_rule(
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);

    /**
     * Clear cached source or target evaluations.
     */
    void reset_cached_source_evaluations();
    void reset_cached_target_evaluations();
    /**
     * Lazily evaluate the state pair.
     * Uses cached source and target evaluations to avoid recomputation.
     */
    bool evaluate_lazy(const core::State& source, const core::State& target);

    std::string str() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
    std::vector<std::shared_ptr<Feature<bool>>> get_boolean_features() const;
    std::vector<std::shared_ptr<Feature<int>>> get_numerical_features() const;
};


/**
 * PolicyReader for reading general policy from bytestream.
 */
class PolicyReader {
private:
    pimpl<PolicyReaderImpl> m_pImpl;

public:
    PolicyReader();
    ~PolicyReader();

    Policy read(const std::string& data) const;
};

/**
 * PolicyWriter for writing general policy to bytestream.
 */
class PolicyWriter {
    pimpl<PolicyWriterImpl> m_pImpl;

public:
    PolicyWriter();
    ~PolicyWriter();

    std::string write(const Policy& policy) const;
};

}
}

//#include "policy.tpp"

#endif