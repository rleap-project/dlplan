#include "include/dlplan/policy/parsers/policy/stage_2/parser.hpp"

#include <sstream>

#include "include/dlplan/core/parsers/elements/stage_2/parser.hpp"

using namespace dlplan::common::parsers;


namespace dlplan::policy::parsers::policy::stage_2::parser {

std::string parse(
    const stage_1::ast::Name& node, const error_handler_type&, Context&) {
    std::stringstream ss;
    ss << node.alphabetical << node.suffix;
    return ss.str();
}

std::pair<std::string, std::shared_ptr<const core::Boolean>> parse(
    const stage_1::ast::BooleanDefinition& node, const error_handler_type& error_handler, Context& context) {
    return *context.booleans.emplace(
            parse(node.key, error_handler, context),
            dlplan::core::parsers::elements::stage_2::parser::parse(
                node.boolean, error_handler, *context.policy_factory.get_element_factory())).first;
}

std::shared_ptr<const core::Boolean> parse(
    const stage_1::ast::BooleanReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.booleans.find(key);
    if (it == context.booleans.end()) {
        error_handler(node, "Undefined boolean " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const core::Boolean>> parse(
    const stage_1::ast::BooleansEntry& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const core::Boolean>> booleans;
    for (const auto& child : node.definitions) {
        booleans.insert(parse(child, error_handler, context));
    }
    return booleans;
}

std::pair<std::string, std::shared_ptr<const core::Numerical>> parse(
    const stage_1::ast::NumericalDefinition& node, const error_handler_type& error_handler, Context& context) {
    return *context.numericals.emplace(
            parse(node.key, error_handler, context),
            dlplan::core::parsers::elements::stage_2::parser::parse(
                node.numerical, error_handler, *context.policy_factory.get_element_factory())).first;
}

std::shared_ptr<const core::Numerical> parse(
    const stage_1::ast::NumericalReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.numericals.find(key);
    if (it == context.numericals.end()) {
        error_handler(node, "Undefined numerical " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const core::Numerical>> parse(
    const stage_1::ast::NumericalsEntry& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const core::Numerical>> numericals;
    for (const auto& child : node.definitions) {
        numericals.insert(parse(child, error_handler, context));
    }
    return numericals;
}

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::PositiveBooleanConditionEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_pos_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::NegativeBooleanConditionEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_neg_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::GreaterNumericalConditionEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_gt_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::EqualNumericalConditionEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_eq_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::PositiveBooleanEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_pos_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::NegativeBooleanEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_neg_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::UnchangedBooleanEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::IncrementNumericalEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_inc_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::DecrementNumericalEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_dec_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::UnchangedNumericalEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

class FeatureConditionEntryInnerVisitor : public boost::static_visitor<> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    std::shared_ptr<const BaseCondition> result;

    FeatureConditionEntryInnerVisitor(const error_handler_type& error_handler, Context& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::FeatureConditionEntryInner& node, const error_handler_type& error_handler, Context& context) {
    FeatureConditionEntryInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::FeatureConditionEntry& node, const error_handler_type& error_handler, Context& context) {
    return parse(node.condition, error_handler, context);
}


class FeatureEffectEntryInnerVisitor : public boost::static_visitor<> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    std::shared_ptr<const BaseEffect> result;

    FeatureEffectEntryInnerVisitor(const error_handler_type& error_handler, Context& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::FeatureEffectEntryInner& node, const error_handler_type& error_handler, Context& context) {
    FeatureEffectEntryInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::FeatureEffectEntry& node, const error_handler_type& error_handler, Context& context) {
    return parse(node.effect, error_handler, context);
}


std::shared_ptr<const Rule> parse(
    const stage_1::ast::RuleEntry& node, const error_handler_type& error_handler, Context& context) {
    Conditions conditions;
    for (const auto& child : node.feature_conditions) {
        conditions.insert(parse(child, error_handler, context));
    }
    Effects effects;
    for (const auto& child : node.feature_effects) {
        effects.insert(parse(child, error_handler, context));
    }
    return context.policy_factory.make_rule(conditions, effects);
}

Rules parse(
    const stage_1::ast::Rules& node, const error_handler_type& error_handler, Context& context) {
    Rules rules;
    for (const auto& child : node.rules) {
        rules.insert(parse(child, error_handler, context));
    }
    return rules;
}

std::shared_ptr<const Policy> parse(
    const stage_1::ast::Policy& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context) {
    parse(node.booleans, error_handler, context);
    parse(node.numericals, error_handler, context);
    return context.policy_factory.make_policy(parse(node.rules, error_handler, context));
}

}