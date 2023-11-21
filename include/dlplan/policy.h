/// @brief Implements the general policy language.

#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_H_

#include <unordered_set>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "common/parsers/config.hpp"
#include "core.h"
#include "utils/pimpl.h"


// Forward declarations of this header
namespace dlplan::policy {
class NamedBoolean;
class NamedNumerical;
class NamedConcept;
class NamedRole;
class PolicyFactoryImpl;
class BaseCondition;
class BaseEffect;
class Rule;
class Policy;
class PolicyFactory;
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
            return l->compute_repr() < r->compute_repr();
        }
        return l->compute_evaluate_time_score() < r->compute_evaluate_time_score();
    }
};

using Booleans = std::set<std::shared_ptr<const NamedBoolean>, ScoreCompare<const NamedBoolean>>;
using Numericals = std::set<std::shared_ptr<const NamedNumerical>, ScoreCompare<const NamedNumerical>>;
using Concepts = std::set<std::shared_ptr<const NamedConcept>, ScoreCompare<const NamedConcept>>;
using Roles = std::set<std::shared_ptr<const NamedRole>, ScoreCompare<const NamedRole>>;
using Conditions = std::set<std::shared_ptr<const BaseCondition>, ScoreCompare<const BaseCondition>>;
using Effects = std::set<std::shared_ptr<const BaseEffect>, ScoreCompare<const BaseEffect>>;
using Rules = std::set<std::shared_ptr<const Rule>, ScoreCompare<const Rule>>;
using Policies = std::set<std::shared_ptr<const Policy>, ScoreCompare<const Policy>>;

using StatePair = std::pair<dlplan::core::State, dlplan::core::State>;
using StatePairs = std::vector<StatePair>;


/// @brief Wrappers around core elements to add an additional key
///        that can potentially add some more human readable meaning.
class NamedBoolean {
private:
    int m_identifier;
    std::string m_key;
    std::shared_ptr<const core::Boolean> m_boolean;

    NamedBoolean(int identifier, const std::string& key, std::shared_ptr<const core::Boolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    NamedBoolean(const NamedBoolean& other);
    NamedBoolean& operator=(const NamedBoolean& other);
    NamedBoolean(NamedBoolean&& other);
    NamedBoolean& operator=(NamedBoolean&& other);
    ~NamedBoolean();

    bool operator==(const NamedBoolean& other) const;
    bool operator<(const NamedBoolean& other) const;

    size_t hash() const;

    int compute_evaluate_time_score() const;
    std::string compute_repr() const;
    std::string str() const;

    const std::string& get_key() const;
    std::shared_ptr<const core::Boolean> get_boolean() const;
};


class NamedNumerical {
private:
    int m_identifier;
    std::string m_key;
    std::shared_ptr<const core::Numerical> m_numerical;

    NamedNumerical(int identifier, const std::string& key, std::shared_ptr<const core::Numerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    NamedNumerical(const NamedNumerical& other);
    NamedNumerical& operator=(const NamedNumerical& other);
    NamedNumerical(NamedNumerical&& other);
    NamedNumerical& operator=(NamedNumerical&& other);
    ~NamedNumerical();

    bool operator==(const NamedNumerical& other) const;
    bool operator<(const NamedNumerical& other) const;

    size_t hash() const;

    int compute_evaluate_time_score() const;
    std::string compute_repr() const;
    std::string str() const;

    const std::string& get_key() const;
    std::shared_ptr<const core::Numerical> get_numerical() const;
};


class NamedConcept {
private:
    int m_identifier;
    std::string m_key;
    std::shared_ptr<const core::Concept> m_concept;

    NamedConcept(int identifier, const std::string& key, std::shared_ptr<const core::Concept> concept);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    NamedConcept(const NamedConcept& other);
    NamedConcept& operator=(const NamedConcept& other);
    NamedConcept(NamedConcept&& other);
    NamedConcept& operator=(NamedConcept&& other);
    ~NamedConcept();

    bool operator==(const NamedConcept& other) const;
    bool operator<(const NamedConcept& other) const;

    size_t hash() const;

    int compute_evaluate_time_score() const;
    std::string compute_repr() const;
    std::string str() const;

