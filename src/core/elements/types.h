#ifndef DLP_SRC_CORE_ELEMENTS_TYPES_H_
#define DLP_SRC_CORE_ELEMENTS_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>


namespace dlp {
namespace core {
namespace element {

class Concept;
class Role;
class Numerical;
class Boolean;

using Concept_Ptr = std::shared_ptr<Concept>;
using Role_Ptr = std::shared_ptr<Role>;
using Numerical_Ptr = std::shared_ptr<Numerical>;
using Boolean_Ptr = std::shared_ptr<Boolean>;

using ConceptDenotation = std::vector<int>;
using ConceptDenotation_Set = std::unordered_set<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;
struct RoleDenotationHash {
    std::size_t operator()(const std::pair<int, int> &r) const {
        // return boost::hash_value(r);
        // return std::hash<int>()(r.first) ^ std::hash<int>()(r.second);
        return r.first << (sizeof(size_t) / 2) + r.second;
    }
};
using RoleDenotation_Set = std::unordered_set<std::pair<int, int>, RoleDenotationHash>;

}
}
}

#endif
