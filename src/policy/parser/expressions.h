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

    virtual std::shared_ptr<const Policy> parse_policy(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_policy - cannot parse expression into policy.");
    }

    virtual std::shared_ptr<const Rule> parse_rule(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_rule - cannot parse expression into rule.");
    }

    virtual std::set<std::shared_ptr<const BaseCondition>> parse_conditions(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_conditions - cannot parse expression into conditions.");
    }

    virtual std::set<std::shared_ptr<const BaseEffect>> parse_effects(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_effects - cannot parse expression into effects.");
    }

    virtual std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_condition - cannot parse expression into condition.");
    }

    virtual std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder&, core::SyntacticElementFactory&) const {
        throw std::runtime_error("Expression::parse_effect - cannot parse expression into effect.");
    }

    const std::string& get_name() const { return m_name; }
    const std::vector<Expression_Ptr>& get_children() const { return m_children; }
};

class PolicyExpression : public Expression {
public:
    PolicyExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const Policy> parse_policy(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        // Basic error checking.
        if (m_children.size() < 1) {
            throw std::runtime_error("PolicyExpression::parse_policy - insufficient number of children.");
        }
        // Parse rules.
        Rules rules;
        for (size_t i = 1; i < m_children.size(); ++i) {
            rules.insert(m_children.at(i)->parse_rule(builder, factory));
        }
        return builder.add_policy(std::move(rules));
    }
};

class RuleExpression : public Expression {
public:
    RuleExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const Rule> parse_rule(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("RuleExpression::parse_rule - incorrect number of children. Should be 3.");
        }
        std::set<std::shared_ptr<const BaseCondition>> conditions = m_children.at(1)->parse_conditions(builder, factory);
        std::set<std::shared_ptr<const BaseEffect>> effects = m_children.at(2)->parse_effects(builder, factory);
        return builder.add_rule(std::move(conditions), std::move(effects));
    }
};

class ConditionsExpression : public Expression {
public:
    ConditionsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::set<std::shared_ptr<const BaseCondition>> parse_conditions(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_conditions - incorrect number of children. Should be greater than 0.");
        }
        std::set<std::shared_ptr<const BaseCondition>> conditions;
        for (size_t i = 1; i < m_children.size(); ++i) {
            conditions.insert(m_children.at(i)->parse_condition(builder, factory));
        }
        return conditions;
    }
};

class EffectsExpression : public Expression {
public:
    EffectsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::set<std::shared_ptr<const BaseEffect>> parse_effects(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_effects - incorrect number of children. Should be greater than 0.");
        }
        std::set<std::shared_ptr<const BaseEffect>> effects;
        for (size_t i = 1; i < m_children.size(); ++i) {
            effects.insert(m_children.at(i)->parse_effect(builder, factory));
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

    std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        return parse_condition_impl(builder, factory.parse_boolean(m_children.at(1)->get_name()));
    }
};

class BooleanEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Boolean> b) const = 0;

public:
    BooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        return parse_effect_impl(builder, factory.parse_boolean(m_children.at(1)->get_name()));
    }
};

class NumericalConditionExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseCondition> parse_condition_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const = 0;

public:
    NumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        return parse_condition_impl(builder, factory.parse_numerical(m_children.at(1)->get_name()));
    }
};

class NumericalEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(PolicyBuilder& builder, std::shared_ptr<const core::Numerical> n) const = 0;

public:
    NumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(PolicyBuilder& builder, core::SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        return parse_effect_impl(builder, factory.parse_numerical(m_children.at(1)->get_name()));
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
