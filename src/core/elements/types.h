#ifndef DLP_SRC_CORE_ELEMENTS_TYPES_H_
#define DLP_SRC_CORE_ELEMENTS_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <limits>

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

}
}
}

#endif
