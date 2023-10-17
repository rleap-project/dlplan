/// @brief Implements the general policy language.

#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_H_

#include <unordered_set>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "include/dlplan/common/parsers/config.hpp"
#include "core.h"
#include "utils/pimpl.h"


// Forward declarations of this header
namespace dlplan::policy {
class PolicyFactoryImpl;
class BaseCondition;
class BaseEffect;
class Rule;
class Policy;
class PolicyFactory;
}


// Forward declarations of template spezializations for serialization
namespace boost::serialization {
    class access;

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::BaseCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::BaseCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::BaseCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::BaseEffect& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::BaseEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::BaseEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::Rule& t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::Policy& t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::PolicyFactory& t, const unsigned int version);
}


namespace dlplan::policy {
/// @brief Sort elements in policy by their evaluate time score.
/// @tparam T
template<typename T>
struct ScoreCompare {
    bool operator()(
        const std::shared_ptr<T>& l,
        const std::shared_ptr<T>& r) const {
        if (l->compute_evaluate_time_score() == r->compute_evaluate_time_score()) {
            return l->compute_repr() < r->compute_repr();
        }
        return l->compute_evaluate_time_score() < r->compute_evaluate_time_score();
    }
};

using Booleans = std::set<std::shared_ptr<const dlplan::core::Boolean>, ScoreCompare<const dlplan::core::Boolean>>;
using Numericals = std::set<std::shared_ptr<const dlplan::core::Numerical>, ScoreCompare<const dlplan::core::Numerical>>;
using Conditions = std::set<std::shared_ptr<const BaseCondition>, ScoreCompare<const BaseCondition>>;
using Effects = std::set<std::shared_ptr<const BaseEffect>, ScoreCompare<const BaseEffect>>;
using Rules = std::set<std::shared_ptr<const Rule>, ScoreCompare<const Rule>>;
using Policies = std::set<std::shared_ptr<const Policy>, ScoreCompare<const Policy>>;

using StatePair = std::pair<dlplan::core::State, dlplan::core::State>;
using StatePairs = std::vector<StatePair>;

using ConditionIndex = int;
using EffectIndex = int;
using RuleIndex = int;
using PolicyIndex = int;


/// @brief Represents the abstract base class of a feature condition and
///        provides functionality to access its underlying data and for
///        the evaluation on a state.
class BaseCondition {
protected:
    int m_index;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BaseCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BaseCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BaseCondition* t, const unsigned int version);

public:
    explicit BaseCondition(ConditionIndex index);
    BaseCondition(const BaseCondition& other) = delete;
    BaseCondition& operator=(const BaseCondition& other) = delete;
    BaseCondition(BaseCondition&& other) = delete;
    BaseCondition& operator=(BaseCondition&& other) = delete;
    virtual ~BaseCondition();

    virtual bool evaluate(const core::State& source_state) const = 0;
    virtual bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const = 0;

    virtual std::string compute_repr() const = 0;
    virtual std::string str() const = 0;

    /// @brief Computes a time score for evaluating this condition relative to other conditions.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    virtual int compute_evaluate_time_score() const = 0;

    virtual std::shared_ptr<const core::Boolean> get_boolean() const = 0;
    virtual std::shared_ptr<const core::Numerical> get_numerical() const = 0;
    ConditionIndex get_index() const;
};


/// @brief Represents the abstract base class of a feature effect and
///        provides functionality to access its underlying data and for
///        the evaluation on a pair of states.
class BaseEffect {
protected:
    int m_index;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BaseEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BaseEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BaseEffect* t, const unsigned int version);

public:
    explicit BaseEffect(EffectIndex index);
    BaseEffect(const BaseEffect& other) = delete;
    BaseEffect& operator=(const BaseEffect& other) = delete;
    BaseEffect(BaseEffect&& other) = delete;
    BaseEffect& operator=(BaseEffect&& other) = delete;
    virtual ~BaseEffect();

    virtual bool evaluate(const core::State& source_state, const core::State& target_state) const = 0;
    virtual bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const = 0;

    virtual std::string compute_repr() const = 0;
    virtual std::string str() const = 0;

    /// @brief Computes a time score for evaluating this effect relative to other effects.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    virtual int compute_evaluate_time_score() const = 0;

    EffectIndex get_index() const;
    virtual std::shared_ptr<const core::Boolean> get_boolean() const = 0;
    virtual std::shared_ptr<const core::Numerical> get_numerical() const = 0;
};


/// @brief Implements a policy rule of the form C->E with where C is a set of
///        feature conditions and E is a set of feature effects. Provides
///        functionality to access its underlying data and for the evaluation
///        on a pair of states.
class Rule {
private:
    Conditions m_conditions;
    Effects m_effects;
    RuleIndex m_index;

    /// @brief Constructor for serialization.
    Rule();

    Rule(const Conditions& conditions, const Effects& effects, RuleIndex index);

    friend class PolicyFactoryImpl;
    friend class boost::serialization::access;
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Rule& t, const unsigned int version);

