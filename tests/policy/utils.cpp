#include "utils.h"

std::shared_ptr<const VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    vocabulary_info->add_predicate("at", 2);
    vocabulary_info->add_predicate("at_g", 2);
    vocabulary_info->add_predicate("at_roboter", 1);
    vocabulary_info->add_predicate("package", 1);
    vocabulary_info->add_predicate("holding", 1);
    return vocabulary_info;
}

std::shared_ptr<const VocabularyInfo> construct_blocks_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    vocabulary_info->add_predicate("on", 2);
    vocabulary_info->add_predicate("on_g", 2);
    vocabulary_info->add_predicate("clear", 1);
    return vocabulary_info;
}

std::shared_ptr<const InstanceInfo> construct_instance_info(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    std::shared_ptr<InstanceInfo> instance_info = std::make_shared<InstanceInfo>(vocabulary_info);
    instance_info->add_atom("at", {"p1", "A"});
    instance_info->add_atom("at", {"p1", "B"});
    instance_info->add_atom("at", {"p2", "A"});
    instance_info->add_atom("at", {"p2", "B"});
    instance_info->add_atom("at", {"p3", "A"});
    instance_info->add_atom("at", {"p3", "B"});
    instance_info->add_atom("at_roboter", {"A"});
    instance_info->add_atom("at_roboter", {"B"});
    instance_info->add_atom("holding", {"p1"});
    instance_info->add_atom("holding", {"p2"});
    instance_info->add_atom("holding", {"p3"});
    instance_info->add_static_atom("package", {"p1"});
    instance_info->add_static_atom("package", {"p2"});
    instance_info->add_static_atom("package", {"p3"});
    instance_info->add_static_atom("at_g", {"p1", "B"});
    instance_info->add_static_atom("at_g", {"p2", "B"});
    instance_info->add_static_atom("at_g", {"p3", "B"});
    return instance_info;
}

SyntacticElementFactory construct_syntactic_element_factory(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    return SyntacticElementFactory(vocabulary_info);
}
