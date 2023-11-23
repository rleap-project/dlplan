/// Example illustrating the core component.

#include "../../include/dlplan/core.h"

#include <iostream>

using namespace dlplan::core;


/// @brief Construct a VocabularyInfo for the Blocks domain.
///
/// Create an empty VocabularyInfo and then add predicates, and constants.
///
/// @return the VocabularyInfo
static std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    vocabulary->add_predicate("on", 2);
    vocabulary->add_predicate("on_g", 2);
    vocabulary->add_predicate("ontable", 1);
    vocabulary->add_predicate("holding", 1);
    vocabulary->add_predicate("clear", 1);
    vocabulary->add_predicate("arm-empty", 0);
    return vocabulary;
}

/// @brief Construct an InstanceInfo over the Blocks domain.
///
/// Create an empty InstanceInfo and then add objects, atoms, and static atoms.
///
/// @param vocabulary the VocabularyInfo initialized for Blocks
/// @return the InstanceInfo
static std::shared_ptr<InstanceInfo> construct_instance_info(
    const std::shared_ptr<VocabularyInfo>& vocabulary) {
    // User must ensure that each InstanceInfo gets its unique index for caching.
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    instance->add_atom("on", {"a", "b"});
    instance->add_atom("on", {"b", "a"});
    instance->add_atom("ontable", {"a"});
    instance->add_atom("ontable", {"b"});
    instance->add_atom("holding", {"a"});
    instance->add_atom("holding", {"b"});
    instance->add_atom("clear", {"a"});
    instance->add_atom("clear", {"b"});
    instance->add_atom("arm-empty", {});
    instance->add_static_atom("on_g", {"a", "b"});
    return instance;
}


/// @brief Example illustrating the core component on a fragment of a planning
///        problem over the Blocks domain.
///
/// The example illustrates the creation of VocabularyInfo, InstanceInfo,
/// State, Element, and the evaluation of Element on a State using
/// DenotationsCaches.
int main() {
    auto vocabulary = construct_vocabulary_info();
    auto instance = construct_instance_info(vocabulary);
    SyntacticElementFactory factory(vocabulary);

    const auto& atoms = instance->get_atoms();
    const auto& atom_0 = atoms[0];
    const auto& atom_1 = atoms[1];
    const auto& atom_3 = atoms[3];
    const auto& atom_6 = atoms[6];
    // User must ensure that each State gets its unique index for caching.
    State state_0(0, instance, {atom_0, atom_3, atom_6});  // a on b
    State state_1(1, instance, {atom_1, atom_3, atom_6});  // b on a
    States states{state_0, state_1};

    auto numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "Element repr: " << numerical->str() << std::endl << std::endl;

    std::cout << "Evaluate for single state without cache." << std::endl;
    std::cout << "State: " << state_0.str() << std::endl;
    std::cout << "Value: " << numerical->evaluate(state_0) << std::endl << std::endl;

    DenotationsCaches caches;
    std::cout << "Evaluate for single state with cache." << std::endl;
    std::cout << "State: " << state_1.str() << std::endl;
    std::cout << "Value: " << numerical->evaluate(state_1, caches) << std::endl << std::endl;

    std::cout << "Evaluate for multiple states with cache." << std::endl;
    auto denotations = numerical->evaluate(states, caches);
    for (size_t i = 0; i < states.size(); ++i) {
        std::cout << "State: " << states[i].str() << std::endl;
        std::cout << "Value: " << (*denotations)[i] << std::endl;
    }
    return 0;
}
