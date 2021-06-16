#include "../../include/dlplan/core.h"

#include <iostream>


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 0. Initialize the vocabulary
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary = std::make_shared<dlplan::core::VocabularyInfo>();
    dlplan::core::Predicate p0 = vocabulary->add_predicate("on", 2);
    dlplan::core::Predicate p1 = vocabulary->add_predicate("on_g", 2);
    dlplan::core::Predicate p2 = vocabulary->add_predicate("ontable", 1);
    dlplan::core::Predicate p3 = vocabulary->add_predicate("holding", 1);
    dlplan::core::Predicate p4 = vocabulary->add_predicate("clear", 1);
    // 1. Initialize planning instance
    std::shared_ptr<dlplan::core::InstanceInfo> instance = std::make_shared<dlplan::core::InstanceInfo>(vocabulary);
    // Add state atoms

    dlplan::core::Atom a0 = instance->add_atom("on", {"a", "b"});
    dlplan::core::Atom a1 = instance->add_atom("on", {"b", "a"});
    dlplan::core::Atom a2 = instance->add_atom("ontable", {"a"});
    dlplan::core::Atom a3 = instance->add_atom("ontable", {"b"});
    dlplan::core::Atom a4 = instance->add_atom("holding", {"a"});
    dlplan::core::Atom a5 = instance->add_atom("holding", {"b"});
    dlplan::core::Atom a6 = instance->add_atom("clear", {"a"});
    dlplan::core::Atom a7 = instance->add_atom("clear", {"b"});
    // Add goal atoms
    dlplan::core::Atom a8 = instance->add_static_atom("on_g", {"a", "b"});

    // 2. Initialize factory.
    dlplan::core::SyntacticElementFactory factory(vocabulary);

    // 3. Construct and evaluate elements.
    dlplan::core::State state(instance, {a0, a3, a6});
    // Parse text to Numerical
    dlplan::core::Numerical numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    std::cout << "repr: " << numerical.compute_repr() << std::endl;
    int value = numerical.evaluate(state);
    std::cout << "value: " << value << std::endl;

    dlplan::core::Numerical numerical2 = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    std::cout << "repr: " << numerical2.compute_repr() << std::endl;
    value = numerical2.evaluate(state);
    std::cout << "value: " << value << std::endl;

    /*dlplan::core::Boolean numerical5 = factory.parse_boolean("b_empty(c_and(on_g(0),on(0)))");
    std::cout << "repr: " << numerical5.compute_repr() << std::endl;
    value = numerical5.evaluate(state);
    std::cout << "value: " << value << std::endl;

    dlplan::core::Numerical numerical3 = numerical2;
    std::cout << "repr: " << numerical3.compute_repr() << std::endl;
    value = numerical3.evaluate(state);
    std::cout << "value: " << value << std::endl;

    dlplan::core::Boolean numerical4 = numerical5;
    std::cout << "repr: " << numerical4.compute_repr() << std::endl;
    value = numerical4.evaluate(state);
    std::cout << "value: " << value << std::endl;*/
}
