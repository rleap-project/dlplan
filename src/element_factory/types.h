#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>

namespace dlp {
class ConceptElement;
class RoleElement;
class NumericalElement;
class BooleanElement;

using ConceptElement_Ptr = std::shared_ptr<ConceptElement>;
using RoleElement_Ptr = std::shared_ptr<RoleElement>;
using NumericalElement_Ptr = std::shared_ptr<NumericalElement>;
using BooleanElement_Ptr = std::shared_ptr<BooleanElement>;

using Concept = unsigned;
using Concepts = std::vector<Concept>;
using Concepts_Set = std::unordered_set<Concept>;

using Role = std::pair<Concept, Concept>;
using Roles = std::vector<Role>;
using Roles_Set = std::unordered_set<Role>;

using Name_Vec = const std::vector<const std::string>;
using Index_Vec = const std::vector<unsigned>;

}
