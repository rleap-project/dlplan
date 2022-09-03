#include <gtest/gtest.h>

#include "../include/dlplan/policy.h"

using namespace dlplan::core;
using namespace dlplan::policy;

std::shared_ptr<const VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    vocabulary_info->add_predicate("role", 2);
    vocabulary_info->add_predicate("concept", 1);
    return vocabulary_info;
}

SyntacticElementFactory construct_syntactic_element_factory(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    return SyntacticElementFactory(vocabulary_info);
}

TEST(DLPTests, PolicyMinimizer) {
    std::string policy_textual =
        "(:policy"
        "(:boolean_features \"b_empty(r_primitive(role,0,1))\" \"b_empty(c_primitive(concept, 0))\" )"
        "(:numerical_features \"n_count(r_primitive(role,0,1))\" \"n_count(c_primitive(concept, 0))\")"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_gt 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0) (:e_n_dec 1)))"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0) (:c_n_eq 1)) (:effects (:e_b_neg 0) (:e_b_neg 1) (:e_n_dec 0) (:e_n_dec 1)))"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_bot 0) (:e_b_neg 1) (:e_n_dec 0) (:e_n_dec 1)))"
        "(:rule (:conditions (:c_b_pos 0) (:c_b_pos 1) (:c_n_gt 0))             (:effects (:e_b_pos 0) (:e_b_neg 1) (:e_n_dec 0) (:e_n_dec 1)))"
        ")";
    auto vocabulary_info = construct_vocabulary_info();
    auto syntactic_element_factory = construct_syntactic_element_factory(vocabulary_info);
    auto policy = PolicyReader().read(policy_textual, syntactic_element_factory);
    std::cout << policy.compute_repr() << std::endl;
    auto minimized_policy = PolicyMinimizer().minimize_greedy(policy);
    std::cout << minimized_policy.compute_repr() << std::endl;
}
