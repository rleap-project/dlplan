#include "generator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../utils/command.h"

using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space::generator {

void generate_state_space_files(const std::string& domain_file,
    const std::string& instance_file) {
    utils::Command::exec(
        "python3 -c \"import state_space_generator.state_space_generator; state_space_generator.state_space_generator.generate_state_space(\\\"" + domain_file + "\\\", \\\"" + instance_file + "\\\")\"");
}

}
