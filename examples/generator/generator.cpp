/// Example illustrating the generator component.


#include "../../include/dlplan/generator.h"

#include <iostream>

using namespace dlplan::core;
using namespace dlplan::generator;


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
    auto instance = std::make_shared<InstanceInfo>(vocabulary, 0);
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


/// @brief Example illustrating the generator component on a fragment of a planning
///        problem over the Blocks domain.
int main() {
    auto vocabulary = construct_vocabulary_info();
    auto instance = construct_instance_info(vocabulary);
    SyntacticElementFactory factory(vocabulary);

    const auto& atoms = instance->get_atoms();
    const Atom& atom_0 = atoms[0];
    const Atom& atom_1 = atoms[1];
    const Atom& atom_2 = atoms[2];
    const Atom& atom_3 = atoms[3];
    const Atom& atom_4 = atoms[4];
    const Atom& atom_5 = atoms[5];
    const Atom& atom_6 = atoms[6];
    const Atom& atom_7 = atoms[7];
    const Atom& atom_8 = atoms[8];
    // User must ensure that each State gets its unique index for caching.
    State state_0(instance, {atom_0, atom_3, atom_6, atom_8}, 0);  // a on b
    State state_1(instance, {atom_1, atom_2, atom_7, atom_8}, 1);  // b on a
    State state_2(instance, {atom_2, atom_3, atom_6, atom_7, atom_8}, 2);  // a,b on table
    State state_3(instance, {atom_3, atom_4, atom_7}, 3);  // holding a, b on table
    State state_4(instance, {atom_2, atom_5, atom_6}, 4);  // holding b, a on table
    States states({state_0, state_1, state_2, state_3, state_4});

    FeatureRepresentations features = generate_features(factory, states, 5, 5, 10, 10, 10, 180, 100000);

    for (const auto& feature : features) {
        std::cout << feature << std::endl;
    }
}
