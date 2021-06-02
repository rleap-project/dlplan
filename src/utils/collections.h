#ifndef DLP_SRC_UTILS_COLLECTIONS_H_
#define DLP_SRC_UTILS_COLLECTIONS_H_

#include <vector>


namespace dlp {
namespace utils {

template<typename T, class C>
bool in_bounds(T index, const C &container) {
    return index >= 0 && index < container.size();
}

}
}

#endif
