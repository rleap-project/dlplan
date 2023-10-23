#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_CONTEXT_HPP_

#include <unordered_map>

#include "include/dlplan/policy.h"
#include "include/dlplan/core.h"


namespace dlplan::policy::parsers::policy::stage_2::parser {

struct Context {
    PolicyFactory& policy_factory;
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedBoolean>> booleans;
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedNumerical>> numericals;
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedConcept>> concepts;
    std::unordered_map<std::string, std::shared_ptr<const dlplan::policy::NamedRole>> roles;

    explicit Context(PolicyFactory& policy_factory);
};

}

#endif
