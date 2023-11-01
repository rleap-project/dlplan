#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_CONTEXT_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_CONTEXT_HPP_

#include "../../../policy/parsers/syntactic/ast.hpp"
#include "../../../policy.h"
#include "../../../core.h"

#include <unordered_map>




namespace dlplan::policy {

template<typename Node, typename Result>
struct Data {
    Node node;
    Result result;
};

using NamedBooleanData = Data<ast::BooleanDefinition, std::shared_ptr<const NamedBoolean>>;
using NamedNumericalData = Data<ast::NumericalDefinition, std::shared_ptr<const NamedNumerical>>;
using NamedConceptData = Data<ast::ConceptDefinition, std::shared_ptr<const NamedConcept>>;
using NamedRoleData = Data<ast::RoleDefinition, std::shared_ptr<const NamedRole>>;


struct Context {
    PolicyFactory& policy_factory;
    std::unordered_map<std::string, NamedBooleanData> booleans;
    std::unordered_map<std::string, NamedNumericalData> numericals;
    std::unordered_map<std::string, NamedConceptData> concepts;
    std::unordered_map<std::string, NamedRoleData> roles;

    explicit Context(PolicyFactory& policy_factory);
};

}

#endif
