#include "../../include/dlp/generator.h"

#include <memory>

int main() {
    // Factory initialization
    dlp::core::VocabularyInfo vocabulary;
    dlp::core::Predicate p0 = vocabulary.add_predicate("on", 2);
    dlp::core::Predicate p1 = vocabulary.add_predicate("onTable", 1);
    dlp::core::Predicate p2 = vocabulary.add_predicate("holding", 1);
    dlp::core::Predicate p3 = vocabulary.add_predicate("on_g", 2);
    dlp::core::InstanceInfo instance = vocabulary.make_instance();
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"});
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"});
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"});
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"});
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"});
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"});
    dlp::core::Atom a6 = instance.add_static_atom("on_g", {"A", "B"});
    std::shared_ptr<dlp::core::SyntacticElementFactory> factory = std::make_shared<dlp::core::SyntacticElementFactory>(vocabulary.make_factory());

    // Generate a bunch of states
    dlp::core::State s0 = instance.convert_state({a0, a3});
    dlp::core::State s1 = instance.convert_state({a1, a2});
    dlp::core::State s2 = instance.convert_state({a2, a3});
    dlp::generator::States states({s0, s1, s2});

    dlp::generator::FeatureGenerator generator(factory, 2, 180);
    dlp::generator::FeatureCollection features = generator.generate(states);
    dlp::generator::FeatureCollectionWriter().write(features, "test.txt");
}
