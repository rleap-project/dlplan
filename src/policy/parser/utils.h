#ifndef DLPLAN_SRC_POLICY_PARSER_UTILS_H_
#define DLPLAN_SRC_POLICY_PARSER_UTILS_H_

#include <algorithm>
#include <string>


namespace dlplan::policy::parser {

int try_parse_number(const std::string& name);

bool is_number(const std::string& name);


}

#endif
