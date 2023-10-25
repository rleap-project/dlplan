#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_AST_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_1_AST_HPP_

#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "include/dlplan/core/parsers/elements/stage_1/ast.hpp"


namespace dlplan::policy::parsers::policy::stage_1::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct NameInner;
    struct Name;
    struct NameEntry;
    struct Boolean;
    struct BooleanReference;
    struct BooleansEntry;
    struct Numerical;
    struct NumericalReference;
    struct NumericalsEntry;
    struct PositiveBooleanConditionEntry;
    struct NegativeBooleanConditionEntry;
    struct GreaterNumericalConditionEntry;
    struct EqualNumericalConditionEntry;
    struct PositiveBooleanEffectEntry;
    struct NegativeBooleanEffectEntry;
    struct UnchangedBooleanEffectEntry;
    struct IncrementNumericalEffectEntry;
    struct DecrementNumericalEffect;
    struct UnchangedNumericalEffect;
    struct FeatureConditionEntryInner;
    struct FeatureCondition;
    struct FeatureEffectEntryInner;
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
    struct Boolean : x3::position_tagged {
        Name key;
        core::parsers::elements::stage_1::ast::Boolean boolean;
    };

    struct BooleanReference : x3::position_tagged {
        Name key;
    };

    struct BooleansEntry : x3::position_tagged {
        std::vector<Boolean> definitions;
    };


    /* Numerical entry and references */
    struct Numerical : x3::position_tagged {
        Name key;
        core::parsers::elements::stage_1::ast::Numerical numerical;
    };

    struct NumericalReference : x3::position_tagged {
        Name key;
    };

    struct NumericalsEntry : x3::position_tagged {
        std::vector<Numerical> definitions;
    };


    /* Concept entry and references */
    struct Concept : x3::position_tagged {
        Name key;
        core::parsers::elements::stage_1::ast::Concept concept;
    };

    struct ConceptReference : x3::position_tagged {
        Name key;
    };

    struct Concepts : x3::position_tagged {
        std::vector<Concept> definitions;
    };


    /* Role entry and references */
    struct RoleVariant : x3::position_tagged {
        Name key;
        core::parsers::elements::stage_1::ast::Role role;
    };

    struct RoleReference : x3::position_tagged {
        Name key;
    };

    struct Roles : x3::position_tagged {
        std::vector<RoleVariant> definitions;
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
        BooleansEntry booleans;
        NumericalsEntry numericals;
        Rules rules;
    };
}


#endif