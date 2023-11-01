#include "generator.h"

#include "../utils/command.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space::generator {

void generate_state_space_files(
    const std::string& domain_file,
    const std::string& instance_file,
    int max_time,
    int max_num_states) {
    utils::Command::exec(
        "python3 -c \"import state_space_generator.state_space_generator; state_space_generator.state_space_generator.generate_state_space(\\\"" + domain_file + "\\\", \\\"" + instance_file + "\\\", " + std::to_string(max_time) + "," + std::to_string(max_num_states) + ")\"");
}

}
