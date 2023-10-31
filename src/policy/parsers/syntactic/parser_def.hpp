#ifndef DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_PARSER_DEF_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_SYNTACTIC_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/dlplan/core/parsers/syntactic/parser.hpp"
#include "include/dlplan/policy/parsers/syntactic/error_handler.hpp"
#include "include/dlplan/policy/parsers/syntactic/ast.hpp"
#include "include/dlplan/policy/parsers/syntactic/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace dlplan::policy::parser
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

    struct FeatureConditionEntryInnerClass;
    struct FeatureEffectEntryInnerClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    /* Privates rules with annotatations and error handler */
    policy_root_type const policy_root = "policy";

    /* Public rules with annotations */
    name_type const name = "name";

    boolean_type const boolean = "boolean";

    boolean_reference_type const boolean_reference = "boolean_reference";

    booleans_type const booleans = "booleans";

    numerical_type const numerical = "numerical";

    numerical_reference_type const numerical_reference = "numerical_reference";

    numericals_type const numericals = "numericals";

    concept_type const concept = "concept";

    concept_reference_type const concept_reference = "concept_reference";

    concepts_type const concepts = "concepts";

    role_type const role = "role";

    role_reference_type const role_reference = "role_reference";

    roles_type const roles = "roles";

    positive_boolean_condition_type const positive_boolean_condition = "positive_boolean_condition";

    negative_boolean_condition_type const negative_boolean_condition = "negative_boolean_condition";

    greater_numerical_condition_type const greater_numerical_condition = "greater_numerical_condition";

    equal_numerical_condition_type const equal_numerical_condition = "equal_numerical_condition";

    positive_boolean_effect_type const positive_boolean_effect = "positive_boolean_effect";

    negative_boolean_effect_type const negative_boolean_effect = "negative_boolean_effect";

    unchanged_boolean_effect_type const unchanged_boolean_effect = "unchanged_boolean_effect";

    increment_numerical_effect_type const increment_numerical_effect = "increment_numerical_effect";

    decrement_numerical_effect_type const decrement_numerical_effect = "decrement_numerical_effect";

    unchanged_numerical_effect_type const unchanged_numerical_effect = "unchanged_numerical_effect";

    feature_condition_type const feature_condition = "feature_condition";

    feature_effect_type const feature_effect = "feature_effect";

    rule_type const rule = "rule";

    rules_type const rules = "rules";

    policy_type const policy = "policy";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];

    const auto boolean_def = lit('(') > name > lit('"') > dlplan::core::boolean() > lit('"') > lit(')');
    const auto boolean_reference_def = name;
    const auto booleans_def = lit('(') >> lit(":booleans") > *boolean > lit(')');

    const auto numerical_def = lit('(') > name > lit('"') > dlplan::core::numerical() > lit('"') > lit(')');
    const auto numerical_reference_def = name;
    const auto numericals_def = lit('(') >> lit(":numericals") > *numerical > lit(')');

    const auto concept_def = lit('(') > name > lit('"') > dlplan::core::concept() > lit('"') > lit(')');
    const auto concept_reference_def = name;
    const auto concepts_def = lit('(') >> lit(":concepts") > *concept > lit(')');

    const auto role_def = lit('(') > name > lit('"') > dlplan::core::role() > lit('"') > lit(')');
    const auto role_reference_def = name;
    const auto roles_def = lit('(') >> lit(":roles") > *role > lit(')');

    const auto positive_boolean_condition_def = lit('(') >> lit(":c_b_pos") > boolean_reference > lit(')');
    const auto negative_boolean_condition_def = lit('(') >> lit(":c_b_neg") > boolean_reference > lit(')');
    const auto greater_numerical_condition_def = lit('(') >> lit(":c_n_gt") > numerical_reference > lit(')');
    const auto equal_numerical_condition_def = lit('(') >> lit(":c_n_eq") > numerical_reference > lit(')');

    const auto positive_boolean_effect_def = lit('(') >> lit(":e_b_pos") > boolean_reference > lit(')');
    const auto negative_boolean_effect_def = lit('(') >> lit(":e_b_neg") > boolean_reference > lit(')');
    const auto unchanged_boolean_effect_def = lit('(') >> lit(":e_b_bot") > boolean_reference > lit(')');
    const auto increment_numerical_effect_def = lit('(') >> lit(":e_n_inc") > numerical_reference > lit(')');
    const auto decrement_numerical_effect_def = lit('(') >> lit(":e_n_dec") > numerical_reference > lit(')');
    const auto unchanged_numerical_effect_def = lit('(') >> lit(":e_n_bot") > numerical_reference > lit(')');

    const auto feature_condition_def =
        positive_boolean_condition | negative_boolean_condition | greater_numerical_condition | equal_numerical_condition;

    const auto feature_effect_def =
        positive_boolean_effect | negative_boolean_effect | unchanged_boolean_effect | increment_numerical_effect | decrement_numerical_effect | unchanged_numerical_effect;

    const auto rule_def = lit('(') >> lit(":rule")
        > lit('(') > lit(":conditions") >> *feature_condition > lit(')')
        > lit('(') > lit(":effects") >> *feature_effect > lit(')')
        > lit(')');
    const auto rules_def = *rule;

    const auto policy_def = lit('(') > lit(":policy")
        > booleans
        > numericals
        > rules
        > lit(')');
    const auto policy_root_def = policy;

    BOOST_SPIRIT_DEFINE(
        name,
        boolean, boolean_reference, booleans,
        numerical, numerical_reference, numericals,
        concept, concept_reference, concepts,
        role, role_reference, roles,
        positive_boolean_condition, negative_boolean_condition, greater_numerical_condition, equal_numerical_condition,
        positive_boolean_effect, negative_boolean_effect, unchanged_boolean_effect, increment_numerical_effect, decrement_numerical_effect, unchanged_numerical_effect,
        feature_condition, feature_effect, rule, rules, policy, policy_root)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct BooleanClass : x3::annotate_on_success {};
    struct BooleanReferenceClass : x3::annotate_on_success {};
    struct BooleansClass : x3::annotate_on_success {};
    struct NumericalClass : x3::annotate_on_success {};
    struct NumericalReferenceClass : x3::annotate_on_success {};
    struct NumericalsClass : x3::annotate_on_success {};
    struct ConceptClass : x3::annotate_on_success {};
    struct ConceptReferenceClass : x3::annotate_on_success {};
    struct ConceptsClass : x3::annotate_on_success {};
    struct RoleClass : x3::annotate_on_success {};
    struct RoleReferenceClass : x3::annotate_on_success {};
    struct RolesClass : x3::annotate_on_success {};
    struct PositiveBooleanConditionClass : x3::annotate_on_success {};
    struct NegativeBooleanConditionClass : x3::annotate_on_success {};
    struct GreaterNumericalConditionClass : x3::annotate_on_success {};
    struct EqualNumericalConditionClass : x3::annotate_on_success {};
    struct PositiveBooleanEffectClass : x3::annotate_on_success {};
    struct NegativeBooleanEffectClass : x3::annotate_on_success {};
    struct UnchangedBooleanEffectClass : x3::annotate_on_success {};
    struct IncrementNumericalEffectClass : x3::annotate_on_success {};
    struct DecrementNumericalEffectClass : x3::annotate_on_success {};
    struct UnchangedNumericalEffectClass : x3::annotate_on_success {};
    struct FeatureConditionClass : x3::annotate_on_success {};
    struct FeatureEffectClass : x3::annotate_on_success {};
    struct RuleClass : x3::annotate_on_success {};
    struct RulesClass : x3::annotate_on_success {};
    struct PolicyClass : x3::annotate_on_success {};
    struct PolicyRootClass : x3::annotate_on_success, error_handler_policy {};
}

