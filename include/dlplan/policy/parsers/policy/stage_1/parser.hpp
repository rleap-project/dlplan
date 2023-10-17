#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_PARSER_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace dlplan::policy::parsers::policy::stage_1
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // Parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct NameClass;
        struct BooleanDefinitionClass;
        struct BooleanReferenceClass;
        struct BooleansEntryClass;
        struct NumericalDefinitionClass;
        struct NumericalReferenceClass;
        struct NumericalsEntryClass;
        struct PositiveBooleanConditionEntryClass;
        struct NegativeBooleanConditionEntryClass;
        struct GreaterNumericalConditionEntryClass;
        struct EqualNumericalConditionEntryClass;
        struct PositiveBooleanEffectEntryClass;
        struct NegativeBooleanEffectEntryClass;
        struct UnchangedBooleanEffectEntryClass;
        struct IncrementNumericalEffectEntryClass;
        struct DecrementNumericalEffectEntryClass;
        struct UnchangedNumericalEffectEntryClass;
        struct FeatureConditionEntryClass;
        struct FeatureEffectEntryClass;
        struct RuleEntryClass;
        struct RulesClass;
        struct PolicyClass;

        typedef x3::rule<NameClass, ast::Name> name_type;
        typedef x3::rule<BooleanDefinitionClass, ast::BooleanDefinition> boolean_definition_type;
        typedef x3::rule<BooleanReferenceClass, ast::BooleanReference> boolean_reference_type;
        typedef x3::rule<BooleansEntryClass, ast::BooleansEntry> booleans_entry_type;
        typedef x3::rule<NumericalDefinitionClass, ast::NumericalDefinition> numerical_definition_type;
        typedef x3::rule<NumericalReferenceClass, ast::NumericalReference> numerical_reference_type;
        typedef x3::rule<NumericalsEntryClass, ast::NumericalsEntry> numericals_entry_type;
        typedef x3::rule<PositiveBooleanConditionEntryClass, ast::PositiveBooleanConditionEntry> positive_boolean_condition_entry_type;
        typedef x3::rule<NegativeBooleanConditionEntryClass, ast::NegativeBooleanConditionEntry> negative_boolean_condition_entry_type;
        typedef x3::rule<GreaterNumericalConditionEntryClass, ast::GreaterNumericalConditionEntry> greater_numerical_condition_entry_type;
        typedef x3::rule<EqualNumericalConditionEntryClass, ast::EqualNumericalConditionEntry> equal_numerical_condition_entry_type;
        typedef x3::rule<PositiveBooleanEffectEntryClass, ast::PositiveBooleanEffectEntry> positive_boolean_effect_entry_type;
        typedef x3::rule<NegativeBooleanEffectEntryClass, ast::NegativeBooleanEffectEntry> negative_boolean_effect_entry_type;
        typedef x3::rule<UnchangedBooleanEffectEntryClass, ast::UnchangedBooleanEffectEntry> unchanged_boolean_effect_entry_type;
        typedef x3::rule<IncrementNumericalEffectEntryClass, ast::IncrementNumericalEffectEntry> increment_numerical_effect_entry_type;
        typedef x3::rule<DecrementNumericalEffectEntryClass, ast::DecrementNumericalEffectEntry> decrement_numerical_effect_entry_type;
        typedef x3::rule<UnchangedNumericalEffectEntryClass, ast::UnchangedNumericalEffectEntry> unchanged_numerical_effect_entry_type;
        typedef x3::rule<FeatureConditionEntryClass, ast::FeatureConditionEntry> feature_condition_entry_type;
        typedef x3::rule<FeatureEffectEntryClass, ast::FeatureEffectEntry> feature_effect_entry_type;
        typedef x3::rule<RuleEntryClass, ast::RuleEntry> rule_entry_type;
        typedef x3::rule<RulesClass, ast::Rules> rules_type;
        typedef x3::rule<PolicyClass, ast::Policy> policy_type;

        BOOST_SPIRIT_DECLARE(name_type,
            boolean_definition_type, boolean_reference_type, booleans_entry_type,
            numerical_definition_type, numerical_reference_type, numericals_entry_type,
            positive_boolean_condition_entry_type, negative_boolean_condition_entry_type,
            greater_numerical_condition_entry_type, equal_numerical_condition_entry_type,
            positive_boolean_effect_entry_type, negative_boolean_effect_entry_type, unchanged_boolean_effect_entry_type,
            increment_numerical_effect_entry_type, decrement_numerical_effect_entry_type, unchanged_numerical_effect_entry_type,
            feature_condition_entry_type, feature_effect_entry_type,
            rule_entry_type, rules_type, policy_type)
    }

    parser::name_type const& name();
    parser::boolean_definition_type const& boolean_definition();
    parser::boolean_reference_type const& boolean_reference();
    parser::booleans_entry_type const& booleans_entry();
    parser::numerical_definition_type const& numerical_definition();
    parser::numerical_reference_type const& numerical_reference();
    parser::numericals_entry_type const& numericals_entry();
    parser::positive_boolean_condition_entry_type const& positive_boolean_condition_entry();
    parser::negative_boolean_condition_entry_type const& negative_boolean_condition_entry();
    parser::greater_numerical_condition_entry_type const& greater_numerical_condition_entry();
    parser::equal_numerical_condition_entry_type const& equal_numerical_condition_entry();
    parser::positive_boolean_effect_entry_type const& positive_boolean_effect_entry();
    parser::negative_boolean_effect_entry_type const& negative_boolean_effect_entry();
    parser::unchanged_boolean_effect_entry_type const& unchanged_boolean_effect_entry();
    parser::increment_numerical_effect_entry_type const& increment_numerical_effect_entry();
    parser::decrement_numerical_effect_entry_type const& decrement_numerical_effect_entry();
    parser::unchanged_numerical_effect_entry_type const& unchanged_numerical_effect_entry();
    parser::feature_condition_entry_type const& feature_condition_entry();
    parser::feature_effect_entry_type const& feature_effect_entry();
    parser::rule_entry_type const& rule_entry();
    parser::rules_type const& rules();
    parser::policy_type const& policy();
}

#endif