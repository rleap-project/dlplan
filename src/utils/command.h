#ifndef DLPLAN_SRC_UTILS_COMMAND_H
#define DLPLAN_SRC_UTILS_COMMAND_H

#include <array>
#include <ostream>


namespace dlplan::utils {

/**
 * Source: https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html
 * Date of last access: 2022-09-09
 */

struct CommandResult {
    std::string output;
    int exitstatus;

    friend std::ostream &operator<<(std::ostream &os, const CommandResult &result) {
        os << "command exitstatus: " << result.exitstatus << " output: " << result.output;
        return os;
    }
    bool operator==(const CommandResult &rhs) const {
        return output == rhs.output &&
               exitstatus == rhs.exitstatus;
    }
    bool operator!=(const CommandResult &rhs) const {
        return !(rhs == *this);
    }
};

class Command {
public:
    /**
     * Execute system command and get STDOUT result.
     * Like system() but gives back exit status and stdout.
     * @param command system command to execute
     * @return CommandResult containing STDOUT (not stderr) output & exitstatus
     * of command. Empty if command failed (or has no output). If you want stderr,
     * use shell redirection (2&>1).
     */
    static CommandResult exec(const std::string &command) {
        int exitcode = 255;
        std::array<char, 1048576> buffer {};
        std::string result;
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif
        FILE *pipe = popen(command.c_str(), "r");
        if (pipe == nullptr) {
            throw std::runtime_error("popen() failed!");
        }
        try {
            std::size_t bytesread;
            while ((bytesread = fread(buffer.data(), sizeof(buffer.at(0)), sizeof(buffer), pipe)) != 0) {
                result += std::string(buffer.data(), bytesread);
            }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        int return_code = pclose(pipe);
        exitcode = WEXITSTATUS(return_code);
        return CommandResult{result, exitcode};
    }
};


}

#endif
