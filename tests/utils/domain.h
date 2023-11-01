#ifndef DLPLAN_TESTS_UTILS_DOMAIN_H_
#define DLPLAN_TESTS_UTILS_DOMAIN_H_

#include "../../include/dlplan/policy.h"


namespace dlplan::tests {

namespace gripper {
extern std::shared_ptr<dlplan::core::VocabularyInfo> construct_vocabulary_info();

extern std::shared_ptr<dlplan::core::InstanceInfo> construct_instance_info(std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info);
}

namespace blocks_4 {
extern std::shared_ptr<dlplan::core::VocabularyInfo> construct_vocabulary_info();
}

extern std::shared_ptr<dlplan::core::SyntacticElementFactory> construct_syntactic_element_factory(std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info);

}

#endif
