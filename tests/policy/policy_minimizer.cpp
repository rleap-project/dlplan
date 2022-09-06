#include <gtest/gtest.h>

#include "utils.h"

#include "../include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;


TEST(DLPTests, StructuralMinimization) {
    std::string policy_textual =
        "(:policy\n"
        "(:boolean_features \"b_empty(r_primitive(at,0,1))\" \"b_empty(c_primitive(package, 0))\")\n"
        "(:numerical_features \"n_count(r_primitive(at,0,1))\" \"n_count(c_primitive(package, 0))\")\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_gt 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0)))\n"  // 1) rule 1 and rule 2 can be merged by condition
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_eq 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0)))\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_bot 0) (:e_b_neg 1) (:e_n_dec 0)))\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_pos 0) (:e_b_neg 1) (:e_n_dec 0)))\n"  // 2) after 1) we can merge by effects
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_n_dec 0) (:e_b_neg 1)              (:e_n_dec 1)))\n"  // 3) after 2) we can removed dominated rule
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:boolean_features \"b_empty(c_primitive(package,0))\" \"b_empty(r_primitive(at,0,1))\")\n"
        "(:numerical_features \"n_count(r_primitive(at,0,1))\")\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0)) (:effects (:e_b_neg 0) (:e_n_dec 0)))\n"
        ")";

    auto vocabulary_info = construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto input_policy = PolicyReader().read(policy_textual, syntactic_element_factory);
    auto minimized_policy = PolicyMinimizer().minimize(input_policy);
    std::cout << "Input policy:" << std::endl
              << input_policy.str() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy.compute_repr() << std::endl;
    ASSERT_EQ(minimized_policy.compute_repr(), minimized_policy_textual);
}


TEST(DLPTests, EmpiricalMinimization) {
    std::string policy_textual =
        "(:policy\n"
        "(:boolean_features \"b_empty(r_primitive(at,0,1))\" \"b_empty(c_primitive(package, 0))\")\n"
        "(:numerical_features \"n_count(r_primitive(at,0,1))\" \"n_count(c_primitive(package, 0))\")\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_gt 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0)))\n"  // 1) rule 1 and rule 2 can be merged by condition
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_eq 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0)))\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_bot 0) (:e_b_neg 1) (:e_n_dec 0)))\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_pos 0) (:e_b_neg 1) (:e_n_dec 0)))\n"  // 2) after 1) we can merge by effects
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_n_dec 0) (:e_b_neg 1)              (:e_n_dec 1)))\n"  // 3) after 2) we can removed dominated rule
        ")";
    std::string minimized_policy_textual =
        "(:policy\n"
        "(:boolean_features \"b_empty(c_primitive(package,0))\" \"b_empty(r_primitive(at,0,1))\")\n"
        "(:numerical_features \"n_count(r_primitive(at,0,1))\")\n"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0)) (:effects (:e_b_neg 0) (:e_n_dec 0)))\n"
        ")";

    auto vocabulary_info = construct_vocabulary_info();
    auto instance_info = construct_instance_info(vocabulary_info);
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto input_policy = PolicyReader().read(policy_textual, syntactic_element_factory);

    auto minimized_policy = PolicyMinimizer().minimize(input_policy);
    std::cout << "Input policy:" << std::endl
              << input_policy.str() << std::endl << std::endl
              << "Minimized policy:" << std::endl
              << minimized_policy.compute_repr() << std::endl;
    // ASSERT_EQ(minimized_policy.compute_repr(), minimized_policy_textual);
}
