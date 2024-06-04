/// @brief Implements the general policy language.

#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_H_

#include <unordered_set>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "common/base.h"
#include "common/parsers/config.hpp"
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
class BaseConditionVisitor;
class BaseEffectVisitor;
}


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
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
            return l->get_index() < r->get_index();
        }
        return l->compute_evaluate_time_score() < r->compute_evaluate_time_score();
    }
};


using Rules = std::set<std::shared_ptr<const Rule>, ScoreCompare<const Rule>>;
using Policies = std::set<std::shared_ptr<const Policy>, ScoreCompare<const Policy>>;

using StatePair = std::pair<dlplan::core::State, dlplan::core::State>;
using StatePairs = std::vector<StatePair>;


/// @brief Wrappers around core elements to add an additional key
///        that can potentially add some more human readable meaning.
template<typename Element>
class NamedElement : public Base<NamedElement<Element>> {
private:
    std::string m_key;
    std::shared_ptr<const Element> m_element;

    NamedElement(int identifier, const std::string& key, std::shared_ptr<const Element> element)
        : Base<NamedElement<Element>>(identifier), m_key(key), m_element(element) { }

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    NamedElement(const NamedElement& other) = default;
    NamedElement& operator=(const NamedElement& other) = default;
    NamedElement(NamedElement&& other) = default;
    NamedElement& operator=(NamedElement&& other) = default;
    ~NamedElement() = default;

    bool are_equal_impl(const NamedElement& other) const {
        if (this != &other) {
            return (m_key == other.m_key)
                && (m_element == other.m_element);
        }
        return true;
    }
    size_t hash_impl() const { return hash_combine(m_key, m_element); }
    void str_impl(std::stringstream& out) const { out << "(" << m_key << " \"" << m_element->str() << "\")"; }

    int compute_evaluate_time_score() const { return m_element->compute_evaluate_time_score(); }

    const std::string& get_key() const { return m_key; }
    const std::shared_ptr<const Element>& get_element() const { return m_element; }
};


using NamedBoolean = NamedElement<core::Boolean>;
using NamedNumerical = NamedElement<core::Numerical>;
using NamedConcept = NamedElement<core::Concept>;
using NamedRole = NamedElement<core::Role>;

using Booleans = std::set<std::shared_ptr<const NamedBoolean>, ScoreCompare<const NamedBoolean>>;
using Numericals = std::set<std::shared_ptr<const NamedNumerical>, ScoreCompare<const NamedNumerical>>;
using Concepts = std::set<std::shared_ptr<const NamedConcept>, ScoreCompare<const NamedConcept>>;
using Roles = std::set<std::shared_ptr<const NamedRole>, ScoreCompare<const NamedRole>>;

/// @brief Represents the abstract base class of a feature condition and
///        provides functionality to access its underlying data and for
///        the evaluation on a state.
class BaseCondition : public Base<BaseCondition> {
protected:
    explicit BaseCondition(int identifier);

    // protected copy/move to prevent accidental object slicing when passed by value
    BaseCondition(const BaseCondition& other) = default;
    BaseCondition& operator=(const BaseCondition& other) = default;
    BaseCondition(BaseCondition&& other) = default;
    BaseCondition& operator=(BaseCondition&& other) = default;

public:
    virtual ~BaseCondition();

    virtual bool are_equal_impl(const BaseCondition& other) const = 0;
    virtual size_t hash_impl() const = 0;
    virtual void str_impl(std::stringstream& out) const = 0;
    virtual int compute_evaluate_time_score() const = 0;

    virtual bool evaluate(const core::State& source_state) const = 0;
    virtual bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const = 0;
    virtual void accept(BaseConditionVisitor& visitor) const = 0;
};


/// @brief Represents the abstract base class of a feature effect and
///        provides functionality to access its underlying data and for
///        the evaluation on a pair of states.
class BaseEffect : public Base<BaseEffect> {
protected:
    explicit BaseEffect(int identifier);

    // protected copy/move to prevent accidental object slicing when passed by value
    BaseEffect(const BaseEffect& other) = default;
    BaseEffect& operator=(const BaseEffect& other) = default;
    BaseEffect(BaseEffect&& other) = default;
    BaseEffect& operator=(BaseEffect&& other) = default;

public:
    virtual ~BaseEffect();

    virtual bool are_equal_impl(const BaseEffect& other) const = 0;
    virtual size_t hash_impl() const = 0;
    virtual void str_impl(std::stringstream& out) const = 0;
    virtual int compute_evaluate_time_score() const = 0;

    virtual bool evaluate(const core::State& source_state, const core::State& target_state) const = 0;
    virtual bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const = 0;
    virtual void accept(BaseEffectVisitor& visitor) const = 0;
};

using Conditions = std::set<std::shared_ptr<const BaseCondition>, ScoreCompare<const BaseCondition>>;
using Effects = std::set<std::shared_ptr<const BaseEffect>, ScoreCompare<const BaseEffect>>;


/// @brief Implements a policy rule of the form C->E with where C is a set of
///        feature conditions and E is a set of feature effects. Provides
///        functionality to access its underlying data and for the evaluation
///        on a pair of states.
class Rule : public Base<Rule> {
private:
    Conditions m_conditions;
    Effects m_effects;

    Rule(int identifier, const Conditions& conditions, const Effects& effects);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    Rule(const Rule& other);
    Rule& operator=(const Rule& other);
    Rule(Rule&& other);
    Rule& operator=(Rule&& other);
    ~Rule();

