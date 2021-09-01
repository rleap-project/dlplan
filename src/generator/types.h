#ifndef DLPLAN_SRC_GENERATOR_TYPES_H_
#define DLPLAN_SRC_GENERATOR_TYPES_H_

#include <string>
#include <vector>
#include <limits>


using namespace std::string_literals;

namespace dlplan {
namespace core {
    class State;
}
namespace generator {

using States = std::vector<core::State>;
using FeatureRepresentations = std::vector<std::string>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

const int UNDEFINED = -1;
const int INF = std::numeric_limits<int>::max();

}
}

#endif
