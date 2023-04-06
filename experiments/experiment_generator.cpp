#include <vector>
#include <iostream>
#include <chrono>

#include "../include/dlplan/core.h"
#include "../include/dlplan/generator.h"
#include "../include/dlplan/state_space.h"

using namespace dlplan;


int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc != 12) {
        std::cout << "User error. Expected: ./experiment_core <str:domain_filename> <str:instance_filename> <int:concept_complexity_limit> <int:role_complexity_limit> <int:boolean_complexity_limit> <int:count_numerical_complexity_limit> <int:distance_numerical_complexity_limit> <int:time_limit> <int:features_limit> <int:limit_threads> <int:num_iterations>" << std::endl;
        return 1;
    }
    std::string domain_filename = argv[1];
    std::string instance_filename = argv[2];
    int concept_complexity_limit = std::atoi(argv[3]);
    int role_complexity_limit = std::atoi(argv[4]);
    int boolean_complexity_limit = std::atoi(argv[5]);
    int count_numerical_complexity_limit = std::atoi(argv[6]);
    int distance_numerical_complexity_limit = std::atoi(argv[7]);
    int time_limit = std::atoi(argv[8]);
    int feature_limit = std::atoi(argv[9]);
    int threads_limit = std::atoi(argv[10]);
    int num_iterations = std::atoi(argv[11]);
    std::cout << "Number of iterations: " << num_iterations << std::endl;

    auto state_space =  state_space::generate_state_space(domain_filename, instance_filename, nullptr, 0);
    std::cout << "Started generating features" << std::endl;
    std::cout << "Number of states: " << state_space.get_states().size() << std::endl;
    std::cout << "Number of dynamic atoms: " << state_space.get_instance_info()->get_atoms().size() << std::endl;
    std::cout << "Number of static atoms: " << state_space.get_instance_info()->get_static_atoms().size() << std::endl;

    auto syntactic_element_factory = core::SyntacticElementFactory(state_space.get_instance_info()->get_vocabulary_info());
    auto feature_generator = generator::FeatureGenerator();
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
    // feature_generator.set_generate_top_role(false);
    feature_generator.set_generate_transitive_reflexive_closure_role(false);
    core::States states;
    std::for_each(state_space.get_states().begin(), state_space.get_states().end(), [&](const auto& pair){ states.push_back(pair.second); });
    auto feature_reprs = feature_generator.generate(
        syntactic_element_factory,
        states,
        concept_complexity_limit,
        role_complexity_limit,
        boolean_complexity_limit,
        count_numerical_complexity_limit,
        distance_numerical_complexity_limit,
        time_limit,
        feature_limit);

    std::vector<core::Boolean> boolean_features;
    std::vector<core::Numerical> numerical_features;
    for (const auto& repr : feature_reprs) {
        if (repr.substr(0, 2) == "b_") {
            boolean_features.push_back(syntactic_element_factory.parse_boolean(repr));
        } else if (repr.substr(0, 2) == "n_") {
            numerical_features.push_back(syntactic_element_factory.parse_numerical(repr));
        }
    }

    std::cout << "Started element evaluation" << std::endl;
    std::cout << "Number of booleans: " << boolean_features.size() << std::endl;
    std::cout << "Number of numericals: " << numerical_features.size() << std::endl;

    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < num_iterations; ++i) {
            for (const auto& pair : state_space.get_states()) {
                for (const auto& boolean : boolean_features) {
                    boolean.evaluate(pair.second);
                }
                for (const auto& numerical : numerical_features) {
                    numerical.evaluate(pair.second);
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for single state without cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        core::DenotationsCaches caches;
        for (int i = 0; i < std::atoi(argv[10]); ++i) {
            for (const auto& pair : state_space.get_states()) {
                for (const auto& boolean : boolean_features) {
                    boolean.evaluate(pair.second, caches);
                }
                for (const auto& numerical : numerical_features) {
                    numerical.evaluate(pair.second, caches);
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for single state with cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        core::DenotationsCaches caches;
        for (int i = 0; i < std::atoi(argv[10]); ++i) {
            for (const auto& boolean : boolean_features) {
                boolean.evaluate(states, caches);
            }
            for (const auto& numerical : numerical_features) {
                numerical.evaluate(states, caches);
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time evaluate features for all states with cache: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
    }
    return 0;
}