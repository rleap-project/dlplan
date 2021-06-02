#ifndef DLP_SRC_CORE_TYPES_H_
#define DLP_SRC_CORE_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <limits>

#include "boost/functional/hash.hpp"


using namespace std::string_literals;

namespace dlp {
namespace core {

using ConceptDenotation = std::vector<int>;
using ConceptDenotation_Set = std::unordered_set<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;
struct RoleDenotationHash {
    std::size_t operator()(const std::pair<int, int> &r) const {
        return boost::hash_value(r);
    }
};
using RoleDenotation_Set = std::unordered_set<std::pair<int, int>, RoleDenotationHash>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

const int UNDEFINED = -1;
const int INF = std::numeric_limits<int>::max();

}
}

#endif
