#ifndef DLPLAN_SRC_POLICY_PARSER_EXPRESSIONS_H_
#define DLPLAN_SRC_POLICY_PARSER_EXPRESSIONS_H_

#include <string>
#include <memory>
#include <vector>

#include "types.h"
#include "utils.h"

#include "../../../include/dlplan/policy.h"


namespace dlplan::policy::parser {

class Expression {
protected:
    std::string m_name;
    std::vector<Expression_Ptr> m_children;

protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<Expression>(*this);
    }

public:
    Expression(const std::string &name,
        std::vector<Expression_Ptr> &&children)
        : m_name(name), m_children(std::move(children)) {
    }
    Expression(const Expression& other) {
        m_name = other.m_name;
        for (const auto& child : other.m_children) {
            m_children.push_back(child->clone());
        }
    }
    Expression& operator=(const Expression& other) {
        if (this != &other) {
            m_name = other.m_name;
            m_children.clear();
            for (const auto& child : other.m_children) {
                m_children.push_back(child->clone());
            }
            m_children.shrink_to_fit();
        }
        return *this;
    }
    virtual ~Expression() { }

    virtual Policy parse_general_policy() const {
        throw std::runtime_error("Expression::parse_general_policy - cannot parse expression into general policy.");
    }

    virtual std::vector<std::shared_ptr<Feature<bool>>> parse_boolean_features(Policy&) const {
        throw std::runtime_error("Expression::parse_boolean_features - cannot parse expression into Boolean features.");
    }

    virtual std::vector<std::shared_ptr<Feature<int>>> parse_numerical_features(Policy&) const {
        throw std::runtime_error("Expression::parse_numerical_features - cannot parse expression into numerical features.");
    }

    virtual std::shared_ptr<const Rule> parse_rule(Policy&, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>&) const {
        throw std::runtime_error("Expression::parse_rule - cannot parse expression into rule.");
    }

    virtual std::unordered_set<std::shared_ptr<const BaseCondition>> parse_conditions(Policy&, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>&) const {
        throw std::runtime_error("Expression::parse_conditions - cannot parse expression into conditions.");
    }

    virtual std::unordered_set<std::shared_ptr<const BaseEffect>> parse_effects(Policy&, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>&) const {
        throw std::runtime_error("Expression::parse_effects - cannot parse expression into effects.");
    }

    virtual std::shared_ptr<const BaseCondition> parse_condition(Policy&, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>&) const {
        throw std::runtime_error("Expression::parse_condition - cannot parse expression into condition.");
    }

    virtual std::shared_ptr<const BaseEffect> parse_effect(Policy&, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>&) const {
        throw std::runtime_error("Expression::parse_effect - cannot parse expression into effect.");
    }

    const std::string& get_name() const { return m_name; }
    const std::vector<Expression_Ptr>& get_children() const { return m_children; }

    Expression_Ptr clone() const {
        return clone_impl();
    }
};

class PolicyExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<PolicyExpression>(*this);
    }

public:
    PolicyExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    Policy parse_general_policy() const override {
        Policy gp;
        // Basic error checking.
        if (m_children.size() < 3) {
            throw std::runtime_error("PolicyExpression::parse_general_policy - insufficient number of children.");
        }
        // Parse Boolean features.
        std::vector<std::shared_ptr<Feature<bool>>> boolean_features = m_children.at(1)->parse_boolean_features(gp);
        // Parse numerical features.
        std::vector<std::shared_ptr<Feature<int>>> numerical_features = m_children.at(2)->parse_numerical_features(gp);
        // Parse rules.
        for (size_t i = 3; i < m_children.size(); ++i) {
            m_children.at(i)->parse_rule(gp, boolean_features, numerical_features);
        }
        return gp;
    }
};

class BooleanFeaturesExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<BooleanFeaturesExpression>(*this);
    }

