#include "domain.h"

using namespace dlplan::core;


namespace dlplan::tests {
namespace gripper {
std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    vocabulary->add_predicate("at", 2);
    vocabulary->add_predicate("at_g", 2);
    vocabulary->add_predicate("at_roboter", 1);
    vocabulary->add_predicate("package", 1);
    vocabulary->add_predicate("holding", 1);
    return vocabulary;
}

std::shared_ptr<InstanceInfo> construct_instance_info(std::shared_ptr<VocabularyInfo> vocabulary) {
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    instance->add_atom("at", {"p1", "A"});
    instance->add_atom("at", {"p1", "B"});
    instance->add_atom("at", {"p2", "A"});
    instance->add_atom("at", {"p2", "B"});
    instance->add_atom("at", {"p3", "A"});
    instance->add_atom("at", {"p3", "B"});
    instance->add_atom("at_roboter", {"A"});
    instance->add_atom("at_roboter", {"B"});
    instance->add_atom("holding", {"p1"});
    instance->add_atom("holding", {"p2"});
    instance->add_atom("holding", {"p3"});
    instance->add_static_atom("package", {"p1"});
    instance->add_static_atom("package", {"p2"});
    instance->add_static_atom("package", {"p3"});
    instance->add_static_atom("at_g", {"p1", "B"});
    instance->add_static_atom("at_g", {"p2", "B"});
    instance->add_static_atom("at_g", {"p3", "B"});
    return instance;
}

}

namespace blocks_4 {
std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    vocabulary->add_predicate("on", 2);
    vocabulary->add_predicate("on_g", 2);
    vocabulary->add_predicate("ontable", 1);
    vocabulary->add_predicate("holding", 1);
    vocabulary->add_predicate("clear", 1);
    vocabulary->add_predicate("arm-empty", 0);
    return vocabulary;
}
}

std::shared_ptr<SyntacticElementFactory> construct_syntactic_element_factory(std::shared_ptr<VocabularyInfo> vocabulary) {
    return std::make_shared<SyntacticElementFactory>(vocabulary);
}

}

