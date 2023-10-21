#ifndef DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_AST_ADAPTED_HPP_
#define DLPLAN_SRC_POLICY_PARSERS_POLICY_STAGE_1_AST_ADAPTED_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"


// We need to tell fusion about our ast nodes
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NameInner, alphabetical, suffix)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::Name, name)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::BooleanDefinition, key, boolean)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::BooleanReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::BooleansEntry, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NumericalDefinition, key, numerical)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NumericalReference, key)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NumericalsEntry, definitions)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::PositiveBooleanConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NegativeBooleanConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::GreaterNumericalConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::EqualNumericalConditionEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::FeatureConditionEntry, condition)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::PositiveBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::NegativeBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::UnchangedBooleanEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::IncrementNumericalEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::DecrementNumericalEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::UnchangedNumericalEffectEntry, reference)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::FeatureEffectEntry, effect)

BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::RuleEntry, feature_conditions, feature_effects)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::Rules, rules)
BOOST_FUSION_ADAPT_STRUCT(dlplan::policy::parsers::policy::stage_1::ast::Policy, booleans, numericals, rules)

#endif
