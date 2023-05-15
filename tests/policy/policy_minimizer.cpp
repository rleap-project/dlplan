#include <gtest/gtest.h>

#include "../utils/domain.h"

#include "../../include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;


TEST(DLPTests, StructuralMinimization) {
    std::string policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_b_pos \"b_empty(c_primitive(package, 0))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\") (:c_n_gt \"n_count(c_primitive(package, 0))\")) (:effects (:e_b_neg \"b_empty(r_primitive(at,0,1))\") (:e_b_neg \"b_empty(c_primitive(package, 0))\") (:e_n_dec \"n_count(r_primitive(at,0,1))\")))\n"  // 1) rule 1 and rule 2 can be merged by condition
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_b_pos \"b_empty(c_primitive(package, 0))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\") (:c_n_eq \"n_count(c_primitive(package, 0))\")) (:effects (:e_b_neg \"b_empty(r_primitive(at,0,1))\") (:e_b_neg \"b_empty(c_primitive(package, 0))\") (:e_n_dec \"n_count(r_primitive(at,0,1))\")))\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_b_pos \"b_empty(c_primitive(package, 0))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\"))             (:effects (:e_b_bot \"b_empty(c_primitive(package,0))\") (:e_b_neg \"b_empty(c_primitive(package, 0))\") (:e_n_dec \"n_count(r_primitive(at,0,1))\")))\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_b_pos \"b_empty(c_primitive(package, 0))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\"))             (:effects (:e_b_pos \"b_empty(c_primitive(package,0))\") (:e_b_neg \"b_empty(c_primitive(package, 0))\") (:e_n_dec \"n_count(r_primitive(at,0,1))\")))\n"  // 2) after 1) we can merge by effects
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_b_pos \"b_empty(c_primitive(package, 0))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\"))             (:effects (:e_n_dec \"n_count(r_primitive(at,0,1))\") (:e_b_neg \"b_empty(c_primitive(package, 0))\")              (:e_n_dec \"n_count(c_primitive(package, 0))\")))\n"  // 3) after 2) we can removed dominated rule
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(c_primitive(package,0))\") (:c_b_pos \"b_empty(r_primitive(at,0,1))\") (:c_n_gt \"n_count(r_primitive(at,0,1))\")) (:effects (:e_b_neg \"b_empty(c_primitive(package,0))\") (:e_n_dec \"n_count(r_primitive(at,0,1))\")))\n"
        ")";

    auto vocabulary_info = gripper::construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto builder = PolicyBuilder();
    auto input_policy = PolicyReader().read(policy_textual, builder, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy, builder);
    auto result_policy = PolicyReader().read(minimized_policy_textual, builder, syntactic_element_factory);
    std::cout << "Input policy:" << std::endl
              << input_policy->compute_repr() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy->compute_repr() << std::endl;
    EXPECT_EQ(minimized_policy->compute_repr(), result_policy->compute_repr());
}


TEST(DLPTests, StructuralMinimization2) {
    std::string policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\")) (:effects (:e_b_pos \"b_empty(r_primitive(at,0,1))\")))\n"
        "(:rule (:conditions (:c_b_neg \"b_empty(r_primitive(at,0,1))\")) (:effects (:e_b_pos \"b_empty(r_primitive(at,0,1))\")))\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\")) (:effects (:e_b_neg \"b_empty(r_primitive(at,0,1))\")))\n"
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:rule (:conditions ) (:effects (:e_b_pos \"b_empty(r_primitive(at,0,1))\")))\n"
        "(:rule (:conditions (:c_b_pos \"b_empty(r_primitive(at,0,1))\")) (:effects ))\n"
        ")";

    auto vocabulary_info = gripper::construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto builder = PolicyBuilder();
    auto input_policy = PolicyReader().read(policy_textual, builder, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy, builder);
    auto result_policy = PolicyReader().read(minimized_policy_textual, builder, syntactic_element_factory);
    std::cout << "Input policy:" << std::endl
              << input_policy->compute_repr() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy->compute_repr() << std::endl;
    EXPECT_EQ(minimized_policy->compute_repr(), result_policy->compute_repr());
}


TEST(DLPTests, StructuralMinimization3) {
    std::string policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_n_bot \"n_count(c_primitive(clear,0))\") (:e_b_neg \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_n_dec \"n_count(c_primitive(clear,0))\") (:e_b_neg \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\") (:e_b_bot \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\") (:e_b_neg \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")))\n"
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_b_neg \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\") (:c_b_pos \"b_empty(r_and(r_primitive(on,0,1),r_primitive(on_g,0,1)))\")) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\")))\n"
        ")";

    auto vocabulary_info = gripper::construct_blocks_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto builder = PolicyBuilder();
    auto input_policy = PolicyReader().read(policy_textual, builder, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy, builder);
    auto result_policy = PolicyReader().read(minimized_policy_textual, builder, syntactic_element_factory);
    std::cout << "Input policy:" << std::endl
              << input_policy->compute_repr() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy->compute_repr() << std::endl;
    EXPECT_EQ(minimized_policy->compute_repr(), result_policy->compute_repr());
}


