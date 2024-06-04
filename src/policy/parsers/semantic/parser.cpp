#include "../../../../include/dlplan/policy/parsers/semantic/parser.hpp"

#include "../../../../include/dlplan/core/parsers/semantic/parser.hpp"
#include "../../../../include/dlplan/policy/parsers/semantic/context.hpp"

#include <sstream>

using namespace dlplan;


namespace dlplan::policy {
template<typename T>
class ParseVisitor : public boost::static_visitor<T> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    ParseVisitor(const error_handler_type& error_handler, Context& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    T operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

std::string parse(
    const ast::Name& node, const error_handler_type&, Context&) {
    std::stringstream ss;
    ss << node.alphabetical << node.suffix;
    return ss.str();
}

std::string parse(
    const ast::BooleanDefinition& node, const dlplan::error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    auto it = context.booleans.find(key);
    if (it != context.booleans.end()) {
        error_handler(node, "Multiple definitions of boolean " + key);
        error_handler(it->second.node, "Previous definition: ");
        throw std::runtime_error("Failed parse.");
    }
    return key;
}
std::shared_ptr<const dlplan::core::Boolean> parse(
    const ast::BooleanImplementation& node, const dlplan::error_handler_type& error_handler, Context& context) {
    return dlplan::core::parse(node.boolean, error_handler, *context.policy_factory.get_element_factory());
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(
    const ast::Boolean& node, const error_handler_type& error_handler, Context& context) {
    const auto definition = parse(node.definition, error_handler, context);
    const auto implementation = parse(node.implementation, error_handler, context);
    auto named_boolean = context.policy_factory.make_boolean(definition, implementation);
    context.booleans.emplace(definition, NamedBooleanData{ node.definition, named_boolean });
    return {definition, named_boolean};
}

std::shared_ptr<const dlplan::policy::NamedBoolean> parse(
    const ast::BooleanReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.booleans.find(key);
    if (it == context.booleans.end()) {
        error_handler(node, "Undefined boolean " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second.result;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(
    const ast::Booleans& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> booleans;
    for (const auto& child : node.definitions) {
        booleans.insert(parse(child, error_handler, context));
    }
    return booleans;
}

std::string parse(
    const ast::NumericalDefinition& node, const dlplan::error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    auto it = context.numericals.find(key);
    if (it != context.numericals.end()) {
        error_handler(node, "Multiple definitions of numerical " + key);
        error_handler(it->second.node, "Previous definition: ");
        throw std::runtime_error("Failed parse.");
    }
    return key;
}
std::shared_ptr<const dlplan::core::Numerical> parse(
    const ast::NumericalImplementation& node, const dlplan::error_handler_type& error_handler, Context& context) {
    return dlplan::core::parse(node.numerical, error_handler, *context.policy_factory.get_element_factory());
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(
    const ast::Numerical& node, const error_handler_type& error_handler, Context& context) {
    const auto definition = parse(node.definition, error_handler, context);
    const auto implementation = parse(node.implementation, error_handler, context);
    auto named_numerical = context.policy_factory.make_numerical(definition, implementation);
    context.numericals.emplace(definition, NamedNumericalData{ node.definition, named_numerical });
    return {definition, named_numerical};
}

std::shared_ptr<const dlplan::policy::NamedNumerical> parse(
    const ast::NumericalReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.numericals.find(key);
    if (it == context.numericals.end()) {
        error_handler(node, "Undefined numerical " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second.result;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(
    const ast::Numericals& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> numericals;
    for (const auto& child : node.definitions) {
        numericals.insert(parse(child, error_handler, context));
    }
    return numericals;
}

std::string parse(
    const ast::ConceptDefinition& node, const dlplan::error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    auto it = context.concepts.find(key);
    if (it != context.concepts.end()) {
        error_handler(node, "Multiple definitions of concept " + key);
        error_handler(it->second.node, "Previous definition: ");
        throw std::runtime_error("Failed parse.");
    }
    return key;
}
std::shared_ptr<const dlplan::core::Concept> parse(
    const ast::ConceptImplementation& node, const dlplan::error_handler_type& error_handler, Context& context) {
    return dlplan::core::parse(node.concept_, error_handler, *context.policy_factory.get_element_factory());
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(
    const ast::Concept& node, const error_handler_type& error_handler, Context& context) {
    const auto definition = parse(node.definition, error_handler, context);
    const auto implementation = parse(node.implementation, error_handler, context);
    auto named_concept = context.policy_factory.make_concept(definition, implementation);
    context.concepts.emplace(definition, NamedConceptData{ node.definition, named_concept });
    return {definition, named_concept};
}

std::shared_ptr<const dlplan::policy::NamedConcept> parse(
    const ast::ConceptReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.concepts.find(key);
    if (it == context.concepts.end()) {
        error_handler(node, "Undefined concept " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second.result;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(
    const ast::Concepts& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> concepts;
    for (const auto& child : node.definitions) {
        concepts.insert(parse(child, error_handler, context));
    }
    return concepts;
}

std::string parse(
    const ast::RoleDefinition& node, const dlplan::error_handler_type& error_handler, Context& context) {
    const auto key = parse(node.key, error_handler, context);
    auto it = context.roles.find(key);
    if (it != context.roles.end()) {
        error_handler(node, "Multiple definitions of role " + key);
        error_handler(it->second.node, "Previous definition: ");
        throw std::runtime_error("Failed parse.");
    }
    return key;
}
std::shared_ptr<const dlplan::core::Role> parse(
    const ast::RoleImplementation& node, const dlplan::error_handler_type& error_handler, Context& context) {
    return dlplan::core::parse(node.role, error_handler, *context.policy_factory.get_element_factory());
}

std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(
    const ast::Role& node, const error_handler_type& error_handler, Context& context) {
    const auto definition = parse(node.definition, error_handler, context);
    const auto implementation = parse(node.implementation, error_handler, context);
    auto named_role = context.policy_factory.make_role(definition, implementation);
    context.roles.emplace(definition, NamedRoleData{ node.definition, named_role });
    return {definition, named_role};
}

std::shared_ptr<const dlplan::policy::NamedRole> parse(
    const ast::RoleReference& node, const error_handler_type& error_handler, Context& context) {
    auto key = parse(node.key, error_handler, context);
    auto it = context.roles.find(key);
    if (it == context.roles.end()) {
        error_handler(node, "Undefined role " + key);
        throw std::runtime_error("Failed parse.");
    }
    return it->second.result;
}

std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(
    const ast::Roles& node, const error_handler_type& error_handler, Context& context) {
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> roles;
    for (const auto& child : node.definitions) {
        roles.insert(parse(child, error_handler, context));
    }
    return roles;
}

std::shared_ptr<const BaseCondition> parse(
    const ast::PositiveBooleanCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_pos_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::NegativeBooleanCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_neg_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::GreaterNumericalCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_gt_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::EqualNumericalCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_eq_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::GreaterConceptCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_gt_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::EqualConceptCondition& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_eq_condition(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::PositiveBooleanEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_pos_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::NegativeBooleanEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_neg_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::UnchangedBooleanEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::IncrementNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_inc_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::IncrementOrUnchangedNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_inc_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::DecrementNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_dec_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::DecrementOrUnchangedNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_dec_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::UnchangedNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::GreaterNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_gt_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::EqualNumericalEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_eq_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::IncrementConceptEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_inc_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::DecrementConceptEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_dec_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::UnchangedConceptEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_bot_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::GreaterConceptEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_gt_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseEffect> parse(
    const ast::EqualConceptEffect& node, const error_handler_type& error_handler, Context& context) {
    return context.policy_factory.make_eq_effect(parse(node.reference, error_handler, context));
}

std::shared_ptr<const BaseCondition> parse(
    const ast::FeatureCondition& node, const error_handler_type& error_handler, Context& context) {
    ParseVisitor<std::shared_ptr<const BaseCondition>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}

std::shared_ptr<const BaseEffect> parse(
    const ast::FeatureEffect& node, const error_handler_type& error_handler, Context& context) {
    ParseVisitor<std::shared_ptr<const BaseEffect>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}

std::shared_ptr<const Rule> parse(
    const ast::Rule& node, const error_handler_type& error_handler, Context& context) {
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
    const ast::Rules& node, const error_handler_type& error_handler, Context& context) {
    Rules rules;
    for (const auto& child : node.rules) {
        rules.insert(parse(child, error_handler, context));
    }
    return rules;
}

std::shared_ptr<const Policy> parse(
    const ast::Policy& node, const dlplan::error_handler_type& error_handler, Context& context) {
    if (node.booleans.has_value()) {
        parse(node.booleans.value(), error_handler, context);
    }
    if (node.numericals.has_value()) {
        parse(node.numericals.value(), error_handler, context);
    }
    return context.policy_factory.make_policy(parse(node.rules, error_handler, context));
}

}