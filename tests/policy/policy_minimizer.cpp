#include <gtest/gtest.h>

#include "../include/dlplan/policy.h"

using namespace dlplan::policy;


TEST(DLPTests, PolicyMinimizer) {
    std::string policy =
        "(:policy"
        "(:boolean_features )"
        "(:numerical_features \"n_count(c_primitive(at_kitchen_sandwich,0))\" \"n_count(c_primitive(no_gluten_sandwich,0))\" \"n_count(c_primitive(served,0))\" \"n_count(c_and(c_not(c_primitive(served,0)),c_primitive(allergic_gluten,0)))\")"
        "(:rule (:conditions (:c_n_eq 0) (:c_n_eq 1) (:c_n_eq 2) (:c_n_gt 3)) (:effects (:e_n_inc 0) (:e_n_inc 1) (:e_n_bot 2) (:e_n_bot 3)))"
        ")";

}
