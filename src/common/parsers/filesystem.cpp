#include "../../../include/dlplan/common/parsers/filesystem.hpp"

#include <string>
#include <fstream>
#include <sstream>


namespace dlplan {

std::string read_file(const fs::path& file_path) {
    std::ifstream file(file_path.c_str());
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    throw std::runtime_error("file does not exist at " + std::string(file_path.c_str()));
}

}