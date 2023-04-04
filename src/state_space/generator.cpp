#include "generator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../utils/command.h"

#include <pybind11/embed.h> // everything needed for embedding
namespace py = pybind11;


using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space::generator {

void generate_state_space_files(const std::string& domain_file,
    const std::string& instance_file) {
    py::scoped_interpreter guard{};
    py::module_ state_space_generator = py::module_::import("state_space_generator.state_space_generator");
    state_space_generator.attr("generate_state_space")(domain_file, instance_file);
}

}