TEST(DLPTests, StructuralMinimization4) {
    std::string policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_n_eq \"n_count(c_primitive(clear,0))\")) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\")) (:effects (:e_n_dec \"n_count(c_primitive(clear,0))\")))\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\")) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\")))\n"
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(clear,0))\")) (:effects (:e_n_dec \"n_count(c_primitive(clear,0))\")))\n"
        "(:rule (:conditions ) (:effects (:e_n_inc \"n_count(c_primitive(clear,0))\")))\n"
        ")";
    auto vocabulary_info = gripper::construct_blocks_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto builder = PolicyBuilder();
    auto input_policy = PolicyReader().read(policy_textual, builder, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy, builder);
    auto result_policy = PolicyReader().read(minimized_policy_textual, builder, syntactic_element_factory);
    std::cout << "Input policy:" << std::endl
              << input_policy->compute_repr() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy->compute_repr() << std::endl;
    EXPECT_EQ(minimized_policy->compute_repr(), result_policy->compute_repr());
}


TEST(DLPTests, EmpiricalMinimization) {
    std::string policy_textual =
        "(:policy\n"
        "(:rule (:conditions (:c_n_gt \"n_count(c_primitive(holding,0))\") (:c_n_gt \"n_count(c_equal(r_primitive(at,0,1),r_primitive(at_g,0,1)))\") (:c_n_gt \"n_count(r_and(r_primitive(at,0,1),r_primitive(at_g,0,1)))\")) (:effects (:e_n_inc \"n_count(c_primitive(holding,0))\") (:e_n_bot \"n_count(c_equal(r_primitive(at,0,1),r_primitive(at_g,0,1)))\") (:e_n_bot \"n_count(r_and(r_primitive(at,0,1),r_primitive(at_g,0,1)))\"))))\n"
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:rule (:conditions ) (:effects (:e_n_inc \"n_count(c_primitive(holding,0))\")))\n"
        ")";

    auto vocabulary_info = gripper::construct_vocabulary_info();
    auto instance_info = gripper::construct_instance_info(vocabulary_info);
    const auto& atoms = instance_info->get_atoms();
    auto at_p1_A = atoms[0];
    auto at_p1_B = atoms[1];
    auto at_p2_A = atoms[2];
    auto at_p2_B = atoms[3];
    auto at_p3_A = atoms[4];
    auto at_p3_B = atoms[5];
    auto at_roboter_A = atoms[6];
    auto at_roboter_B = atoms[7];
    auto holding_p1 = atoms[8];
    auto holding_p2 = atoms[9];
    auto holding_p3 = atoms[10];
    auto A_A_A_B = State(instance_info, {at_roboter_A, at_p1_A, at_p2_A, at_p3_B});
    auto B_A_A_B = State(instance_info, {at_roboter_B, at_p1_A, at_p2_A, at_p3_B});
    auto A_H_A_B = State(instance_info, {at_roboter_A, holding_p1, at_p2_A, at_p3_B});
    auto B_A_A_H = State(instance_info, {at_roboter_B, holding_p1, at_p2_A, at_p3_B});
    auto A_A_A_B_B_A_A_B = StatePair(A_A_A_B, B_A_A_B);  // move robot from A to B
    auto B_A_A_B_A_A_A_B = StatePair(B_A_A_B, A_A_A_B);  // move robot from B to A
    auto A_A_A_B_A_H_A_B = StatePair(A_A_A_B, A_H_A_B);  // pick up package 1 at A
    auto B_A_A_B_B_A_A_H = StatePair(B_A_A_B, B_A_A_H);  // pick up package 3 at B
    auto true_state_pairs = {A_A_A_B_A_H_A_B, B_A_A_B_B_A_A_H};  // picking up is good
    auto false_state_pairs =  {A_A_A_B_B_A_A_B, B_A_A_B_A_A_A_B};  // not picking up a package is bad

    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto builder = PolicyBuilder();
    auto input_policy = PolicyReader().read(policy_textual, builder, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy, true_state_pairs, false_state_pairs, builder);
    auto result_policy = PolicyReader().read(minimized_policy_textual, builder, syntactic_element_factory);
    std::cout << "Input policy:" << std::endl
              << input_policy->compute_repr() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy->compute_repr() << std::endl;
    EXPECT_EQ(minimized_policy->compute_repr(), result_policy->compute_repr());
}
