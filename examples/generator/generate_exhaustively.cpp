#include "../../include/dlplan/generator.h"

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary = std::make_shared<dlplan::core::VocabularyInfo>();
    dlplan::core::Predicate p0 = vocabulary->add_predicate("on", 2);
    dlplan::core::Predicate p1 = vocabulary->add_predicate("ontable", 1);
    dlplan::core::Predicate p2 = vocabulary->add_predicate("holding", 1);
    dlplan::core::Predicate p3 = vocabulary->add_predicate("clear", 1);
    dlplan::core::Predicate p4 = vocabulary->add_predicate("on_g", 2);
    std::shared_ptr<dlplan::core::InstanceInfo> instance = std::make_shared<dlplan::core::InstanceInfo>(vocabulary);
    dlplan::core::Atom a0 = instance->add_atom("on", {"A", "B"});
    dlplan::core::Atom a1 = instance->add_atom("on", {"B", "A"});
    dlplan::core::Atom a2 = instance->add_atom("ontable", {"A"});
    dlplan::core::Atom a3 = instance->add_atom("ontable", {"B"});
    dlplan::core::Atom a4 = instance->add_atom("holding", {"A"});
    dlplan::core::Atom a5 = instance->add_atom("holding", {"B"});
    dlplan::core::Atom a6 = instance->add_atom("clear", {"A"});
    dlplan::core::Atom a7 = instance->add_atom("clear", {"B"});
    dlplan::core::Atom a8 = instance->add_static_atom("on_g", {"A", "B"});
    std::shared_ptr<dlplan::core::SyntacticElementFactory> factory = std::make_shared<dlplan::core::SyntacticElementFactory>(vocabulary);

    // Generate a bunch of states
    dlplan::core::State s0(instance, {a0, a3});
    dlplan::core::State s1(instance, {a1, a2});
    dlplan::core::State s2(instance, {a2, a3});
    dlplan::generator::States states({s0, s1, s2});

    dlplan::generator::FeatureGenerator generator(factory, 10, 180);
    dlplan::generator::FeatureCollection features = generator.generate(states);
}
