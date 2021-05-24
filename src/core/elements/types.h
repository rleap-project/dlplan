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

class ConceptElement;
class RoleElement;
class NumericalElement;
class BooleanElement;

using ConceptElement_Ptr = std::shared_ptr<ConceptElement>;
using RoleElement_Ptr = std::shared_ptr<RoleElement>;
using NumericalElement_Ptr = std::shared_ptr<NumericalElement>;
using BooleanElement_Ptr = std::shared_ptr<BooleanElement>;

}
}
}

#endif
