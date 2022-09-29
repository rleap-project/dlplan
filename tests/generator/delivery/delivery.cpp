#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../include/dlplan/core.h"
#include "../include/dlplan/generator.h"
#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::generator;
using namespace dlplan::state_space;


TEST(DLPTests, GeneratorDeliveryTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "instance_2_2_0.pddl");
    auto state_space = StateSpaceReader().read(nullptr, 0);
    auto vocabulary_info = state_space.get_instance_info_ref().get_vocabulary_info();
    auto feature_generator = FeatureGenerator();
    feature_generator.set_generate_inclusion_boolean(false);
    feature_generator.set_generate_diff_concept(false);
    feature_generator.set_generate_or_concept(false);
    feature_generator.set_generate_subset_concept(false);
    feature_generator.set_generate_and_role(false);
    feature_generator.set_generate_compose_role(false);
    feature_generator.set_generate_diff_role(false);
    feature_generator.set_generate_identity_role(false);
    feature_generator.set_generate_not_role(false);
    feature_generator.set_generate_or_role(false);
    feature_generator.set_generate_top_role(false);
    feature_generator.set_generate_transitive_reflexive_closure_role(false);
    SyntacticElementFactory syntactic_element_factory(vocabulary_info);
    States states(state_space.get_states_ref().begin(), state_space.get_states_ref().end());
    auto feature_reprs = feature_generator.generate(syntactic_element_factory, 9, 9, 9, 9, 15, 1000, 100000, 1, states);
    DenotationsCaches caches;
    std::unordered_set<BooleanDenotations*> boolean_denotations;
    std::unordered_set<NumericalDenotations*> numerical_denotations;
    for (const auto& repr : feature_reprs) {
        if (repr.substr(0, 2) == "b_") {
            boolean_denotations.insert(syntactic_element_factory.parse_boolean(repr).evaluate(states, caches));
        } else if (repr.substr(0, 2) == "n_") {
            numerical_denotations.insert(syntactic_element_factory.parse_numerical(repr).evaluate(states, caches));
        }
    }
    std::vector<BooleanDenotations*> required_boolean_denotations = {
        syntactic_element_factory.parse_boolean("b_empty(c_primitive(empty,0))").evaluate(states, caches)
    };
    std::vector<NumericalDenotations*> required_numerical_denotations = {
        syntactic_element_factory.parse_numerical("n_count(c_not(c_equal(r_primitive(at_g,0,1),r_primitive(at,0,1))))").evaluate(states, caches),
        syntactic_element_factory.parse_numerical("n_concept_distance(c_some(r_inverse(r_primitive(at,0,1)),c_primitive(truck,0)), r_primitive(adjacent,0,1), c_primitive(at_g,1))").evaluate(states, caches),
        syntactic_element_factory.parse_numerical("n_concept_distance(c_some(r_inverse(r_primitive(at,0,1)),c_primitive(truck,0)), r_primitive(adjacent,0,1), c_and(c_all(r_inverse(r_primitive(at_g,0,1)),c_bot),c_some(r_inverse(r_primitive(at,0,1)),c_primitive(package,0))))").evaluate(states, caches),
    };
    for (const auto& denotations : required_boolean_denotations) {
        auto find = boolean_denotations.find(denotations);
        EXPECT_NE(find, boolean_denotations.end());
    }
    for (const auto& denotations : required_numerical_denotations) {
        auto find = numerical_denotations.find(denotations);
        EXPECT_NE(find, numerical_denotations.end());
    }
}
