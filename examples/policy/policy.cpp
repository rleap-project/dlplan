/// Example illustrating the policy component.

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/policy.h"

#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>


using namespace dlplan::policy;
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


/// @brief Example illustrating the policy component on a fragment of a planning
///        problem over the Blocks domain.
///
/// The examples focuses on the creation and evaluation of a Policy on a
/// planning instance over the Blocks planning domain.
int main() {
    auto vocabulary = construct_vocabulary_info();
    auto instance = construct_instance_info(vocabulary);
    auto element_factory = std::make_shared<dlplan::core::SyntacticElementFactory>(vocabulary);
    PolicyFactory policy_factory(element_factory);

    std::cout << "Constructing policy:" << std::endl;
    // boolean represents whether the hand is empty or not
    auto boolean = policy_factory.make_boolean("b0", element_factory->parse_boolean("b_nullary(arm-empty)"));
    // numerical representes the number of blocks on top of another block
    auto numerical = policy_factory.make_numerical("n1", element_factory->parse_numerical("n_count(r_primitive(on,0,1))"));
    auto b_pos_condition_0 = policy_factory.make_pos_condition(boolean);
    auto b_bot_effect_0 = policy_factory.make_bot_effect(boolean);
    auto n_gt_condition_0 = policy_factory.make_gt_condition(numerical);
    auto n_dec_effect_0 = policy_factory.make_dec_effect(numerical);
    // rule_1 represents that picking up a block that is on top of another block is good
    auto rule_1 = policy_factory.make_rule(
        {b_pos_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    );
    auto policy = policy_factory.make_policy({rule_1});

    const auto& atoms = instance->get_atoms();
    const Atom& atom_0 = atoms[0];
    const Atom& atom_1 = atoms[1];
    const Atom& atom_2 = atoms[2];
    const Atom& atom_3 = atoms[3];
    const Atom& atom_6 = atoms[6];
    const Atom& atom_7 = atoms[7];
    const Atom& atom_8 = atoms[8];
    // User must ensure that each State gets its unique index for caching.
    State state_0(0, instance, {atom_0, atom_3, atom_6, atom_8});  // a on b
    State state_1(1, instance, {atom_1, atom_2, atom_7, atom_8});  // b on a
    State state_2(2, instance, {atom_2, atom_3, atom_6, atom_7, atom_8});  // a,b on table

    dlplan::core::DenotationsCaches caches;
    assert(policy->evaluate(state_0, state_2, caches));
    assert(policy->evaluate(state_1, state_2, caches));
    assert(!policy->evaluate(state_2, state_0, caches));
    assert(!policy->evaluate(state_2, state_1, caches));

    std::cout << policy->str() << std::endl << std::endl;

    std::cout << "Parsing policy:" << std::endl;
    std::string policy_str =
        "(:policy\n"
        "(:booleans (b0 \"b_nullary(arm-empty)\") (b0 \"b_nullary(arm-empty)\"))\n"
        "(:numericals (n0 \"n_count(r_primitive(on,0,1))\"))\n"
        "(:rule (:conditions (:c_b_pos b0) (:c_n_gt n0)) (:effects (:e_b_bot b0) (:e_n_dec n0)))\n"
        ")";
    auto policy_in = policy_factory.parse_policy(policy_str);
    std::cout << policy_in->str() << std::endl << std::endl;

    return 0;
}
