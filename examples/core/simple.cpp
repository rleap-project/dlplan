#include "../../include/dlp/core.h"

#include <iostream>


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 0. Initialize the vocabulary
    dlp::core::VocabularyInfo vocabulary;
    dlp::core::Predicate p0 = vocabulary.add_predicate("on", 2);
    dlp::core::Predicate p1 = vocabulary.add_predicate("onTable", 1);
    dlp::core::Predicate p2 = vocabulary.add_predicate("holding", 1);
    dlp::core::Predicate p3 = vocabulary.add_predicate("on_g", 2);
    std::vector<dlp::core::Predicate> predicates({p0, p1, p2, p3});
    // 1. Initialize planning instance
    dlp::core::InstanceInfo instance(vocabulary);
    // Add state atoms

    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"});
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"});
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"});
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"});
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"});
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"});
    // Add goal atoms
    dlp::core::Atom a6 = instance.add_static_atom("on_g", {"A", "B"});

    // 2. Initialize factory.
    dlp::core::SyntacticElementFactory factory(vocabulary);

    // 3. Construct and evaluate elements.
    // Parse text to Numerical
    dlp::core::Numerical numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    std::cout << "complexity: " << numerical.compute_complexity() << std::endl;
    // Parse text to Concept: returns the Concept that is already cached
    dlp::core::Concept concept = factory.parse_concept("c_and(on(0),on_g(0))");

    // 4. Evaluate for example state: block A on B
    dlp::core::State state = instance.convert_state({a0, a3});

    int value = numerical.evaluate(state);
    std::cout << "repr: " << numerical.compute_repr() << std::endl;
    std::cout << "value: " << value << std::endl;

    dlp::core::Numerical numerical2 = factory.parse_numerical("n_count(r_diff(on(0,1), on_g(1,0)))");
    value = numerical2.evaluate(state);
    std::cout << "repr: " << numerical2.compute_repr() << std::endl;
    std::cout << "value: " << value << std::endl;
};
