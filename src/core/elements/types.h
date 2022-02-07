#ifndef DLPLAN_SRC_CORE_ELEMENTS_TYPES_H_
#define DLPLAN_SRC_CORE_ELEMENTS_TYPES_H_

#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>


namespace dlplan::core::element {

class Concept;
class Role;
class Numerical;
class Boolean;

using Concept_Ptr = std::shared_ptr<const Concept>;
using Role_Ptr = std::shared_ptr<const Role>;
using Numerical_Ptr = std::shared_ptr<const Numerical>;
using Boolean_Ptr = std::shared_ptr<const Boolean>;

}

#endif
