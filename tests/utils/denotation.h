#ifndef DLPLAN_TESTS_UTILS_DOMAIN_H_
#define DLPLAN_TESTS_UTILS_DOMAIN_H_

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests {

extern ConceptDenotation create_concept_denotation(const InstanceInfo& instance, const std::vector<std::string>& object_names);

extern RoleDenotation create_role_denotation(const InstanceInfo& instance, const std::vector<std::pair<std::string, std::string>>& object_pair_names);

}


#endif
