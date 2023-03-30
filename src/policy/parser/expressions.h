#ifndef DLPLAN_SRC_POLICY_PARSER_EXPRESSIONS_H_
#define DLPLAN_SRC_POLICY_PARSER_EXPRESSIONS_H_

#include <string>
#include <memory>
#include <vector>

#include "types.h"
#include "utils.h"

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/policy.h"


namespace dlplan::policy::parser {
using Booleans = std::vector<std::shared_ptr<const dlplan::core::Boolean>>;
using Numericals = std::vector<std::shared_ptr<const dlplan::core::Numerical>>;

class Expression {
protected:
    std::string m_name;
    std::vector<Expression_Ptr> m_children;

public:
    Expression(const std::string &name,
        std::vector<Expression_Ptr> &&children)
        : m_name(name), m_children(std::move(children)) {
    }
    Expression(const Expression& other) = delete;
    Expression& operator=(const Expression& other) = delete;
    Expression(Expression&& other) = default;
    Expression& operator=(Expression&& other) = default;
    virtual ~Expression() = default;

    virtual Policy parse_policy(core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_policy - cannot parse expression into policy.");
    }

    virtual Booleans parse_booleans(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_booleans - cannot parse expression into Boolean features.");
    }

    virtual Numericals parse_numericals(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_numericals - cannot parse expression into numerical features.");
    }

    virtual std::shared_ptr<const Rule> parse_rule(PolicyBuilder&, const Booleans&, const Numericals&) const {
        throw std::runtime_error("Expression::parse_rule - cannot parse expression into rule.");
    }

    virtual std::set<std::shared_ptr<const BaseCondition>> parse_conditions(PolicyBuilder&, const Booleans&, const Numericals&) const {
        throw std::runtime_error("Expression::parse_conditions - cannot parse expression into conditions.");
    }

    virtual std::set<std::shared_ptr<const BaseEffect>> parse_effects(PolicyBuilder&, const Booleans&, const Numericals&) const {
        throw std::runtime_error("Expression::parse_effects - cannot parse expression into effects.");
    }

    virtual std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder&, const Booleans&, const Numericals&) const {
        throw std::runtime_error("Expression::parse_condition - cannot parse expression into condition.");
    }

    virtual std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder&, const Booleans&, const Numericals&) const {
        throw std::runtime_error("Expression::parse_effect - cannot parse expression into effect.");
    }

    const std::string& get_name() const { return m_name; }
    const std::vector<Expression_Ptr>& get_children() const { return m_children; }
};

class PolicyExpression : public Expression {
public:
    PolicyExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    Policy parse_policy(core::SyntacticElementFactory& factory) const override {
        PolicyBuilder builder;
        // Basic error checking.
        if (m_children.size() < 3) {
            throw std::runtime_error("PolicyExpression::parse_policy - insufficient number of children.");
        }
        // Parse Boolean features.
        const auto booleans = m_children.at(1)->parse_booleans(builder, factory);
        // Parse numerical features.
        const auto numericals = m_children.at(2)->parse_numericals(builder, factory);
        // Parse rules.
        for (size_t i = 3; i < m_children.size(); ++i) {
            m_children.at(i)->parse_rule(builder, booleans, numericals);
        }
        return builder.get_result();
    }
};


class BooleansExpression : public Expression {
public:
    BooleansExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::vector<std::shared_ptr<const core::Boolean>> parse_booleans(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        std::vector<std::shared_ptr<const core::Boolean>> boolean_features;
        for (size_t i = 1; i < m_children.size(); ++i) {
            auto boolean = factory.parse_boolean(m_children.at(i)->get_name());
            boolean.set_index(boolean_features.size());
            boolean_features.push_back(builder.add_boolean_feature(boolean));
        }
        return boolean_features;
    }
};

class NumericalsExpression : public Expression {
public:
    NumericalsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::vector<std::shared_ptr<const core::Numerical>> parse_numericals(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        std::vector<std::shared_ptr<const core::Numerical>> numerical_features;
        for (size_t i = 1; i < m_children.size(); ++i) {
            auto numerical = factory.parse_numerical(m_children.at(i)->get_name());
            numerical.set_index(numerical_features.size());
            numerical_features.push_back(builder.add_numerical_feature(numerical));
        }
        return numerical_features;
    }
};

class RuleExpression : public Expression {
public:
    RuleExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const Rule> parse_rule(policy::PolicyBuilder& builder, const Booleans& booleans, const Numericals& numericals) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("RuleExpression::parse_rule - incorrect number of children. Should be 3.");
        }
        std::set<std::shared_ptr<const BaseCondition>> conditions = m_children.at(1)->parse_conditions(builder, booleans, numericals);
        std::set<std::shared_ptr<const BaseEffect>> effects = m_children.at(2)->parse_effects(builder, booleans, numericals);
        return builder.add_rule(std::move(conditions), std::move(effects));
    }
};

