#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_AST_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_AST_HPP_

#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "include/dlplan/core/parsers/syntactic/ast.hpp"


namespace dlplan::policy::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct NameInner;
    struct Name;
    struct NameEntry;

    struct BooleanDefinition;
    struct BooleanImplementation;
    struct Boolean;
    struct BooleanReference;
    struct Booleans;

    struct NumericalDefinition;
    struct NumericalImplementation;
    struct Numerical;
    struct NumericalReference;
    struct Numericals;

    struct ConceptDefinition;
    struct ConceptImplementation;
    struct Concept;
    struct ConceptReference;
    struct Concepts;

    struct BooleanDefinition;
    struct BooleanImplementation;
    struct PositiveBooleanCondition;
    struct NegativeBooleanCondition;
    struct GreaterNumericalCondition;
    struct EqualNumericalCondition;
    struct PositiveBooleanEffect;
    struct NegativeBooleanEffect;
    struct UnchangedBooleanEffect;
    struct IncrementNumericalEffect;
    struct DecrementNumericalEffect;
    struct UnchangedNumericalEffect;

    struct FeatureCondition;
    struct FeatureEffect;

    struct Rule;
    struct Rules;

    struct Policy;


    /* Basic character compounds */
    struct Name : x3::position_tagged {
        char alphabetical;
        std::string suffix;
    };

    /* Boolean entry and references */
    struct BooleanDefinition : x3::position_tagged {
        Name key;
    };

    struct BooleanImplementation {
        core::ast::Boolean boolean;
    };

    struct Boolean : x3::position_tagged {
        BooleanDefinition definition;
        BooleanImplementation implementation;
    };

    struct BooleanReference : x3::position_tagged {
        Name key;
    };

    struct Booleans : x3::position_tagged {
        std::vector<Boolean> definitions;
    };


    /* Numerical entry and references */
    struct NumericalDefinition : x3::position_tagged {
        Name key;
    };

    struct NumericalImplementation {
        core::ast::Numerical numerical;
    };

    struct Numerical : x3::position_tagged {
        NumericalDefinition definition;
        NumericalImplementation implementation;
    };

    struct NumericalReference : x3::position_tagged {
        Name key;
    };

    struct Numericals : x3::position_tagged {
        std::vector<Numerical> definitions;
    };


    /* Concept entry and references */
    struct ConceptDefinition : x3::position_tagged {
        Name key;
    };

    struct ConceptImplementation {
        core::ast::Concept concept;
    };

    struct Concept : x3::position_tagged {
        ConceptDefinition definition;
        ConceptImplementation implementation;
    };

    struct ConceptReference : x3::position_tagged {
        Name key;
    };

    struct Concepts : x3::position_tagged {
        std::vector<Concept> definitions;
    };


    /* Role entry and references */
    struct RoleDefinition : x3::position_tagged {
        Name key;
    };

    struct RoleImplementation {
        core::ast::Role role;
    };

    struct Role : x3::position_tagged {
        RoleDefinition definition;
        RoleImplementation implementation;
    };

    struct RoleReference : x3::position_tagged {
        Name key;
    };

    struct Roles : x3::position_tagged {
        std::vector<Role> definitions;
    };


    /* Condition and effects */
    struct PositiveBooleanConditionEntry : x3::position_tagged {
        BooleanReference reference;
    };

    struct NegativeBooleanConditionEntry : x3::position_tagged {
        BooleanReference reference;
    };

    struct GreaterNumericalConditionEntry : x3::position_tagged {
        NumericalReference reference;
    };

    struct EqualNumericalConditionEntry : x3::position_tagged {
        NumericalReference reference;
    };

    struct FeatureCondition : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanConditionEntry>,
            x3::forward_ast<NegativeBooleanConditionEntry>,
            x3::forward_ast<GreaterNumericalConditionEntry>,
            x3::forward_ast<EqualNumericalConditionEntry>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    struct PositiveBooleanEffectEntry : x3::position_tagged {
        BooleanReference reference;
    };

    struct NegativeBooleanEffectEntry : x3::position_tagged {
        BooleanReference reference;
    };

    struct UnchangedBooleanEffectEntry : x3::position_tagged {
        BooleanReference reference;
    };

    struct IncrementNumericalEffectEntry : x3::position_tagged {
        NumericalReference reference;
    };

    struct DecrementNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct UnchangedNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct FeatureEffect : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanEffectEntry>,
            x3::forward_ast<NegativeBooleanEffectEntry>,
            x3::forward_ast<UnchangedBooleanEffectEntry>,
            x3::forward_ast<IncrementNumericalEffectEntry>,
            x3::forward_ast<DecrementNumericalEffect>,
            x3::forward_ast<UnchangedNumericalEffect>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Rule : x3::position_tagged {
        std::vector<FeatureCondition> feature_conditions;
        std::vector<FeatureEffect> feature_effects;
    };

    struct Rules : x3::position_tagged {
        std::vector<Rule> rules;
    };

    /* Policy */
    struct Policy : x3::position_tagged {
        Booleans booleans;
        Numericals numericals;
        Rules rules;
    };
}


#endif