#ifndef DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_PARSER_DEF_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/dlplan/core/parsers/elements/stage_1/parser.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/error_handler.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


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

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameInnerClass;
    struct FeatureConditionEntryInnerClass;
    struct FeatureEffectEntryInnerClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    /* Private rules with annotations */
    x3::rule<NameInnerClass, ast::NameInner> const
        name_inner = "name_inner";

    x3::rule<FeatureConditionEntryInnerClass, ast::FeatureConditionEntryInner> const
        feature_condition_entry_inner = "feature_condition_entry_inner";

    x3::rule<FeatureEffectEntryInnerClass, ast::FeatureEffectEntryInner> const
        feature_effect_entry_inner = "feature_effect_entry_inner";

    /* Privates rules with annotatations and error handler */
    policy_root_type const policy_root = "policy";

    /* Public rules with annotations */
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

    decrement_numerical_effect_entry_type const decrement_numerical_effect_entry = "decrement_numerical_effect_entry";

    unchanged_numerical_effect_entry_type const unchanged_numerical_effect_entry = "unchanged_numerical_effect_entry";

    feature_condition_entry_type const feature_condition_entry = "feature_condition_entry";

    feature_effect_entry_type const feature_effect_entry = "feature_effect_entry";

    rule_entry_type const rule_entry = "rule_entry";

    rules_type const rules = "rules";

    policy_type const policy = "policy";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_inner_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];
    const auto name_def = eps > name_inner;

    const auto boolean_definition_def = lit('(') > name > lit('"') > dlplan::core::parsers::elements::stage_1::boolean() > lit('"') > lit(')');

    const auto boolean_reference_def = eps > name;

    const auto booleans_entry_def = lit('(') >> lit(":booleans") > *boolean_definition > lit(')');

    const auto numerical_definition_def = lit('(') > name > lit('"') > dlplan::core::parsers::elements::stage_1::numerical() > lit('"') > lit(')');

    const auto numerical_reference_def = eps > name;

    const auto numericals_entry_def = lit('(') >> lit(":numericals") > *numerical_definition > lit(')');

    const auto positive_boolean_condition_entry_def = lit(":c_b_pos") > boolean_reference;

    const auto negative_boolean_condition_entry_def = lit(":c_b_neg") > boolean_reference;

    const auto greater_numerical_condition_entry_def = lit(":c_n_gt") > numerical_reference;

    const auto equal_numerical_condition_entry_def = lit(":c_n_eq") > numerical_reference;

    const auto positive_boolean_effect_entry_def = lit(":e_b_pos") > boolean_reference;

    const auto negative_boolean_effect_entry_def = lit(":e_b_neg") > boolean_reference;

    const auto unchanged_boolean_effect_entry_def = lit(":e_b_bot") > boolean_reference;

    const auto increment_numerical_effect_entry_def = lit(":e_n_inc") > numerical_reference;

    const auto decrement_numerical_effect_entry_def = lit(":e_n_dec") > numerical_reference;

    const auto unchanged_numerical_effect_entry_def = lit(":e_n_bot") > numerical_reference;

    const auto feature_condition_entry_inner_def =
        positive_boolean_condition_entry | negative_boolean_condition_entry | greater_numerical_condition_entry | equal_numerical_condition_entry;

    const auto feature_condition_entry_def = lit('(') > feature_condition_entry_inner > lit(')');

    const auto feature_effect_entry_inner_def =
        positive_boolean_effect_entry | negative_boolean_effect_entry | unchanged_boolean_effect_entry | increment_numerical_effect_entry | decrement_numerical_effect_entry | unchanged_numerical_effect_entry;

    const auto feature_effect_entry_def = lit('(') > feature_effect_entry_inner > lit(')');

    const auto rule_entry_def = lit('(') >> lit(":rule")
        > lit('(') > lit(":conditions") > *feature_condition_entry > lit(')')
        > lit('(') > lit(":effects") > *feature_effect_entry > lit(')')
        > lit(')');

    const auto rules_def = *rule_entry;

    const auto policy_def = lit('(') > lit(":policy")
        > booleans_entry
        > numericals_entry
        > rules
        > lit(')');

    const auto policy_root_def = policy;

    BOOST_SPIRIT_DEFINE(
        name_inner, name, boolean_definition, boolean_reference, booleans_entry, numerical_definition, numerical_reference, numericals_entry,
        positive_boolean_condition_entry, negative_boolean_condition_entry, greater_numerical_condition_entry, equal_numerical_condition_entry,
        positive_boolean_effect_entry, negative_boolean_effect_entry, unchanged_boolean_effect_entry, increment_numerical_effect_entry, decrement_numerical_effect_entry, unchanged_numerical_effect_entry,
        feature_condition_entry_inner, feature_condition_entry, feature_effect_entry_inner, feature_effect_entry, rule_entry, rules, policy, policy_root)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameInnerClass : x3::annotate_on_success {};
    struct NameClass : x3::annotate_on_success {};
    struct BooleanDefinitionClass : x3::annotate_on_success {};
    struct BooleanReferenceClass : x3::annotate_on_success {};
    struct BooleansEntryClass : x3::annotate_on_success {};
    struct NumericalDefinitionClass : x3::annotate_on_success {};
    struct NumericalReferenceClass : x3::annotate_on_success {};
    struct NumericalsEntryClass : x3::annotate_on_success {};
    struct PositiveBooleanConditionEntryClass : x3::annotate_on_success {};
    struct NegativeBooleanConditionEntryClass : x3::annotate_on_success {};
    struct GreaterNumericalConditionEntryClass : x3::annotate_on_success {};
    struct EqualNumericalConditionEntryClass : x3::annotate_on_success {};
    struct PositiveBooleanEffectEntryClass : x3::annotate_on_success {};
    struct NegativeBooleanEffectEntryClass : x3::annotate_on_success {};
    struct UnchangedBooleanEffectEntryClass : x3::annotate_on_success {};
    struct IncrementNumericalEffectEntryClass : x3::annotate_on_success {};
    struct DecrementNumericalEffectEntryClass : x3::annotate_on_success {};
    struct UnchangedNumericalEffectEntryClass : x3::annotate_on_success {};
    struct FeatureConditionEntryInnerClass : x3::annotate_on_success {};
    struct FeatureConditionEntryClass : x3::annotate_on_success {};
    struct FeatureEffectEntryInnerClass : x3::annotate_on_success {};
    struct FeatureEffectEntryClass : x3::annotate_on_success {};
    struct RuleEntryClass : x3::annotate_on_success {};
    struct RulesClass : x3::annotate_on_success {};
    struct PolicyClass : x3::annotate_on_success {};
    struct PolicyRootClass : x3::annotate_on_success, error_handler_policy {};
}

