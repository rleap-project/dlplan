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

    struct Name;
    struct NameEntry;
    struct BooleanDefinition;
    struct BooleanReference;
    struct BooleansEntry;
    struct NumericalDefinition;
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
    struct DecrementNumericalEffectEntry;
    struct UnchangedNumericalEffectEntry;
    struct FeatureConditionEntryInner;
    struct FeatureConditionEntry;
    struct FeatureEffectEntryInner;
    struct FeatureEffectEntry;
    struct RuleEntry;
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
        core::parsers::elements::stage_1::ast::Boolean boolean;
    };

    struct BooleanReference : x3::position_tagged {
        Name key;
    };

    struct BooleansEntry : x3::position_tagged {
        std::vector<BooleanDefinition> definitions;
    };


    /* Numerical entry and references */
    struct NumericalDefinition : x3::position_tagged {
        Name key;
        core::parsers::elements::stage_1::ast::Numerical numerical;
    };

    struct NumericalReference : x3::position_tagged {
        Name key;
    };

    struct NumericalsEntry : x3::position_tagged {
        std::vector<NumericalDefinition> definitions;
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

    struct FeatureConditionEntryInner : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanConditionEntry>,
            x3::forward_ast<NegativeBooleanConditionEntry>,
            x3::forward_ast<GreaterNumericalConditionEntry>,
            x3::forward_ast<EqualNumericalConditionEntry>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct FeatureConditionEntry : x3::position_tagged {
        FeatureConditionEntryInner condition;
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

    struct DecrementNumericalEffectEntry : x3::position_tagged {
        NumericalReference reference;
    };

    struct UnchangedNumericalEffectEntry : x3::position_tagged {
        NumericalReference reference;
    };

    struct FeatureEffectEntryInner : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PositiveBooleanEffectEntry>,
            x3::forward_ast<NegativeBooleanEffectEntry>,
            x3::forward_ast<UnchangedBooleanEffectEntry>,
            x3::forward_ast<IncrementNumericalEffectEntry>,
            x3::forward_ast<DecrementNumericalEffectEntry>,
            x3::forward_ast<UnchangedNumericalEffectEntry>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct FeatureEffectEntry : x3::position_tagged {
        FeatureEffectEntryInner effect;
    };

    struct RuleEntry : x3::position_tagged {
        std::vector<FeatureConditionEntry> feature_conditions;
        std::vector<FeatureEffectEntry> feature_effects;
    };

    struct Rules : x3::position_tagged {
        std::vector<RuleEntry> rules;
    };

    /* Policy */
    struct Policy : x3::position_tagged {
        BooleansEntry booleans;
        NumericalsEntry numericals;
        Rules rules;
    };
}


#endif