    bool are_equal_impl(const Rule& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out) const;
    int compute_evaluate_time_score() const;

    bool evaluate_conditions(const core::State& source_state) const;
    bool evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const;

    const Conditions& get_conditions() const;
    const Effects& get_effects() const;
};


/// @brief Implements a policy consisting of a set of rules over a set of
///        Boolean and a set of numerical features. Provides functionality
///        to access its underlying data and for the evaluation on a pair of
///        states.
class Policy : public Base<Policy> {
private:
    Booleans m_booleans;
    Numericals m_numericals;
    Concepts m_concepts;
    Rules m_rules;

    Policy(int identifier, const Rules& rules);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    Policy(const Policy& other);
    Policy& operator=(const Policy& other);
    Policy(Policy&& other);
    Policy& operator=(Policy&& other);
    ~Policy();

    bool are_equal_impl(const Policy& other) const;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out) const;
    int compute_evaluate_time_score() const;


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

    const Booleans& get_booleans() const;
    const Numericals& get_numericals() const;
    const Concepts& get_concepts() const;
    const Rules& get_rules() const;
};


/// @brief Provides functionality for the syntactically unique creation of
///        conditions, effects, rules, and policies.
class PolicyFactory {
private:
    pimpl<PolicyFactoryImpl> m_pImpl;

public:
    PolicyFactory(std::shared_ptr<core::SyntacticElementFactory> element_factory);
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
        iterator_type& iter, iterator_type end,
        const std::string& filename="");

    /**
     *  Uniquely adds a boolean (resp. numerical) and returns it.
     */
    std::shared_ptr<const NamedBoolean> make_boolean(const std::string& key, const std::shared_ptr<const core::Boolean>& boolean);
    std::shared_ptr<const NamedNumerical> make_numerical(const std::string& key, const std::shared_ptr<const core::Numerical>& numerical);
    std::shared_ptr<const NamedConcept> make_concept(const std::string& key, const std::shared_ptr<const core::Concept>& concept_);
    std::shared_ptr<const NamedRole> make_role(const std::string& key, const std::shared_ptr<const core::Role>& role);

    /**
     * Uniquely adds a condition (resp. effect) and returns it.
     */
    std::shared_ptr<const BaseCondition> make_pos_condition(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseCondition> make_neg_condition(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseCondition> make_gt_condition(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseCondition> make_eq_condition(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_inc_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_gt_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_eq_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_gt_effect(const std::shared_ptr<const NamedConcept>& concept_);
    std::shared_ptr<const BaseEffect> make_eq_effect(const std::shared_ptr<const NamedConcept>& concept_);

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


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::policy::NamedBoolean>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NamedBoolean>& left_boolean,
            const std::shared_ptr<const dlplan::policy::NamedBoolean>& right_boolean) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NamedNumerical>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NamedNumerical>& left_numerical,
            const std::shared_ptr<const dlplan::policy::NamedNumerical>& right_numerical) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NamedConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NamedConcept>& left_concept,
            const std::shared_ptr<const dlplan::policy::NamedConcept>& right_concept) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NamedRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NamedRole>& left_role,
            const std::shared_ptr<const dlplan::policy::NamedRole>& right_role) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::Rule>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::Rule>& left_rule,
            const std::shared_ptr<const dlplan::policy::Rule>& right_rule) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::Policy>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::Policy>& left_policy,
            const std::shared_ptr<const dlplan::policy::Policy>& right_policy) const;
    };

    template<>
    struct hash<dlplan::policy::NamedBoolean>
    {
        std::size_t operator()(const dlplan::policy::NamedBoolean& boolean) const;
    };

    template<>
    struct hash<dlplan::policy::NamedNumerical>
    {
        std::size_t operator()(const dlplan::policy::NamedNumerical& numerical) const;
    };

    template<>
    struct hash<dlplan::policy::NamedConcept>
    {
        std::size_t operator()(const dlplan::policy::NamedConcept& concept_) const;
    };

    template<>
    struct hash<dlplan::policy::NamedRole>
    {
        std::size_t operator()(const dlplan::policy::NamedRole& role) const;
    };

    template<>
    struct hash<dlplan::policy::Rule>
    {
        std::size_t operator()(const dlplan::policy::Rule& rule) const;
    };

    template<>
    struct hash<dlplan::policy::Policy>
    {
        std::size_t operator()(const dlplan::policy::Policy& policy) const;
    };


    template<>
    struct hash<dlplan::policy::Booleans>
    {
        std::size_t operator()(const dlplan::policy::Booleans& booleans) const;
    };

    template<>
    struct hash<dlplan::policy::Numericals>
    {
        std::size_t operator()(const dlplan::policy::Numericals& numericals) const;
    };

    template<>
    struct hash<dlplan::policy::Concepts>
    {
        std::size_t operator()(const dlplan::policy::Concepts& concepts) const;
    };

    template<>
    struct hash<dlplan::policy::Roles>
    {
        std::size_t operator()(const dlplan::policy::Roles& roles) const;
    };

    template<>
    struct hash<dlplan::policy::Conditions>
    {
        std::size_t operator()(const dlplan::policy::Conditions& conditions) const;
    };

    template<>
    struct hash<dlplan::policy::Effects>
    {
        std::size_t operator()(const dlplan::policy::Effects& effects) const;
    };


}

#endif