public:
    BooleanFeaturesExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::vector<std::shared_ptr<Feature<bool>>> parse_boolean_features(Policy& gp) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanFeaturesExpression::parse_boolean_features - incorrect number of children. Should be 2.");
        }
        int num_boolean_features = try_parse_number(m_children.at(1)->get_name());
        std::vector<std::shared_ptr<Feature<bool>>> boolean_features;
        for (int i = 0; i < num_boolean_features; ++i) {
            boolean_features.push_back(gp.add_boolean_feature());
        }
        return boolean_features;
    }
};

class NumericalFeaturesExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<NumericalFeaturesExpression>(*this);
    }

public:
    NumericalFeaturesExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::vector<std::shared_ptr<Feature<int>>> parse_numerical_features(Policy& gp) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalFeaturesExpression::parse_numerical_features - incorrect number of children. Should be 2.");
        }
        int num_numerical_features = try_parse_number(m_children.at(1)->get_name());
        std::vector<std::shared_ptr<Feature<int>>> numerical_features;
        for (int i = 0; i < num_numerical_features; ++i) {
            numerical_features.push_back(gp.add_numerical_feature());
        }
        return numerical_features;
    }
};

class RuleExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<RuleExpression>(*this);
    }

public:
    RuleExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const Rule> parse_rule(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>& boolean_features, const std::vector<std::shared_ptr<Feature<int>>>& numerical_features) const override {
        if (m_children.size() != 3) {
            throw std::runtime_error("RuleExpression::parse_rule - incorrect number of children. Should be 3.");
        }
        std::unordered_set<std::shared_ptr<const BaseCondition>> conditions = m_children.at(1)->parse_conditions(gp, boolean_features, numerical_features);
        std::unordered_set<std::shared_ptr<const BaseEffect>> effects = m_children.at(2)->parse_effects(gp, boolean_features, numerical_features);
        return gp.add_rule(std::move(conditions), std::move(effects));
    }
};

class ConditionsExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<ConditionsExpression>(*this);
    }

public:
    ConditionsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::unordered_set<std::shared_ptr<const BaseCondition>> parse_conditions(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>& boolean_features, const std::vector<std::shared_ptr<Feature<int>>>& numerical_features) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_conditions - incorrect number of children. Should be greater than 0.");
        }
        std::unordered_set<std::shared_ptr<const BaseCondition>> conditions;
        for (size_t i = 1; i < m_children.size(); ++i) {
            conditions.insert(m_children.at(i)->parse_condition(gp, boolean_features, numerical_features));
        }
        return conditions;
    }
};

class EffectsExpression : public Expression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<EffectsExpression>(*this);
    }

public:
    EffectsExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::unordered_set<std::shared_ptr<const BaseEffect>> parse_effects(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>& boolean_features, const std::vector<std::shared_ptr<Feature<int>>>& numerical_features) const override {
        if (m_children.size() < 1) {
            throw std::runtime_error("RuleExpression::parse_effects - incorrect number of children. Should be greater than 0.");
        }
        std::unordered_set<std::shared_ptr<const BaseEffect>> effects;
        for (size_t i = 1; i < m_children.size(); ++i) {
            effects.insert(m_children.at(i)->parse_effect(gp, boolean_features, numerical_features));
        }
        return effects;
    }
};

class BooleanConditionExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const = 0;

public:
    BooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>& boolean_features, const std::vector<std::shared_ptr<Feature<int>>>&) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        int b_idx = try_parse_number(m_children.at(1)->get_name());
        if (b_idx < 0 && b_idx >= static_cast<int>(boolean_features.size())) {
            throw std::runtime_error("BooleanConditionExpression::parse_condition - Boolean feature index out of range.");
        }
        return parse_condition_impl(gp, boolean_features.at(b_idx));
    }
};

class BooleanEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const = 0;

public:
    BooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>& boolean_features, const std::vector<std::shared_ptr<Feature<int>>>&) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("BooleanEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        int b_idx = try_parse_number(m_children.at(1)->get_name());
        if (b_idx < 0 && b_idx >= static_cast<int>(boolean_features.size())) {
            throw std::runtime_error("BooleanEffectExpression::parse_effect - Boolean feature index out of range.");
        }
        return parse_effect_impl(gp, boolean_features.at(b_idx));
    }
};

class NumericalConditionExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const = 0;

public:
    NumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>& numerical_features) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalConditionExpression::parse_condition - incorrect number of children. Should be 2.");
        }
        int n_idx = try_parse_number(m_children.at(1)->get_name());
        if (n_idx < 0 && n_idx >= static_cast<int>(numerical_features.size())) {
            throw std::runtime_error("NumericalConditionExpression::parse_condition - Boolean feature index out of range.");
        }
        return parse_condition_impl(gp, numerical_features.at(n_idx));
    }
};

class NumericalEffectExpression : public Expression {
protected:
    virtual std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const = 0;

public:
    NumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : Expression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect(Policy& gp, const std::vector<std::shared_ptr<Feature<bool>>>&, const std::vector<std::shared_ptr<Feature<int>>>& numerical_features) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("NumericalEffectExpression::parse_effect - incorrect number of children. Should be 2.");
        }
        int n_idx = try_parse_number(m_children.at(1)->get_name());
        if (n_idx < 0 && n_idx >= static_cast<int>(numerical_features.size())) {
            throw std::runtime_error("NumericalEffectExpression::parse_effect - Boolean feature index out of range.");
        }
        return parse_effect_impl(gp, numerical_features.at(n_idx));
    }
};

class PositiveBooleanConditionExpression : public BooleanConditionExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<PositiveBooleanConditionExpression>(*this);
    }

public:
    PositiveBooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const override {
        return gp.add_b_pos_condition(b);
    }
};

class NegativeBooleanConditionExpression : public BooleanConditionExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<NegativeBooleanConditionExpression>(*this);
    }

public:
    NegativeBooleanConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const override {
        return gp.add_b_neg_condition(b);
    }
};

class EqualNumericalConditionExpression : public NumericalConditionExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<EqualNumericalConditionExpression>(*this);
    }

public:
    EqualNumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const override {
        return gp.add_n_eq_condition(n);
    }
};

class GreaterNumericalConditionExpression : public NumericalConditionExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<GreaterNumericalConditionExpression>(*this);
    }

public:
    GreaterNumericalConditionExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalConditionExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseCondition> parse_condition_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const override {
        return gp.add_n_gt_condition(n);
    }
};

class PositiveBooleanEffectExpression : public BooleanEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<PositiveBooleanEffectExpression>(*this);
    }

public:
    PositiveBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const override {
        return gp.add_b_pos_effect(b);
    }
};

class NegativeBooleanEffectExpression : public BooleanEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<NegativeBooleanEffectExpression>(*this);
    }

public:
    NegativeBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const override {
        return gp.add_b_neg_effect(b);
    }
};

class UnchangedBooleanEffectExpression : public BooleanEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<UnchangedBooleanEffectExpression>(*this);
    }

public:
    UnchangedBooleanEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : BooleanEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<bool>> b) const override {
        return gp.add_b_bot_effect(b);
    }
};

class IncrementNumericalEffectExpression : public NumericalEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<IncrementNumericalEffectExpression>(*this);
    }

public:
    IncrementNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const override {
        return gp.add_n_inc_effect(n);
    }
};

class DecrementNumericalEffectExpression : public NumericalEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<DecrementNumericalEffectExpression>(*this);
    }

public:
    DecrementNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const override {
        return gp.add_n_dec_effect(n);
    }
};

class UnchangedNumericalEffectExpression : public NumericalEffectExpression {
protected:
    virtual Expression_Ptr clone_impl() const {
        return std::make_unique<UnchangedNumericalEffectExpression>(*this);
    }

public:
    UnchangedNumericalEffectExpression(const std::string &name, std::vector<Expression_Ptr> &&children)
    : NumericalEffectExpression(name, std::move(children)) { }

    std::shared_ptr<const BaseEffect> parse_effect_impl(Policy& gp, std::shared_ptr<Feature<int>> n) const override {
        return gp.add_n_bot_effect(n);
    }
};

}

#endif
