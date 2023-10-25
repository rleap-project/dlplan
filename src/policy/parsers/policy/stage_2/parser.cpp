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

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(
    const stage_1::ast::Boolean& node, const error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    return *context.booleans.emplace(
            key,
            context.policy_factory.make_boolean(key, dlplan::core::parsers::elements::stage_2::parser::parse(
                node.boolean, error_handler, *context.policy_factory.get_element_factory()))).first;
}

std::shared_ptr<const dlplan::policy::NamedBoolean> parse(
    const stage_1::ast::BooleanReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.booleans.find(key);
    if (it == context.booleans.end()) {
        error_handler(node, "Undefined boolean " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(
    const stage_1::ast::Booleans& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> booleans;
    for (const auto& child : node.definitions) {
        booleans.insert(parse(child, error_handler, context));
    }
    return booleans;
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(
    const stage_1::ast::Numerical& node, const error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    return *context.numericals.emplace(
            key,
            context.policy_factory.make_numerical(key, dlplan::core::parsers::elements::stage_2::parser::parse(
                node.numerical, error_handler, *context.policy_factory.get_element_factory()))).first;
}

std::shared_ptr<const dlplan::policy::NamedNumerical> parse(
    const stage_1::ast::NumericalReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.numericals.find(key);
    if (it == context.numericals.end()) {
        error_handler(node, "Undefined numerical " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(
    const stage_1::ast::Numericals& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> numericals;
    for (const auto& child : node.definitions) {
        numericals.insert(parse(child, error_handler, context));
    }
    return numericals;
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(
    const stage_1::ast::Concept& node, const error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    return *context.concepts.emplace(
            key,
            context.policy_factory.make_concept(key, dlplan::core::parsers::elements::stage_2::parser::parse(
                node.concept, error_handler, *context.policy_factory.get_element_factory()))).first;
}

std::shared_ptr<const dlplan::policy::NamedConcept> parse(
    const stage_1::ast::ConceptReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.concepts.find(key);
    if (it == context.concepts.end()) {
        error_handler(node, "Undefined concept " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(
    const stage_1::ast::Concepts& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> concepts;
    for (const auto& child : node.definitions) {
        concepts.insert(parse(child, error_handler, context));
    }
    return concepts;
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(
    const stage_1::ast::Role& node, const error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    return *context.roles.emplace(
            key,
            context.policy_factory.make_role(key, dlplan::core::parsers::elements::stage_2::parser::parse(
                node.role, error_handler, *context.policy_factory.get_element_factory()))).first;
}

std::shared_ptr<const dlplan::policy::NamedRole> parse(
    const stage_1::ast::RoleReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.roles.find(key);
    if (it == context.roles.end()) {
        error_handler(node, "Undefined role " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(
    const stage_1::ast::Roles& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> roles;
    for (const auto& child : node.definitions) {
        roles.insert(parse(child, error_handler, context));
    }
    return roles;
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
    const stage_1::ast::DecrementNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_dec_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::UnchangedNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

class FeatureConditionEntryVisitor : public boost::static_visitor<> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    std::shared_ptr<const BaseCondition> result;

    FeatureConditionEntryVisitor(const error_handler_type& error_handler, Context& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::FeatureCondition& node, const error_handler_type& error_handler, Context& context) {
    FeatureConditionEntryVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}


class FeatureEffectEntryVisitor : public boost::static_visitor<> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    std::shared_ptr<const BaseEffect> result;

    FeatureEffectEntryVisitor(const error_handler_type& error_handler, Context& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::FeatureEffect& node, const error_handler_type& error_handler, Context& context) {
    FeatureEffectEntryVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}


std::shared_ptr<const Rule> parse(
    const stage_1::ast::Rule& node, const error_handler_type& error_handler, Context& context) {
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