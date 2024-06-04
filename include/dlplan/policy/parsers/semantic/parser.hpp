#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_PARSER_HPP_

#include "context.hpp"

#include "../../../core.h"
#include "../../../policy.h"

#include "../../../common/parsers/config.hpp"
#include "../../../policy/parsers/syntactic/ast.hpp"

#include <tuple>
#include <variant>


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

extern std::shared_ptr<const BaseCondition> parse(const ast::PositiveBooleanCondition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::NegativeBooleanCondition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::GreaterNumericalCondition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::EqualNumericalCondition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::GreaterConceptCondition& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseCondition> parse(const ast::EqualConceptCondition& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const ast::PositiveBooleanEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::NegativeBooleanEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::UnchangedBooleanEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::IncrementNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::IncrementOrUnchangedNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::DecrementNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::DecrementOrUnchangedNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::UnchangedNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::GreaterNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::EqualNumericalEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::IncrementConceptEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::DecrementConceptEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::UnchangedConceptEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::GreaterConceptEffect& node, const dlplan::error_handler_type& error_handler, Context& context);
extern std::shared_ptr<const BaseEffect> parse(const ast::EqualConceptEffect& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseCondition> parse(const ast::FeatureCondition& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const BaseEffect> parse(const ast::FeatureEffect& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Rule> parse(const ast::Rule& node, const dlplan::error_handler_type& error_handler, Context& context);
extern Rules parse(const ast::Rules& node, const dlplan::error_handler_type& error_handler, Context& context);

extern std::shared_ptr<const Policy> parse(const ast::Policy& node, const dlplan::error_handler_type& error_handler, Context& context);

}


#endif

