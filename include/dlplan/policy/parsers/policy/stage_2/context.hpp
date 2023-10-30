#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_

#include <unordered_map>

#include "include/dlplan/policy.h"
#include "include/dlplan/core.h"

#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"


namespace dlplan::policy::parsers::policy::stage_2::parser {

template<typename Node, typename Result>
struct Data {
    Node node;
    Result result;
};

using NamedBooleanData = Data<policy::stage_1::ast::Boolean, std::shared_ptr<const dlplan::policy::NamedBoolean>>;
using NamedNumericalData = Data<policy::stage_1::ast::Numerical, std::shared_ptr<const dlplan::policy::NamedNumerical>>;
using NamedConceptData = Data<policy::stage_1::ast::Concept, std::shared_ptr<const dlplan::policy::NamedConcept>>;
using NamedRoleData = Data<policy::stage_1::ast::Role, std::shared_ptr<const dlplan::policy::NamedRole>>;


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
