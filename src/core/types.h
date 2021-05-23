#ifndef DLP_SRC_FEATURE_FACTORY_TYPES_H_
#define DLP_SRC_FEATURE_FACTORY_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <limits>

using namespace std::string_literals;

namespace dlp {

class ConceptElement;
class RoleElement;
class NumericalElement;
class BooleanElement;

using ConceptElement_Ptr = std::shared_ptr<ConceptElement>;
using RoleElement_Ptr = std::shared_ptr<RoleElement>;
using NumericalElement_Ptr = std::shared_ptr<NumericalElement>;
using BooleanElement_Ptr = std::shared_ptr<BooleanElement>;

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

#endif
