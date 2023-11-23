#include "../../include/dlplan/core.h"

#include "../../include/dlplan/utils/hash.h"


namespace dlplan::core {

DenotationsCaches::DenotationsCaches() = default;

DenotationsCaches::~DenotationsCaches() = default;

DenotationsCaches::DenotationsCaches(DenotationsCaches&& other) = default;

DenotationsCaches& DenotationsCaches::operator=(DenotationsCaches&& other) = default;

bool DenotationsCacheKey::operator==(const DenotationsCacheKey& other) const {
    return (element == other.element) &&
           (instance == other.instance) &&
           (state == other.state);
}

size_t DenotationsCacheKey::hash() const {
    return hash_combine(element, instance, state);
}

}
