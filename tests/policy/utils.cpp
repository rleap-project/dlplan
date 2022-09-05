#include "utils.h"

std::shared_ptr<const VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    vocabulary_info->add_predicate("at", 2);
    vocabulary_info->add_predicate("package", 1);
    return vocabulary_info;
}

std::shared_ptr<const InstanceInfo> construct_instance_info(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    std::shared_ptr<InstanceInfo> instance_info = std::make_shared<InstanceInfo>(vocabulary_info);
    instance_info->add_atom("at", {"p1", "A"});
    instance_info->add_atom("at", {"p1", "B"});
    instance_info->add_atom("at", {"p2", "A"});
    instance_info->add_atom("at", {"p1", "B"});
    instance_info->add_atom("at", {"p3", "A"});
    instance_info->add_atom("at", {"p1", "B"});
    instance_info->add_static_atom("package", {"p1"});
    instance_info->add_static_atom("package", {"p2"});
    instance_info->add_static_atom("package", {"p3"});
    return instance_info;
}

SyntacticElementFactory construct_syntactic_element_factory(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    return SyntacticElementFactory(vocabulary_info);
}