namespace dlplan::policy
{
    parser::name_type const& name() {
        return parser::name;
    }

    parser::boolean_type const& boolean() {
        return parser::boolean;
    }

    parser::boolean_reference_type const& boolean_reference() {
        return parser::boolean_reference;
    }

    parser::booleans_type const& booleans() {
        return parser::booleans;
    }

    parser::numerical_type const& numerical() {
        return parser::numerical;
    }

    parser::numerical_reference_type const& numerical_reference() {
        return parser::numerical_reference;
    }

    parser::numericals_type const& numericals() {
        return parser::numericals;
    }

    parser::concept_type const& concept_definition() {
        return parser::concept;
    }

    parser::concept_reference_type const& concept_reference() {
        return parser::concept_reference;
    }

    parser::concepts_type const& concepts() {
        return parser::concepts;
    }

    parser::role_type const& role() {
        return parser::role;
    }

    parser::role_reference_type const& role_reference() {
        return parser::role_reference;
    }

    parser::roles_type const& roles() {
        return parser::roles;
    }

    parser::positive_boolean_condition_type const& positive_boolean_condition() {
        return parser::positive_boolean_condition;
    }

    parser::negative_boolean_condition_type const& negative_boolean_condition() {
        return parser::negative_boolean_condition;
    }

    parser::greater_numerical_condition_type const& greater_numerical_condition() {
        return parser::greater_numerical_condition;
    }

    parser::equal_numerical_condition_type const& equal_numerical_condition() {
        return parser::equal_numerical_condition;
    }

    parser::positive_boolean_effect_type const& positive_boolean_effect() {
        return parser::positive_boolean_effect;
    }

    parser::negative_boolean_effect_type const& negative_boolean_effect() {
        return parser::negative_boolean_effect;
    }

    parser::unchanged_boolean_effect_type const& unchanged_boolean_effect() {
        return parser::unchanged_boolean_effect;
    }

    parser::increment_numerical_effect_type const& increment_numerical_effect() {
        return parser::increment_numerical_effect;
    }

    parser::decrement_numerical_effect_type const& decrement_numerical_effect() {
        return parser::decrement_numerical_effect;
    }

    parser::unchanged_numerical_effect_type const& unchanged_numerical_effect() {
        return parser::unchanged_numerical_effect;
    }

    parser::feature_condition_type const& feature_condition() {
        return parser::feature_condition;
    }

    parser::feature_effect_type const& feature_effect() {
        return parser::feature_effect;
    }

    parser::rule_type const& rule() {
        return parser::rule;
    }

    parser::rules_type const& rules() {
        return parser::rules;
    }

    parser::policy_type const& policy() {
        return parser::policy;
    }
}


namespace dlplan::policy {
    parser::policy_root_type const& policy_root() {
        return parser::policy_root;
    }
}

#endif
