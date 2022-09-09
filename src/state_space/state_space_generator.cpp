#include "../../include/dlplan/state_space.h"

#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

using namespace dlplan::core;


namespace dlplan::state_space {

void StateSpaceGenerator::generate_state_space_data(
    const std::string& domain_file,
    const std::string& instance_file,
    const std::string& output_file) const {
    // https://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file
    int fd = open("state_space.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    close(fd);
    // https://stackoverflow.com/questions/52164723/how-to-execute-a-command-and-get-return-code-stdout-and-stderr-of-command-in-c
    // https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html
    execl((std::filesystem::path(std::getenv("DLPLAN_ROOT_DIR")) / std::filesystem::path("libs/scorpion/fast-downward.py")).c_str(), "fast-downward.py", domain_file.c_str(), instance_file.c_str(), "--translate-options", "--dump-static-atoms", "--dump-predicates", "--dump-goal-atoms", "--search-options", "--search", "dump_reachable_search_space()", nullptr);
}

}