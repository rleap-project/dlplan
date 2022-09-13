#ifndef DLPLAN_INCLUDE_DLPLAN_TYPES_H_
#define DLPLAN_INCLUDE_DLPLAN_TYPES_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>


namespace dlplan::core {

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;
using Index_Set = std::unordered_set<int>;
using IndexPair_Vec = std::vector<std::pair<int, int>>;

}

#endif