public:
    Rule(const Rule& other) = delete;
    Rule& operator=(const Rule& other) = delete;
    Rule(Rule&& other) = delete;
    Rule& operator=(Rule&& other) = delete;
    ~Rule();

    bool evaluate_conditions(const core::State& source_state) const;
    bool evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const;

    std::string compute_repr() const;
    std::string str() const;

    /// @brief Computes a time score for evaluating this rule relative to other rules.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    int compute_evaluate_time_score() const;

    RuleIndex get_index() const;
    const Conditions& get_conditions() const;
    const Effects& get_effects() const;
};


/// @brief Implements a policy consisting of a set of rules over a set of
///        Boolean and a set of numerical features. Provides functionality
///        to access its underlying data and for the evaluation on a pair of
///        states.
class Policy {
private:
    Booleans m_booleans;
    Numericals m_numericals;
    Rules m_rules;
    int m_index;

    /// @brief Constructor for serialization.
    Policy();

    Policy(const Rules& rules, PolicyIndex index);

    friend class PolicyFactoryImpl;
    friend class boost::serialization::access;
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Policy& t, const unsigned int version);

public:
    Policy(const Policy& other);
    Policy& operator=(const Policy& other);
    Policy(Policy&& other);
    Policy& operator=(Policy&& other);
    ~Policy();

    /**
     * Approach 1: naive approach to evaluate (s,s')
     */
    std::shared_ptr<const Rule> evaluate(const core::State& source_state, const core::State& target_state) const;
    std::shared_ptr<const Rule> evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const;

    /**
     * Approach 2: optimized approach for evaluating pairs with similar source state s, i.e., (s,s1), (s,s2), ..., (s,sn)
     */
    std::vector<std::shared_ptr<const Rule>> evaluate_conditions(const core::State& source_state) const;
    std::vector<std::shared_ptr<const Rule>> evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const;
    std::shared_ptr<const Rule> evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules) const;
    std::shared_ptr<const Rule> evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules, core::DenotationsCaches& caches) const;

    std::string compute_repr() const;
    std::string str() const;

    /// @brief Computes a time score for evaluating this condition relative to other conditions.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    int compute_evaluate_time_score() const;

    PolicyIndex get_index() const;
    const Booleans& get_booleans() const;
    const Numericals& get_numericals() const;
    const Rules& get_rules() const;
};


/// @brief Provides functionality for the syntactically unique creation of
///        conditions, effects, rules, and policies.
class PolicyFactory {
private:
    dlplan::utils::pimpl<PolicyFactoryImpl> m_pImpl;

    /// @brief Constructor for serialization.
    PolicyFactory();

    friend class boost::serialization::access;
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PolicyFactory& t, const unsigned int version);

public:
    explicit PolicyFactory(std::shared_ptr<core::SyntacticElementFactory> element_factory);
    PolicyFactory(const PolicyFactory& other);
    PolicyFactory& operator=(const PolicyFactory& other);
    PolicyFactory(PolicyFactory&& other);
    PolicyFactory& operator=(PolicyFactory&& other);
    ~PolicyFactory();

    /**
     * Parses a policy from its textual description
     */
    std::shared_ptr<const Policy> parse_policy(
        const std::string& description,
        const std::string& filename="");

    std::shared_ptr<const Policy> parse_policy(
        common::parsers::iterator_type& iter, common::parsers::iterator_type end,
        const std::string& filename="");

    /**
     * Uniquely adds a condition (resp. effect) and returns it.
     */
    std::shared_ptr<const BaseCondition> make_pos_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> make_neg_condition(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_pos_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_neg_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const core::Numerical>& numerical);

    /**
     * Uniquely adds a rule and returns it.
     */
    std::shared_ptr<const Rule> make_rule(
        const Conditions& conditions,
        const Effects& effects);

    /**
     * Uniquely adds a policy and returns it.
     */
    std::shared_ptr<const Policy> make_policy(
        const Rules& rules);

    std::shared_ptr<core::SyntacticElementFactory> get_element_factory() const;
};


/// @brief Provides functionality for the syntactic and empirical minimization
///        of policies.
class PolicyMinimizer {
public:
    PolicyMinimizer();
    PolicyMinimizer(const PolicyMinimizer& other);
    PolicyMinimizer& operator=(const PolicyMinimizer& other);
    PolicyMinimizer(PolicyMinimizer&& other);
    PolicyMinimizer& operator=(PolicyMinimizer&& other);
    ~PolicyMinimizer();

    std::shared_ptr<const Policy> minimize(
        const std::shared_ptr<const Policy>& policy,
        PolicyFactory& policy_factory) const;
    std::shared_ptr<const Policy> minimize(
        const std::shared_ptr<const Policy>& policy,
        const StatePairs& true_state_pairs,
        const StatePairs& false_state_pairs,
        PolicyFactory& policy_factory) const;
};


}

#endif