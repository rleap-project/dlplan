#include "../../include/dlplan/state_space.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../utils/command.h"


using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space {

ExitCode StateSpaceGenerator::generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file) const {
    std::stringstream command;
    command << "./fast-downward.py"
            << " " << domain_file
            << " " << instance_file
            << " --translate-options"
            << " --dump-static-predicates"
            << " --dump-predicates"
            << " --dump-constants"
            << " --dump-static-atoms"
            << " --dump-goal-atoms"
            << " --search-options"
            << " --search"
            << " \"dump_reachable_search_space()\"";
    auto command_result = utils::Command::exec(command.str());
    std::ofstream state_space_file;
    state_space_file.open("planner.log");
    state_space_file << command_result.output;
    state_space_file.close();
    return ExitCode(command_result.exitstatus);
}

}
