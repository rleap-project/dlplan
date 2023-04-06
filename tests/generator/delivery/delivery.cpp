#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../../../include/dlplan/core.h"
#include "../../../include/dlplan/generator.h"
#include "../../../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::generator;
using namespace dlplan::state_space;


TEST(DLPTests, GeneratorDeliveryTest) {
    auto state_space = generate_state_space("domain.pddl", "instance_4_2_29.pddl", nullptr, 0);
    auto vocabulary_info = state_space.get_instance_info()->get_vocabulary_info();
    auto feature_generator = FeatureGenerator();
    feature_generator.set_generate_inclusion_boolean(false);
    feature_generator.set_generate_diff_concept(false);
    feature_generator.set_generate_or_concept(false);
    feature_generator.set_generate_subset_concept(false);
    feature_generator.set_generate_and_role(false);
    feature_generator.set_generate_projection_concept(false);
    feature_generator.set_generate_compose_role(false);
    feature_generator.set_generate_diff_role(false);
    feature_generator.set_generate_identity_role(false);
    feature_generator.set_generate_not_role(false);
    feature_generator.set_generate_or_role(false);
    feature_generator.set_generate_top_role(false);
    feature_generator.set_generate_transitive_reflexive_closure_role(false);
    SyntacticElementFactory syntactic_element_factory(vocabulary_info);
    States states;
    std::for_each(state_space.get_states().begin(), state_space.get_states().end(), [&](const auto& pair){states.push_back(pair.second); });
    auto feature_reprs = feature_generator.generate(syntactic_element_factory, states, 9, 9, 9, 9, 15, 1000, 100000);
    std::vector<Boolean> generated_boolean_features;
    std::vector<Numerical> generated_numerical_features;
    std::vector<Concept> generated_concept_features;
    std::vector<Role> generated_role_features;
    for (const auto& repr : feature_reprs) {
        if (repr.substr(0, 2) == "b_") {
            generated_boolean_features.push_back(syntactic_element_factory.parse_boolean(repr));
        } else if (repr.substr(0, 2) == "n_") {
            generated_numerical_features.push_back(syntactic_element_factory.parse_numerical(repr));
        } else if (repr.substr(0, 2) == "c_") {
            generated_concept_features.push_back(syntactic_element_factory.parse_concept(repr));
        } else if (repr.substr(0, 2) == "r_") {
            generated_role_features.push_back(syntactic_element_factory.parse_role(repr));
        }
    }
    DenotationsCaches caches;
    std::vector<BooleanDenotations*> generated_boolean_denotations;
    for (const auto& boolean : generated_boolean_features) {
        generated_boolean_denotations.push_back(boolean.evaluate(states, caches));
    }
    std::vector<NumericalDenotations*> generated_numerical_denotations;
    for (const auto& numerical : generated_numerical_features) {
        generated_numerical_denotations.push_back(numerical.evaluate(states, caches));
    }
    std::vector<ConceptDenotations*> generated_concept_denotations;
    for (const auto& concept : generated_concept_features) {
        generated_concept_denotations.push_back(concept.evaluate(states, caches));
    }
    std::vector<RoleDenotations*> generated_role_denotations;
    for (const auto& role : generated_role_features) {
        generated_role_denotations.push_back(role.evaluate(states, caches));
    }

    std::vector<Boolean> required_boolean_features = {
        syntactic_element_factory.parse_boolean("b_empty(c_primitive(empty,0))")
    };
    std::vector<Numerical> required_numerical_features = {
        syntactic_element_factory.parse_numerical("n_count(c_not(c_equal(r_primitive(at_g,0,1),r_primitive(at,0,1))))"),
        syntactic_element_factory.parse_numerical("n_concept_distance(c_some(r_inverse(r_primitive(at,0,1)),c_primitive(truck,0)), r_primitive(adjacent,0,1), c_primitive(at_g,1))"),
        syntactic_element_factory.parse_numerical("n_concept_distance(c_some(r_inverse(r_primitive(at,0,1)),c_primitive(truck,0)), r_primitive(adjacent,0,1), c_and(c_all(r_inverse(r_primitive(at_g,0,1)),c_bot),c_some(r_inverse(r_primitive(at,0,1)),c_primitive(package,0))))"),
    };
    std::vector<Concept> required_concept_features = {
        syntactic_element_factory.parse_concept("c_and(c_all(r_inverse(r_primitive(at_g,0,1)),c_bot),c_some(r_inverse(r_primitive(at,0,1)),c_primitive(package,0)))"),
        syntactic_element_factory.parse_concept("c_not(c_all(r_inverse(r_primitive(at,0,1)),c_equal(r_primitive(at,0,1),r_primitive(at_g,0,1))))")
    };
    std::vector<Role> required_role_features = {

    };
    std::vector<BooleanDenotations*> required_boolean_denotations;
    for (const auto& boolean : required_boolean_features) {
        required_boolean_denotations.push_back(boolean.evaluate(states, caches));
    }
    std::vector<NumericalDenotations*> required_numerical_denotations;
    for (const auto& numerical : required_numerical_features) {
        required_numerical_denotations.push_back(numerical.evaluate(states, caches));
    }
    std::vector<ConceptDenotations*> required_concept_denotations;
    for (const auto& concept : required_concept_features) {
        required_concept_denotations.push_back(concept.evaluate(states, caches));
    }
    std::vector<RoleDenotations*> required_role_denotations;
    for (const auto& role : required_role_features) {
        required_role_denotations.push_back(role.evaluate(states, caches));
    }

    for (size_t i = 0; i < required_boolean_denotations.size(); ++i) {
        const auto& required_denotations = required_boolean_denotations[i];
        bool found = false;
        for (size_t j = 0; j < generated_boolean_denotations.size(); ++j) {
            const auto& generated_denotations = generated_boolean_denotations[j];
            if (required_denotations == generated_denotations) {
                found = true;
                std::cout << "required: " << required_boolean_features[i].compute_repr() << "\n"
                          << "generated: " << generated_boolean_features[j].compute_repr() << "\n";
            }
        }
        EXPECT_EQ(found, true);
    }
    for (size_t i = 0; i < required_numerical_denotations.size(); ++i) {
        const auto& required_denotations = required_numerical_denotations[i];
        bool found = false;
        for (size_t j = 0; j < generated_numerical_denotations.size(); ++j) {
            const auto& generated_denotations = generated_numerical_denotations[j];
            if (required_denotations == generated_denotations) {
                found = true;
                std::cout << "required: " << required_numerical_features[i].compute_repr() << "\n"
                          << "generated: " << generated_numerical_features[j].compute_repr() << "\n";
            }
        }
        EXPECT_EQ(found, true);
    }
    for (size_t i = 0; i < required_concept_denotations.size(); ++i) {
        const auto& required_denotations = required_concept_denotations[i];
        bool found = false;
        for (size_t j = 0; j < generated_concept_denotations.size(); ++j) {
            const auto& generated_denotations = generated_concept_denotations[j];
            if (required_denotations == generated_denotations) {
                found = true;
                std::cout << "required: " << required_concept_features[i].compute_repr() << "\n"
                          << "generated: " << generated_concept_features[j].compute_repr() << "\n";
            }
        }
        EXPECT_EQ(found, true);
    }
    for (size_t i = 0; i < required_role_denotations.size(); ++i) {
        const auto& required_denotations = required_role_denotations[i];
        bool found = false;
        for (size_t j = 0; j < generated_role_denotations.size(); ++j) {
            const auto& generated_denotations = generated_role_denotations[j];
            if (required_denotations == generated_denotations) {
                found = true;
                std::cout << "required: " << required_role_features[i].compute_repr() << "\n"
                          << "generated: " << generated_role_features[j].compute_repr() << "\n";
            }
        }
        EXPECT_EQ(found, true);
    }
}