class ConditionsExpression : public Expression {
public:
    ConditionsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::set<std::shared_ptr<const BaseCondition>> parse_conditions(PolicyBuilder& builder, const Booleans& booleans, const Numericals& numericals) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_conditions - incorrect number of children. Should be greater than 0.");
        }
        std::set<std::shared_ptr<const BaseCondition>> conditions;
        for (size_t i = 1; i < m_children.size(); ++i) {
            conditions.insert(m_children.at(i)->parse_condition(builder, booleans, numericals));
        }
        return conditions;
    }
};

class EffectsExpression : public Expression {
public:
    EffectsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::set<std::shared_ptr<const BaseEffect>> parse_effects(PolicyBuilder& builder, const Booleans& booleans, const Numericals& numericals) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_effects - incorrect number of children. Should be greater than 0.");
        }
        std::set<std::shared_ptr<const BaseEffect>> effects;
        for (size_t i = 1; i < m_children.size(); ++i) {
            effects.insert(m_children.at(i)->parse_effect(builder, booleans, numericals));
        }
        return effects;
    }
};

class BooleanConditionExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const = 0;

public:
    BooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder& builder, const Booleans& booleans, const Numericals&) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        int b_idx = try_parse_number(m_children.at(1)->get_name());
        if (b_idx < 0 && b_idx >= static_cast<int>(booleans.size())) {
            throw std::runtime_error("BooleanConditionExpression::parse_effect - Boolean feature index out of range.");
        }
        return parse_condition_impl(builder, booleans[b_idx]);
    }
};

class BooleanEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const = 0;

public:
    BooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder& builder, const Booleans& booleans, const Numericals&) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        int b_idx = try_parse_number(m_children.at(1)->get_name());
        if (b_idx < 0 && b_idx >= static_cast<int>(booleans.size())) {
            throw std::runtime_error("BooleanEffectExpression::parse_effect - Boolean feature index out of range.");
        }
        return parse_effect_impl(builder, booleans[b_idx]);
    }
};

class NumericalConditionExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const = 0;

public:
    NumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder& builder, const Booleans&, const Numericals& numericals) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        int n_idx = try_parse_number(m_children.at(1)->get_name());
        if (n_idx < 0 && n_idx >= static_cast<int>(numericals.size())) {
            throw std::runtime_error("NumericalConditionExpression::parse_condition - Boolean feature index out of range.");
        }
        return parse_condition_impl(builder, numericals[n_idx]);
    }
};

class NumericalEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const = 0;

public:
    NumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder& builder, const Booleans&, const Numericals& numericals) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        int n_idx = try_parse_number(m_children.at(1)->get_name());
        if (n_idx < 0 && n_idx >= static_cast<int>(numericals.size())) {
            throw std::runtime_error("NumericalEffectExpression::parse_effect - Boolean feature index out of range.");
        }
        return parse_effect_impl(builder, numericals[n_idx]);
    }
};

class PositiveBooleanConditionExpression : public BooleanConditionExpression {
public:
    PositiveBooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const override {
        return builder.add_pos_condition(b);
    }
};

class NegativeBooleanConditionExpression : public BooleanConditionExpression {
public:
    NegativeBooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const override {
        return builder.add_neg_condition(b);
    }
};

class EqualNumericalConditionExpression : public NumericalConditionExpression {
public:
    EqualNumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const override {
        return builder.add_eq_condition(n);
    }
};

class GreaterNumericalConditionExpression : public NumericalConditionExpression {
public:
    GreaterNumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const override {
        return builder.add_gt_condition(n);
    }
};

class PositiveBooleanEffectExpression : public BooleanEffectExpression {
public:
    PositiveBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const override {
        return builder.add_pos_effect(b);
    }
};

class NegativeBooleanEffectExpression : public BooleanEffectExpression {
public:
    NegativeBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const override {
        return builder.add_neg_effect(b);
    }
};

class UnchangedBooleanEffectExpression : public BooleanEffectExpression {
public:
    UnchangedBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const override {
        return builder.add_bot_effect(b);
    }
};

class IncrementNumericalEffectExpression : public NumericalEffectExpression {
public:
    IncrementNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const override {
        return builder.add_inc_effect(n);
    }
};

class DecrementNumericalEffectExpression : public NumericalEffectExpression {
public:
    DecrementNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const override {
        return builder.add_dec_effect(n);
    }
};

class UnchangedNumericalEffectExpression : public NumericalEffectExpression {
public:
    UnchangedNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const override {
        return builder.add_bot_effect(n);
    }
};

}

#endif
