#include "../include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;


extern std::shared_ptr<const VocabularyInfo> construct_vocabulary_info();

extern std::shared_ptr<const VocabularyInfo> construct_blocks_vocabulary_info();

extern std::shared_ptr<const InstanceInfo> construct_instance_info(std::shared_ptr<const VocabularyInfo> vocabulary_info);

extern SyntacticElementFactory construct_syntactic_element_factory(std::shared_ptr<const VocabularyInfo> vocabulary_info);
