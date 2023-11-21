#include "../../include/dlplan/core.h"

#include "../../include/dlplan/utils/hash.h"


namespace dlplan::core {

DenotationsCaches::DenotationsCaches() = default;

DenotationsCaches::~DenotationsCaches() = default;

DenotationsCaches::DenotationsCaches(DenotationsCaches&& other) = default;

DenotationsCaches& DenotationsCaches::operator=(DenotationsCaches&& other) = default;

bool DenotationsCaches::Key::operator==(const Key& other) const {
    return (element == other.element) &&
           (instance == other.instance) &&
           (state == other.state);
}

bool DenotationsCaches::Key::operator!=(const Key& other) const {
    return !(*this == other);
}


std::size_t DenotationsCaches::KeyHash::operator()(const Key& key) const {
    std::size_t seed = key.element;
    hash_combine(seed, key.instance);
    hash_combine(seed, key.state);
    return seed;
}

}
