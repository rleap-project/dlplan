#ifndef DLPLAN_SRC_UTILS_MEMORY_H
#define DLPLAN_SRC_UTILS_MEMORY_H


namespace dlplan::utils {

/**
 * Uses swap-trick to free memory of container.
 */
template<typename CONTAINER_TYPE>
void free_memory(CONTAINER_TYPE& container) {
    CONTAINER_TYPE().swap(container);
}

}

#endif
