#include "../../include/dlplan/state_space.h"

#include <experimental/filesystem>
#include <iostream>
#include <fstream>

#include "../utils/command.h"


using namespace dlplan::core;


namespace dlplan::state_space {


static void parse_goal_atoms(InstanceInfo& instance_info, const std::string& goal_atoms_data) {

}

static void parse_static_atoms(InstanceInfo& instance_info, const std::string& static_atoms_data) {

}

static void parse_predicates(VocabularyInfo& vocabulary_info, const std::string& predicate_data) {

}

static void parse_state_space(std::shared_ptr<InstanceInfo> instance_info, const std::string& state_space_data) {

}


StateSpace StateSpaceGenerator::generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file) const {
    // Generate all necessary output files.
    // https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html
    std::stringstream command;
    command << std::getenv("DLPLAN_ROOT_DIR") << "/libs/scorpion/fast-downward.py"
            << " " << domain_file
            << " " << instance_file
            << " --translate-options"
            << " --dump-static-atoms"
            << " --dump-predicates"
            << " --dump-goal-atoms"
            << " --search-options"
            << " --search"
            << " \"dump_reachable_search_space()\"";
    std::cout << command.str() << std::endl;
    auto command_result = utils::Command::exec(command.str());
    std::cout << command_result.output << std::endl;
    std::ofstream state_space_file;
    state_space_file.open("state_space.txt");
    state_space_file << command_result.output;
    state_space_file.close();
}

}