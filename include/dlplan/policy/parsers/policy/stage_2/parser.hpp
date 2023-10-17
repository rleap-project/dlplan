#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/policy.h"

#include "include/dlplan/common/parsers/config.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"

#include "context.hpp"


namespace dlplan::policy::parsers::policy::stage_2::parser {

extern std::string parse(
    const stage_1::ast::Name& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const core::Boolean>> parse(
    const stage_1::ast::BooleanDefinition& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const core::Boolean> parse(
    const stage_1::ast::BooleanReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::unordered_map<std::string, std::shared_ptr<const core::Boolean>> parse(
    const stage_1::ast::BooleansEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const core::Numerical>> parse(
    const stage_1::ast::NumericalDefinition& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const core::Numerical> parse(
    const stage_1::ast::NumericalReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::unordered_map<std::string, std::shared_ptr<const core::Numerical>> parse(
    const stage_1::ast::NumericalsEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::PositiveBooleanConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::NegativeBooleanConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::GreaterNumericalConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::EqualNumericalConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::PositiveBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::NegativeBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::UnchangedBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::IncrementNumericalEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::DecrementNumericalEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::UnchangedNumericalEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::FeatureConditionEntryInner& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(
    const stage_1::ast::FeatureConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::FeatureEffectEntryInner& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(
    const stage_1::ast::FeatureEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Rule> parse(
    const stage_1::ast::RuleEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern Rules parse(
    const stage_1::ast::Rules& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Policy> parse(
    const stage_1::ast::Policy& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

}


#endif

