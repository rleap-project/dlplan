#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/core.h"
#include "include/dlplan/policy.h"

#include "include/dlplan/common/parsers/config.hpp"
#include "include/dlplan/policy/parsers/syntactic/ast.hpp"

#include "context.hpp"


namespace dlplan::policy {

extern std::string parse(const ast::Name& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::string parse(const ast::BooleanDefinition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::core::Boolean> parse(const ast::BooleanImplementation& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(const ast::Boolean& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedBoolean> parse(const ast::BooleanReference& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> parse(const ast::Booleans& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::string parse(const ast::NumericalDefinition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::core::Numerical> parse(const ast::NumericalImplementation& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(const ast::Numerical& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedNumerical> parse(const ast::NumericalReference& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> parse(const ast::Numericals& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::string parse(const ast::ConceptDefinition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::core::Concept> parse(const ast::ConceptImplementation& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(const ast::Concept& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedConcept> parse(const ast::ConceptReference& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> parse(const ast::Concepts& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::string parse(const ast::RoleDefinition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::core::Role> parse(const ast::RoleImplementation& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::pair<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(const ast::Role& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const dlplan::policy::NamedRole> parse(const ast::RoleReference& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> parse(const ast::Roles& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(const ast::PositiveBooleanConditionEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::NegativeBooleanConditionEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::GreaterNumericalConditionEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::EqualNumericalConditionEntry& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const ast::PositiveBooleanEffectEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::NegativeBooleanEffectEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::UnchangedBooleanEffectEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::IncrementNumericalEffectEntry& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::DecrementNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::UnchangedNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(const ast::FeatureCondition& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const ast::FeatureEffect& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Rule> parse(const ast::Rule& node, const dlplan::error_handler_type& error_handler, Context& context);
extern Rules parse(const ast::Rules& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Policy> parse(const ast::Policy& node, const dlplan::error_handler_type& error_handler, Context& context);

}


#endif

