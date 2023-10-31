#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_CONTEXT_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_SEMANTIC_CONTEXT_HPP_

#include <unordered_map>

#include "include/dlplan/policy.h"
#include "include/dlplan/core.h"

#include "include/dlplan/policy/parsers/syntactic/ast.hpp"


namespace dlplan::policy {

template<typename Node, typename Result>
struct Data {
    Node node;
    Result result;
};

using NamedBooleanData = Data<ast::Boolean, std::shared_ptr<const NamedBoolean>>;
using NamedNumericalData = Data<ast::Numerical, std::shared_ptr<const NamedNumerical>>;
using NamedConceptData = Data<ast::Concept, std::shared_ptr<const NamedConcept>>;
using NamedRoleData = Data<ast::Role, std::shared_ptr<const NamedRole>>;


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
