#ifndef DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_PARSER_DEF_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/dlplan/common/parsers/error_handler.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/parser.hpp"

#include "ast_adapted.hpp"


namespace dlplan::policy::parsers::policy::stage_1::parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::int_;
    using x3::eps;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    using error_handler_base = dlplan::common::parsers::error_handler_base;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    name_type const name = "name";

    boolean_definition_type const boolean_definition = "boolean_definition";

    boolean_reference_type const boolean_reference = "boolean_reference";

    booleans_entry_type const booleans_entry = "booleans_entry";

    numerical_definition_type const numerical_definition = "numerical_definition";

    numerical_reference_type const numerical_reference = "numerical_reference";

    numericals_entry_type const numericals_entry = "numericals_entry";

    positive_boolean_condition_entry_type const positive_boolean_condition_entry = "positive_boolean_condition_entry";

    negative_boolean_condition_entry_type const negative_boolean_condition_entry = "negative_boolean_condition_entry";

    greater_numerical_condition_entry_type const greater_numerical_condition_entry = "greater_numerical_condition_entry";

    equal_numerical_condition_entry_type const equal_numerical_condition_entry = "equal_numerical_condition_entry";

    positive_boolean_effect_entry_type const positive_boolean_effect_entry = "positive_boolean_effect_entry";

    negative_boolean_effect_entry_type const negative_boolean_effect_entry = "negative_boolean_effect_entry";

    unchanged_boolean_effect_entry_type const unchanged_boolean_effect_entry = "unchanged_boolean_effect_entry";

    increment_numerical_effect_entry_type const increment_numerical_effect_entry = "increment_numerical_effect_entry";

    decrement_numerical_effect_type const decrement_numerical_effect = "decrement_numerical_effect";

    unchanged_numerical_effect_entry_type const unchanged_numerical_effect_entry = "unchanged_numerical_effect_entry";

    feature_condition_entry_type const feature_condition_entry = "feature_condition_entry";

    feature_effect_entry_type const feature_effect_entryname = "feature_effect_entry";

    rule_entry_type const rule_entry = "rule_entry";

    rules_type const rules = "rules";

    policy_type const policy = "policy";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];

    BOOST_SPIRIT_DEFINE(
        name)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success, error_handler_base {};
    struct BooleanDefinitionClass : x3::annotate_on_success, error_handler_base {};
    struct BooleanReferenceClass : x3::annotate_on_success, error_handler_base {};
    struct BooleansEntryClass : x3::annotate_on_success, error_handler_base {};
    struct NumericalDefinitionClass : x3::annotate_on_success, error_handler_base {};
    struct NumericalReferenceClass : x3::annotate_on_success, error_handler_base {};
    struct NumericalsEntryClass : x3::annotate_on_success, error_handler_base {};
    struct PositiveBooleanConditionEntryClass : x3::annotate_on_success, error_handler_base {};
    struct NegativeBooleanConditionEntryClass : x3::annotate_on_success, error_handler_base {};
    struct GreaterNumericalConditionEntryClass : x3::annotate_on_success, error_handler_base {};
    struct EqualNumericalConditionEntryClass : x3::annotate_on_success, error_handler_base {};
    struct PositiveBooleanEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct NegativeBooleanEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct UnchangedBooleanEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct IncrementNumericalEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct DecrementNumericalEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct UnchangedNumericalEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct FeatureConditionEntryClass : x3::annotate_on_success, error_handler_base {};
    struct FeatureEffectEntryClass : x3::annotate_on_success, error_handler_base {};
    struct RuleEntryClass : x3::annotate_on_success, error_handler_base {};
    struct RulesClass : x3::annotate_on_success, error_handler_base {};
    struct PolicyClass : x3::annotate_on_success, error_handler_base {};
}

namespace dlplan::policy::parsers::policy::stage_1
{
    parser::name_type const& name() {
        return name;
    }

    parser::boolean_definition_type const& boolean_definition() {
        return boolean_definition;
    }

    parser::boolean_reference_type const& boolean_reference() {
        return boolean_reference;
    }

    parser::booleans_entry_type const& booleans_entry() {
        return booleans_entry;
    }

    parser::numerical_definition_type const& numerical_definition() {
        return numerical_definition;
    }

    parser::numerical_reference_type const& numerical_reference() {
        return numerical_reference;
    }

    parser::numericals_entry_type const& numericals_entry() {
        return numericals_entry;
    }

    parser::positive_boolean_condition_entry_type const& positive_boolean_condition_entry() {
        return positive_boolean_condition_entry;
    }

    parser::negative_boolean_condition_entry_type const& negative_boolean_condition_entry() {
        return negative_boolean_condition_entry;
    }

    parser::greater_numerical_condition_entry_type const& greater_numerical_condition_entry() {
        return greater_numerical_condition_entry;
    }

    parser::equal_numerical_condition_entry_type const& equal_numerical_condition_entry() {
        return equal_numerical_condition_entry;
    }

    parser::positive_boolean_effect_entry_type const& positive_boolean_effect_entry() {
        return positive_boolean_effect_entry;
    }

    parser::negative_boolean_effect_entry_type const& negative_boolean_effect_entry() {
        return negative_boolean_effect_entry;
    }

    parser::unchanged_boolean_effect_entry_type const& unchanged_boolean_effect_entry() {
        return unchanged_boolean_effect_entry;
    }

    parser::increment_numerical_effect_entry_type const& increment_numerical_effect_entry() {
        return increment_numerical_effect_entry;
    }

    parser::decrement_numerical_effect_type const& decrement_numerical_effect() {
        return decrement_numerical_effect;
    }

    parser::unchanged_numerical_effect_entry_type const& unchanged_numerical_effect_entry() {
        return unchanged_numerical_effect_entry;
    }

    parser::feature_condition_entry_type const& feature_condition_entry() {
        return feature_condition_entry;
    }

    parser::feature_effect_entry_type const& feature_effect_entry() {
        return feature_effect_entry;
    }

    parser::rule_entry_type const& rule_entry() {
        return rule_entry;
    }

    parser::rules_type const& rules() {
        return rules;
    }

    parser::policy_type const& policy() {
        return policy;
    }
}

#endif