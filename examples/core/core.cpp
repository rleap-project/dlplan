#include "../../include/dlplan/core.h"

#include <iostream>

using namespace dlplan::core;

static std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    // Add predicates and constants of the domain.
    // Note that there are no constants in Blocksworld.
    vocabulary->add_predicate("on", 2);
    vocabulary->add_predicate("on_g", 2);
    vocabulary->add_predicate("ontable", 1);
    vocabulary->add_predicate("holding", 1);
    vocabulary->add_predicate("clear", 1);
    vocabulary->add_predicate("arm-empty", 0);
    return vocabulary;
}

static std::shared_ptr<InstanceInfo> construct_instance_info(const std::shared_ptr<VocabularyInfo>& vocabulary) {
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add dynamic atoms
    instance->add_atom("on", {"a", "b"});
    instance->add_atom("on", {"b", "a"});
    instance->add_atom("ontable", {"a"});
    instance->add_atom("ontable", {"b"});
    instance->add_atom("holding", {"a"});
    instance->add_atom("holding", {"b"});
    instance->add_atom("clear", {"a"});
    instance->add_atom("clear", {"b"});
    instance->add_atom("arm-empty", {});
    // Add static goal atoms
    instance->add_static_atom("on_g", {"a", "b"});
    // Add static atoms
    // Note that there are no static atoms in Blocksworld.
    return instance;
}


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 1. Initialize VocabularyInfo
    auto vocabulary = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto instance = construct_instance_info(vocabulary);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory factory(vocabulary);

    // 4. Construct a state.
    const auto& atoms = instance->get_atoms();
    const Atom& atom_0 = atoms[0];
    const Atom& atom_3 = atoms[3];
    const Atom& atom_6 = atoms[6];
    State state(instance, {atom_0, atom_3, atom_6});

    // 5. Parse and evaluate elements.
    std::shared_ptr<const Numerical> numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << numerical->compute_repr() << std::endl;
    std::cout << "value: " << numerical->evaluate(state) << std::endl;

    std::shared_ptr<const Boolean> boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << boolean->compute_repr() << std::endl;
    std::cout << "value: " << boolean->evaluate(state) << std::endl;

    return 0;
}
