#include "../../include/dlplan/generator.h"

#include <iostream>

using namespace dlplan::core;
using namespace dlplan::generator;

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


int main() {
    // 1. Initialize VocabularyInfo
    auto vocabulary = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto instance = construct_instance_info(vocabulary);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory factory(vocabulary);

    // 4. Construct a bunch of states
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
    State state_1(instance, {atom_0, atom_3, atom_6, atom_8});
    State state_2(instance, {atom_1, atom_2, atom_7, atom_8});
    State state_3(instance, {atom_2, atom_3, atom_6, atom_7, atom_8});
    State state_4(instance, {atom_3, atom_4, atom_7});
    State state_5(instance, {atom_2, atom_5, atom_6});
    States states({state_1, state_2, state_3, state_4, state_5});

    // 5. Generate features up to complexity 4 with at most 180 seconds and at most 100000 features in total
    FeatureGenerator generator;
    generator.set_generate_inclusion_boolean(false);
    generator.set_generate_diff_concept(false);
    generator.set_generate_or_concept(false);
    generator.set_generate_subset_concept(false);
    generator.set_generate_and_role(false);
    generator.set_generate_compose_role(false);
    generator.set_generate_diff_role(false);
    generator.set_generate_identity_role(false);
    generator.set_generate_not_role(false);
    generator.set_generate_or_role(false);
    generator.set_generate_top_role(false);
    generator.set_generate_transitive_reflexive_closure_role(false);
    FeatureRepresentations features = generator.generate(factory, states, 5, 5, 10, 10, 10, 180, 100000);

    for (const auto& feature : features) {
        std::cout << feature << std::endl;
    }
}
