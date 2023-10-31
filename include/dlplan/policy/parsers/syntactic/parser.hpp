#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_PARSER_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace dlplan::policy
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // Parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct NameClass;
        struct BooleanDefinitionClass;
        struct BooleanImplementationClass;
        struct BooleanClass;
        struct BooleanReferenceClass;
        struct BooleansClass;
        struct NumericalDefinitionClass;
        struct NumericalImplementationClass;
        struct NumericalClass;
        struct NumericalReferenceClass;
        struct NumericalsClass;
        struct ConceptDefinitionClass;
        struct ConceptImplementationClass;
        struct ConceptClass;
        struct ConceptReferenceClass;
        struct ConceptsClass;
        struct RoleDefinitionClass;
        struct RoleImplementationClass;
        struct RoleClass;
        struct RoleReferenceClass;
        struct RolesClass;
        struct PositiveBooleanConditionClass;
        struct NegativeBooleanConditionClass;
        struct GreaterNumericalConditionClass;
        struct EqualNumericalConditionClass;
        struct PositiveBooleanEffectClass;
        struct NegativeBooleanEffectClass;
        struct UnchangedBooleanEffectClass;
        struct IncrementNumericalEffectClass;
        struct DecrementNumericalEffectClass;
        struct UnchangedNumericalEffectClass;
        struct FeatureConditionClass;
        struct FeatureEffectClass;
        struct RuleClass;
        struct RulesClass;
        struct PolicyClass;

        typedef x3::rule<NameClass, ast::Name> name_type;

        typedef x3::rule<BooleanDefinitionClass, ast::BooleanDefinition> boolean_definition_type;
        typedef x3::rule<BooleanImplementationClass, ast::BooleanImplementation> boolean_implementation_type;
        typedef x3::rule<BooleanClass, ast::Boolean> boolean_type;
        typedef x3::rule<BooleanReferenceClass, ast::BooleanReference> boolean_reference_type;
        typedef x3::rule<BooleansClass, ast::Booleans> booleans_type;

        typedef x3::rule<NumericalDefinitionClass, ast::NumericalDefinition> numerical_definition_type;
        typedef x3::rule<NumericalImplementationClass, ast::NumericalImplementation> numerical_implementation_type;
        typedef x3::rule<NumericalClass, ast::Numerical> numerical_type;
        typedef x3::rule<NumericalReferenceClass, ast::NumericalReference> numerical_reference_type;
        typedef x3::rule<NumericalsClass, ast::Numericals> numericals_type;

        typedef x3::rule<ConceptDefinitionClass, ast::ConceptDefinition> concept_definition_type;
        typedef x3::rule<ConceptImplementationClass, ast::ConceptImplementation> concept_implementation_type;
        typedef x3::rule<ConceptClass, ast::Concept> concept_type;
        typedef x3::rule<ConceptReferenceClass, ast::ConceptReference> concept_reference_type;
        typedef x3::rule<ConceptsClass, ast::Concepts> concepts_type;

        typedef x3::rule<RoleDefinitionClass, ast::RoleDefinition> role_definition_type;
        typedef x3::rule<RoleImplementationClass, ast::RoleImplementation> role_implementation_type;
        typedef x3::rule<RoleClass, ast::Role> role_type;
        typedef x3::rule<RoleReferenceClass, ast::RoleReference> role_reference_type;
        typedef x3::rule<RolesClass, ast::Roles> roles_type;

        typedef x3::rule<PositiveBooleanConditionClass, ast::PositiveBooleanConditionEntry> positive_boolean_condition_type;
        typedef x3::rule<NegativeBooleanConditionClass, ast::NegativeBooleanConditionEntry> negative_boolean_condition_type;
        typedef x3::rule<GreaterNumericalConditionClass, ast::GreaterNumericalConditionEntry> greater_numerical_condition_type;
        typedef x3::rule<EqualNumericalConditionClass, ast::EqualNumericalConditionEntry> equal_numerical_condition_type;
        typedef x3::rule<PositiveBooleanEffectClass, ast::PositiveBooleanEffectEntry> positive_boolean_effect_type;
        typedef x3::rule<NegativeBooleanEffectClass, ast::NegativeBooleanEffectEntry> negative_boolean_effect_type;
        typedef x3::rule<UnchangedBooleanEffectClass, ast::UnchangedBooleanEffectEntry> unchanged_boolean_effect_type;
        typedef x3::rule<IncrementNumericalEffectClass, ast::IncrementNumericalEffectEntry> increment_numerical_effect_type;
        typedef x3::rule<DecrementNumericalEffectClass, ast::DecrementNumericalEffect> decrement_numerical_effect_type;
        typedef x3::rule<UnchangedNumericalEffectClass, ast::UnchangedNumericalEffect> unchanged_numerical_effect_type;
        typedef x3::rule<FeatureConditionClass, ast::FeatureCondition> feature_condition_type;
        typedef x3::rule<FeatureEffectClass, ast::FeatureEffect> feature_effect_type;

        typedef x3::rule<RuleClass, ast::Rule> rule_type;
        typedef x3::rule<RulesClass, ast::Rules> rules_type;

        typedef x3::rule<PolicyClass, ast::Policy> policy_type;

        BOOST_SPIRIT_DECLARE(name_type,
            boolean_definition_type, boolean_implementation_type, boolean_type, boolean_reference_type, booleans_type,
            numerical_definition_type, numerical_implementation_type, numerical_type, numerical_reference_type, numericals_type,
            concept_definition_type, concept_implementation_type, concept_type, concept_reference_type, concepts_type,
            role_definition_type, role_implementation_type, role_type, role_reference_type, roles_type,
            positive_boolean_condition_type, negative_boolean_condition_type,
            greater_numerical_condition_type, equal_numerical_condition_type,
            positive_boolean_effect_type, negative_boolean_effect_type, unchanged_boolean_effect_type,
            increment_numerical_effect_type, decrement_numerical_effect_type, unchanged_numerical_effect_type,
            feature_condition_type, feature_effect_type,
            rule_type, rules_type, policy_type)
    }

    parser::name_type const& name();

    parser::boolean_definition_type const& boolean_definition();
    parser::boolean_implementation_type const& boolean_implementation();
    parser::boolean_type const& boolean();
    parser::boolean_reference_type const& boolean_reference();
    parser::booleans_type const& booleans();

    parser::numerical_definition_type const& numerical_definition();
    parser::numerical_implementation_type const& numerical_implementation();
    parser::numerical_type const& numerical();
    parser::numerical_reference_type const& numerical_reference();
    parser::numericals_type const& numericals();

    parser::concept_definition_type const& concept_definition();
    parser::concept_implementation_type const& concept_implementation();
    parser::concept_type const& concept();
    parser::concept_reference_type const& concept_reference();
    parser::concepts_type const& concepts();

    parser::role_definition_type const& role_definition();
    parser::role_implementation_type const& role_implementation();
    parser::role_type const& role();
    parser::role_reference_type const& role_reference();
    parser::roles_type const& roles();

    parser::positive_boolean_condition_type const& positive_boolean_condition();
    parser::negative_boolean_condition_type const& negative_boolean_condition();
    parser::greater_numerical_condition_type const& greater_numerical_condition();
    parser::equal_numerical_condition_type const& equal_numerical_condition();
    parser::positive_boolean_effect_type const& positive_boolean_effect();
    parser::negative_boolean_effect_type const& negative_boolean_effect();
    parser::unchanged_boolean_effect_type const& unchanged_boolean_effect();
    parser::increment_numerical_effect_type const& increment_numerical_effect();
    parser::decrement_numerical_effect_type const& decrement_numerical_effect();
    parser::unchanged_numerical_effect_type const& unchanged_numerical_effect();

    parser::feature_condition_type const& feature_condition();
    parser::feature_effect_type const& feature_effect();

    parser::rule_type const& rule();
    parser::rules_type const& rules();

    parser::policy_type const& policy();
}

#endif