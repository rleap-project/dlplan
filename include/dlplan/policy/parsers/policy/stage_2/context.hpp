#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_

#include <unordered_map>

#include "include/dlplan/policy.h"
#include "include/dlplan/core.h"


namespace dlplan::policy::parsers::policy::stage_2::parser {

struct Context {
    PolicyFactory& policy_factory;
    std::unordered_map<std::string, std::shared_ptr<const core::Boolean>> booleans;
    std::unordered_map<std::string, std::shared_ptr<const core::Numerical>> numericals;

    explicit Context(PolicyFactory& policy_factory);
};

}

#endif 
