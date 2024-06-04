#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_AST_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SYNTACTIC_AST_HPP_

#include "../../../core/parsers/syntactic/ast.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>

#include <vector>


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

    struct PositiveBooleanCondition;
    struct NegativeBooleanCondition;
    struct GreaterNumericalCondition;
    struct EqualNumericalCondition;
    struct GreaterConceptCondition;
    struct EqualConceptCondition;
    struct PositiveBooleanEffect;
    struct NegativeBooleanEffect;
    struct UnchangedBooleanEffect;
    struct IncrementNumericalEffect;
    struct IncrementOrUnchangedNumericalEffect;
    struct DecrementNumericalEffect;
    struct DecrementOrUnchangedNumericalEffect;
    struct UnchangedNumericalEffect;
    struct GreaterNumericalEffect;
    struct EqualNumericalEffect;
    struct IncrementConceptEffect;
    struct DecrementConceptEffect;
    struct UnchangedConceptEffect;
    struct GreaterConceptEffect;
    struct EqualConceptEffect;

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
        core::ast::Concept concept_;
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
    struct PositiveBooleanCondition : x3::position_tagged {
        BooleanReference reference;
    };

    struct NegativeBooleanCondition : x3::position_tagged {
        BooleanReference reference;
    };

    struct GreaterNumericalCondition : x3::position_tagged {
        NumericalReference reference;
    };

    struct EqualNumericalCondition : x3::position_tagged {
        NumericalReference reference;
    };

    struct GreaterConceptCondition : x3::position_tagged {
        ConceptReference reference;
    };

    struct EqualConceptCondition : x3::position_tagged {
        ConceptReference reference;
    };

    struct FeatureCondition : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanCondition>,
            x3::forward_ast<NegativeBooleanCondition>,
            x3::forward_ast<GreaterNumericalCondition>,
            x3::forward_ast<EqualNumericalCondition>,
            x3::forward_ast<GreaterConceptCondition>,
            x3::forward_ast<EqualConceptCondition>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    struct PositiveBooleanEffect : x3::position_tagged {
        BooleanReference reference;
    };

    struct NegativeBooleanEffect : x3::position_tagged {
        BooleanReference reference;
    };

    struct UnchangedBooleanEffect : x3::position_tagged {
        BooleanReference reference;
    };

    struct IncrementNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct IncrementOrUnchangedNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct DecrementNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct DecrementOrUnchangedNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct UnchangedNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct GreaterNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct EqualNumericalEffect : x3::position_tagged {
        NumericalReference reference;
    };

    struct IncrementConceptEffect : x3::position_tagged {
        ConceptReference reference;
    };

    struct DecrementConceptEffect : x3::position_tagged {
        ConceptReference reference;
    };

    struct UnchangedConceptEffect : x3::position_tagged {
        ConceptReference reference;
    };

    struct GreaterConceptEffect : x3::position_tagged {
        ConceptReference reference;
    };

    struct EqualConceptEffect : x3::position_tagged {
        ConceptReference reference;
    };

    struct FeatureEffect : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanEffect>,
            x3::forward_ast<NegativeBooleanEffect>,
            x3::forward_ast<UnchangedBooleanEffect>,
            x3::forward_ast<IncrementNumericalEffect>,
            x3::forward_ast<IncrementOrUnchangedNumericalEffect>,
            x3::forward_ast<DecrementNumericalEffect>,
            x3::forward_ast<DecrementOrUnchangedNumericalEffect>,
            x3::forward_ast<UnchangedNumericalEffect>,
            x3::forward_ast<GreaterNumericalEffect>,
            x3::forward_ast<EqualNumericalEffect>,
            x3::forward_ast<IncrementConceptEffect>,
            x3::forward_ast<DecrementConceptEffect>,
            x3::forward_ast<UnchangedConceptEffect>,
            x3::forward_ast<GreaterConceptEffect>,
            x3::forward_ast<EqualConceptEffect>> {
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
        boost::optional<Booleans> booleans;
        boost::optional<Numericals> numericals;
        boost::optional<Concepts> concepts;
        boost::optional<Roles> roles;
        Rules rules;
    };
}


#endif