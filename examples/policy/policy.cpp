/// Example illustrating the policy component.

#include "../include/dlplan/core.h"
#include "../include/dlplan/policy.h"

#include <fstream>
#include <cassert>
#include <sstream>


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


/// @brief Example illustrating the policy component on a fragment of a planning
///        problem over the Blocks domain.
///
/// The examples focuses on the creation and evaluation of a Policy on a
/// planning instance over the Blocks planning domain.
int main() {
    auto vocabulary = construct_vocabulary_info();
    auto instance = construct_instance_info(vocabulary);

    dlplan::core::SyntacticElementFactory factory(vocabulary);
    // boolean_1 represents whether the hand is empty or not
    auto boolean_1 = factory.parse_boolean("b_nullary(arm-empty)");
    // numerical_1 representes the number of blocks on top of another block
    auto numerical_1 = factory.parse_numerical("n_count(r_primitive(on,0,1))");
    PolicyBuilder builder;
    auto b_pos_condition_0 = builder.add_pos_condition(boolean_1);
    auto b_bot_effect_0 = builder.add_bot_effect(boolean_1);
    auto n_gt_condition_0 = builder.add_gt_condition(numerical_1);
    auto n_dec_effect_0 = builder.add_dec_effect(numerical_1);
    // rule_1 represents that picking up a block that is on top of another block is good
    auto rule_1 = builder.add_rule(
        {b_pos_condition_0, n_gt_condition_0},
        {b_bot_effect_0, n_dec_effect_0}
    );
    auto policy = builder.add_policy({rule_1});

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
    State state_1(instance, {atom_0, atom_3, atom_6, atom_8}, 1);  // a on b
    State state_2(instance, {atom_1, atom_2, atom_7, atom_8}, 2);  // b on a
    State state_3(instance, {atom_2, atom_3, atom_6, atom_7, atom_8}, 3);  // a,b on table
    State state_4(instance, {atom_3, atom_4, atom_7}, 4);  // holding a, b on table
    State state_5(instance, {atom_2, atom_5, atom_6}, 5);  // holding b, a on table

    dlplan::core::DenotationsCaches caches;
    assert(policy->evaluate(state_1, state_3, caches));
    assert(policy->evaluate(state_2, state_3, caches));
    assert(!policy->evaluate(state_3, state_1, caches));
    assert(!policy->evaluate(state_3, state_2, caches));

    std::cout << "Write policy:" << std::endl;
    std::cout << policy->compute_repr() << std::endl << std::endl;
    std::cout << policy->str() << std::endl << std::endl;
    std::ofstream ofs;
    ofs.open("test.txt", std::ofstream::out);
    ofs << PolicyWriter().write(*policy);
    ofs.close();

    std::ifstream ifs;
    ifs.open("test.txt", std::ifstream::in);
    std::stringstream ss;
    ss << ifs.rdbuf();
    auto policy_in = PolicyReader().read(ss.str(), builder, factory);
    ifs.close();
    std::cout << "Read policy:" << std::endl;
    std::cout << policy_in->compute_repr() << std::endl << std::endl;
    std::cout << policy_in->str() << std::endl << std::endl;
    assert(policy->compute_repr() == policy_in->compute_repr());
    return 0;
}
