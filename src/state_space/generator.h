#include "../../include/dlplan/state_space.h"

#include <string>


namespace dlplan::state_space::generator {

extern void generate_state_space_files(
    const std::string& domain_file,
    const std::string& instance_file);

}
