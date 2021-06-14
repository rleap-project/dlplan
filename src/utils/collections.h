#ifndef DLPLAN_SRC_UTILS_COLLECTIONS_H_
#define DLPLAN_SRC_UTILS_COLLECTIONS_H_

/**
 * Taken from fast-downward.org
 */

#include <vector>


namespace dlplan::utils {

template<class T>
bool in_bounds(int index, const T &container) {
    return index >= 0 && static_cast<size_t>(index) < container.size();
}

template<class T>
bool in_bounds(long index, const T &container) {
    return index >= 0 && static_cast<size_t>(index) < container.size();
}

template<class T>
bool in_bounds(size_t index, const T &container) {
    return index < container.size();
}

}

#endif
