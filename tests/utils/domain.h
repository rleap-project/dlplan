#ifndef DLPLAN_TESTS_UTILS_DOMAIN_H_
#define DLPLAN_TESTS_UTILS_DOMAIN_H_

#include "include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;


namespace dlplan::tests {

namespace gripper {
extern std::shared_ptr<VocabularyInfo> construct_vocabulary_info();

extern std::shared_ptr<InstanceInfo> construct_instance_info(std::shared_ptr<VocabularyInfo> vocabulary_info);
}

namespace blocks_4 {
extern std::shared_ptr<VocabularyInfo> construct_vocabulary_info();
}

extern std::shared_ptr<SyntacticElementFactory> construct_syntactic_element_factory(std::shared_ptr<VocabularyInfo> vocabulary_info);

}

#endif
