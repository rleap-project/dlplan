#include "../../include/dlplan/core.h"

#include <iostream>

using namespace dlplan::core;

static std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> v = std::make_shared<VocabularyInfo>();
    // Add predicates and constants of the domain.
    // Note that there are no constants in Blocksworld.
    v->add_predicate("on", 2);
    v->add_predicate("on_g", 2);
    v->add_predicate("ontable", 1);
    v->add_predicate("holding", 1);
    v->add_predicate("clear", 1);
    v->add_predicate("arm-empty", 0);
    return v;
}

static std::shared_ptr<InstanceInfo> construct_instance_info(std::shared_ptr<VocabularyInfo> v) {
    std::shared_ptr<InstanceInfo> i = std::make_shared<InstanceInfo>(v);
    // Add dynamic atoms
    i->add_atom("on", {"a", "b"});
    i->add_atom("on", {"b", "a"});
    i->add_atom("ontable", {"a"});
    i->add_atom("ontable", {"b"});
    i->add_atom("holding", {"a"});
    i->add_atom("holding", {"b"});
    i->add_atom("clear", {"a"});
    i->add_atom("clear", {"b"});
    i->add_atom("arm-empty", {});
    // Add static goal atoms
    i->add_static_atom("on_g", {"a", "b"});
    // Add static atoms
    // Note that there are no static atoms in Blocksworld.
    return i;
}


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 1. Initialize VocabularyInfo
    auto v = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto i = construct_instance_info(v);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory f(v);

    // 4. Construct a state.
    const auto& atoms = i->get_atoms();
    const Atom& a0 = atoms[0];
    const Atom& a3 = atoms[3];
    const Atom& a6 = atoms[6];
    State state(i, {a0, a3, a6});

    // 5. Parse and evaluate elements.
    Numerical numerical = f.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << numerical.compute_repr() << std::endl;
    std::cout << "value: " << numerical.evaluate(state) << std::endl;

    Boolean boolean = f.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << boolean.compute_repr() << std::endl;
    std::cout << "value: " << boolean.evaluate(state) << std::endl;

    return 0;
}