namespace dlplan::policy::parsers::policy::stage_1
{
    parser::name_type const& name() {
        return parser::name;
    }

    parser::boolean_definition_type const& boolean_definition() {
        return parser::boolean_definition;
    }

    parser::boolean_reference_type const& boolean_reference() {
        return parser::boolean_reference;
    }

    parser::booleans_entry_type const& booleans_entry() {
        return parser::booleans_entry;
    }

    parser::numerical_definition_type const& numerical_definition() {
        return parser::numerical_definition;
    }

    parser::numerical_reference_type const& numerical_reference() {
        return parser::numerical_reference;
    }

    parser::numericals_entry_type const& numericals_entry() {
        return parser::numericals_entry;
    }

    parser::positive_boolean_condition_entry_type const& positive_boolean_condition_entry() {
        return parser::positive_boolean_condition_entry;
    }

    parser::negative_boolean_condition_entry_type const& negative_boolean_condition_entry() {
        return parser::negative_boolean_condition_entry;
    }

    parser::greater_numerical_condition_entry_type const& greater_numerical_condition_entry() {
        return parser::greater_numerical_condition_entry;
    }

    parser::equal_numerical_condition_entry_type const& equal_numerical_condition_entry() {
        return parser::equal_numerical_condition_entry;
    }

    parser::positive_boolean_effect_entry_type const& positive_boolean_effect_entry() {
        return parser::positive_boolean_effect_entry;
    }

    parser::negative_boolean_effect_entry_type const& negative_boolean_effect_entry() {
        return parser::negative_boolean_effect_entry;
    }

    parser::unchanged_boolean_effect_entry_type const& unchanged_boolean_effect_entry() {
        return parser::unchanged_boolean_effect_entry;
    }

    parser::increment_numerical_effect_entry_type const& increment_numerical_effect_entry() {
        return parser::increment_numerical_effect_entry;
    }

    parser::decrement_numerical_effect_entry_type const& decrement_numerical_effect() {
        return parser::decrement_numerical_effect_entry;
    }

    parser::unchanged_numerical_effect_entry_type const& unchanged_numerical_effect_entry() {
        return parser::unchanged_numerical_effect_entry;
    }

    parser::feature_condition_entry_type const& feature_condition_entry() {
        return parser::feature_condition_entry;
    }

    parser::feature_effect_entry_type const& feature_effect_entry() {
        return parser::feature_effect_entry;
    }

    parser::rule_entry_type const& rule_entry() {
        return parser::rule_entry;
    }

    parser::rules_type const& rules() {
        return parser::rules;
    }

    parser::policy_type const& policy() {
        return parser::policy;
    }
}


namespace dlplan::policy::parsers::policy::stage_1 {
    parser::policy_root_type const& policy_root() {
        return parser::policy_root;
    }
}

#endif
