#include "../../include/dlplan/utils/hash.h"


namespace dlplan::utils {
size_t hash_impl<std::vector<unsigned>>::operator()(const std::vector<unsigned>& data) const {
    size_t seed = data.size();
    for (unsigned value : data) {
        dlplan::utils::hash_combine(seed, value);
    }
    return seed;
}
size_t hash_impl<std::vector<int>>::operator()(const std::vector<int>& data) const {
    size_t seed = data.size();
    for (int value : data) {
        dlplan::utils::hash_combine(seed, value);
    }
    return seed;
}


}