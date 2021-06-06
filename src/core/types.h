#ifndef DLP_SRC_CORE_TYPES_H_
#define DLP_SRC_CORE_TYPES_H_

// #include "boost/functional/hash.hpp"


using namespace std::string_literals;

namespace dlp {
namespace core {

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

const int UNDEFINED = -1;
const int INF = std::numeric_limits<int>::max();

}
}

#endif