    const std::string& get_key() const;
    std::shared_ptr<const core::Concept> get_concept() const;
};


class NamedRole {
private:
    int m_identifier;
    std::string m_key;
    std::shared_ptr<const core::Role> m_role;

    NamedRole(int identifier, const std::string& key, std::shared_ptr<const core::Role> role);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    NamedRole(const NamedRole& other);
    NamedRole& operator=(const NamedRole& other);
    NamedRole(NamedRole&& other);
    NamedRole& operator=(NamedRole&& other);
    ~NamedRole();

    bool operator==(const NamedRole& other) const;
    bool operator<(const NamedRole& other) const;

    size_t hash() const;

    int compute_evaluate_time_score() const;
    std::string compute_repr() const;
    std::string str() const;

    const std::string& get_key() const;
    std::shared_ptr<const core::Role> get_role() const;
};


/// @brief Represents the abstract base class of a feature condition and
///        provides functionality to access its underlying data and for
///        the evaluation on a state.
class BaseCondition {
protected:
    int m_identifier;

    explicit BaseCondition(int identifier);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    virtual ~BaseCondition();

    virtual bool operator==(const BaseCondition& other) const = 0;
    bool operator<(const BaseCondition& other) const;

    virtual size_t hash() const = 0;

    virtual bool evaluate(const core::State& source_state) const = 0;
    virtual bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const = 0;

    virtual std::string compute_repr() const = 0;
    virtual std::string str() const = 0;

    /// @brief Computes a time score for evaluating this condition relative to other conditions.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    virtual int compute_evaluate_time_score() const = 0;

    int get_index() const;
    virtual std::shared_ptr<const NamedBoolean> get_boolean() const = 0;
    virtual std::shared_ptr<const NamedNumerical> get_numerical() const = 0;
};


/// @brief Represents the abstract base class of a feature effect and
///        provides functionality to access its underlying data and for
///        the evaluation on a pair of states.
class BaseEffect {
protected:
    int m_identifier;

    explicit BaseEffect(int identifier);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    virtual ~BaseEffect();

    virtual bool operator==(const BaseEffect& other) const = 0;
    bool operator<(const BaseEffect& other) const;

    virtual size_t hash() const = 0;

    virtual bool evaluate(const core::State& source_state, const core::State& target_state) const = 0;
    virtual bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const = 0;

    virtual std::string compute_repr() const = 0;
    virtual std::string str() const = 0;

    /// @brief Computes a time score for evaluating this effect relative to other effects.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    virtual int compute_evaluate_time_score() const = 0;

    int get_index() const;
    virtual std::shared_ptr<const NamedBoolean> get_boolean() const = 0;
    virtual std::shared_ptr<const NamedNumerical> get_numerical() const = 0;
};


/// @brief Implements a policy rule of the form C->E with where C is a set of
///        feature conditions and E is a set of feature effects. Provides
///        functionality to access its underlying data and for the evaluation
///        on a pair of states.
class Rule {
private:
    int m_identifier;
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

    bool operator==(const Rule& other) const;
    bool operator<(const Rule& other) const;

    bool evaluate_conditions(const core::State& source_state) const;
    bool evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state) const;
    bool evaluate_effects(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const;

    std::string compute_repr() const;
    std::string str() const;

    size_t hash() const;

    /// @brief Computes a time score for evaluating this rule relative to other rules.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    int compute_evaluate_time_score() const;

    int get_index() const;
    const Conditions& get_conditions() const;
    const Effects& get_effects() const;
};


/// @brief Implements a policy consisting of a set of rules over a set of
///        Boolean and a set of numerical features. Provides functionality
///        to access its underlying data and for the evaluation on a pair of
///        states.
class Policy {
private:
    int m_identifier;
    Booleans m_booleans;
    Numericals m_numericals;
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

    bool operator==(const Policy& other) const;
    bool operator<(const Policy& other) const;

    size_t hash() const;

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

    int get_index() const;
    const Booleans& get_booleans() const;
    const Numericals& get_numericals() const;
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
    std::shared_ptr<const BaseEffect> make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean);
    std::shared_ptr<const BaseEffect> make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical);
    std::shared_ptr<const BaseEffect> make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical);

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