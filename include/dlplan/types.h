#ifndef DLPLAN_INCLUDE_DLPLAN_TYPES_H_
#define DLPLAN_INCLUDE_DLPLAN_TYPES_H_


#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

namespace dlplan::core {

using ConceptDenotation = std::vector<int>;
using ConceptDenotation_Set = std::unordered_set<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;
struct RoleDenotationHash {
    std::size_t operator()(const std::pair<int, int> &r) const {
        // return boost::hash_value(r);
        // return std::hash<int>()(r.first) ^ std::hash<int>()(r.second);
        return r.first << ((sizeof(size_t) / 2) + r.second);
    }
};
using RoleDenotation_Set = std::unordered_set<std::pair<int, int>, RoleDenotationHash>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

}

#endif
