#include "../../include/dlp/core.h"

#include <iostream>


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 0. Initialize the vocabulary
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary = std::make_shared<dlplan::core::VocabularyInfo>();
    dlplan::core::Predicate p0 = vocabulary->add_predicate("on", 2);
    dlplan::core::Predicate p1 = vocabulary->add_predicate("onTable", 1);
    dlplan::core::Predicate p2 = vocabulary->add_predicate("holding", 1);
    dlplan::core::Predicate p3 = vocabulary->add_predicate("on_g", 2);
    // 1. Initialize planning instance
    std::shared_ptr<dlplan::core::InstanceInfo> instance = std::make_shared<dlplan::core::InstanceInfo>(vocabulary);
    // Add state atoms

    dlplan::core::Atom a0 = instance->add_atom("on", {"A", "B"});
    dlplan::core::Atom a1 = instance->add_atom("on", {"B", "A"});
    dlplan::core::Atom a2 = instance->add_atom("onTable", {"A"});
    dlplan::core::Atom a3 = instance->add_atom("onTable", {"B"});
    dlplan::core::Atom a4 = instance->add_atom("holding", {"A"});
    dlplan::core::Atom a5 = instance->add_atom("holding", {"B"});
    // Add goal atoms
    dlplan::core::Atom a6 = instance->add_static_atom("on_g", {"A", "B"});

    // 2. Initialize factory.
    dlplan::core::SyntacticElementFactory factory(vocabulary);

    // 3. Construct and evaluate elements.
    // Parse text to Numerical
    dlplan::core::Numerical numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    std::cout << "complexity: " << numerical.compute_complexity() << std::endl;
    // Parse text to Concept: returns the Concept that is already cached
    dlplan::core::Concept concept = factory.parse_concept("c_and(on(0),on_g(0))");

    // 4. Evaluate for example state: block A on B
    dlplan::core::State state(instance, {a0, a3});

    std::cout << "repr: " << numerical.compute_repr() << std::endl;
    int value = numerical.evaluate(state);
    std::cout << "value: " << value << std::endl;
    dlplan::core::Numerical numerical2 = factory.parse_numerical("n_count(r_transitive_reflexive_closure(r_transitive_reflexive_closure(r_top)))");
    std::cout << "repr: " << numerical2.compute_repr() << std::endl;
    value = numerical2.evaluate(state);
    std::cout << "value: " << value << std::endl;

    dlplan::core::Numerical numerical3 = factory.parse_numerical("n_count(r_transitive_reflexive_closure(r_top))");
    std::cout << "repr: " << numerical3.compute_repr() << std::endl;
    value = numerical3.evaluate(state);
    std::cout << "value: " << value << std::endl;

    dlplan::core::Numerical numerical4 = factory.parse_numerical("n_count(r_top)");
    std::cout << "repr: " << numerical4.compute_repr() << std::endl;
    value = numerical4.evaluate(state);
    std::cout << "value: " << value << std::endl;
}
