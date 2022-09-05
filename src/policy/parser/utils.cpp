#include "utils.h"

#include <stdexcept>


namespace dlplan::policy::parser {

int try_parse_number(const std::string& name) {
    if (!std::all_of(name.begin(), name.end(), [](char c){ return isdigit(c); })) {
        throw std::runtime_error("try_parse_number - error parsing (" + name + ") to int.");
    }
    return atoi(name.c_str());
}

bool is_number(const std::string& name) {
    return std::all_of(name.begin(), name.end(), [](char c){ return isdigit(c); });
}

}