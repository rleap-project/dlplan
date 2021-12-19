#ifndef DLPLAN_INCLUDE_DLPLAN_TYPES_H_
#define DLPLAN_INCLUDE_DLPLAN_TYPES_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

#include "dynamic_bitset.h"


namespace dlplan::core {

using ConceptDenotation = dynamic_bitset::DynamicBitset<unsigned int>;
using RoleDenotation = dynamic_bitset::DynamicBitset<unsigned int>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

}

#endif
