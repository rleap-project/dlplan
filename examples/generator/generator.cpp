#include "../../include/dlplan/generator.h"

#include <iostream>

using namespace dlplan::core;
using namespace dlplan::generator;

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


int main() {
    // 1. Initialize VocabularyInfo
    auto v = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto i = construct_instance_info(v);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory factory(v);

    // 4. Construct a bunch of states
    const auto& atoms = i->get_atoms();
    const Atom& a0 = atoms[0];
    const Atom& a1 = atoms[1];
    const Atom& a2 = atoms[2];
    const Atom& a3 = atoms[3];
    const Atom& a4 = atoms[4];
    const Atom& a5 = atoms[5];
    const Atom& a6 = atoms[6];
    const Atom& a7 = atoms[7];
    const Atom& a8 = atoms[8];
    State s0(i, {a0, a3, a6, a8});
    State s1(i, {a1, a2, a7, a8});
    State s2(i, {a2, a3, a6, a7, a8});
    State s3(i, {a3, a4, a7});
    State s4(i, {a2, a5, a6});
    States states({s0, s1, s2, s3, s4});

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
