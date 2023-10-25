#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/policy.h"

#include "include/dlplan/common/parsers/config.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"

#include "context.hpp"


namespace dlplan::policy::parsers::policy::stage_2::parser {

extern std::string parse(const stage_1::ast::Name& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(const stage_1::ast::Boolean& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedBoolean> parse(const stage_1::ast::BooleanReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(const stage_1::ast::Booleans& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(const stage_1::ast::Numerical& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedNumerical> parse(const stage_1::ast::NumericalReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(const stage_1::ast::Numericals& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(const stage_1::ast::Concept& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedConcept> parse(const stage_1::ast::ConceptReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(const stage_1::ast::Concepts& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(const stage_1::ast::Role& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedRole> parse(const stage_1::ast::RoleReference& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(const stage_1::ast::Roles& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(const stage_1::ast::PositiveBooleanConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const stage_1::ast::NegativeBooleanConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const stage_1::ast::GreaterNumericalConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const stage_1::ast::EqualNumericalConditionEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::PositiveBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::NegativeBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::UnchangedBooleanEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::IncrementNumericalEffectEntry& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::DecrementNumericalEffect& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::UnchangedNumericalEffect& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(const stage_1::ast::FeatureCondition& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const stage_1::ast::FeatureEffect& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Rule> parse(const stage_1::ast::Rule& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);
extern Rules parse(const stage_1::ast::Rules& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Policy> parse(const stage_1::ast::Policy& node, const dlplan::common::parsers::error_handler_type& error_handler, Context& context);

}


#endif

