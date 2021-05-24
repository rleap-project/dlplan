#ifndef DLP_SRC_CORE_TYPES_H_
#define DLP_SRC_CORE_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <limits>

using namespace std::string_literals;

namespace dlp {
namespace core {

using Concept = int;
using Concepts = std::vector<Concept>;
using Concepts_Set = std::unordered_set<Concept>;

using Role = std::pair<Concept, Concept>;
using Roles = std::vector<Role>;
using Roles_Set = std::unordered_set<Role>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

const int UNDEFINED = -1;
const int INF = std::numeric_limits<int>::max();

}
}

#endif
