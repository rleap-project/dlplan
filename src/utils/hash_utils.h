#ifndef DLPLAN_SRC_UTILS_HASH_UTILS_H_
#define DLPLAN_SRC_UTILS_HASH_UTILS_H_

#include <vector>
#include <array>

#include "MurmurHash3.h"


namespace dlplan::utils::hashing {

/**
 * For combining hash value we use the boost::hash_combine one-liner.
 * https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
 */
template <class T>
void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

}


namespace std {
    /**
     * We provide custom specialization of std::hash that are injected in the namespace std.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for (const auto& i : denotation) {
                dlplan::utils::hashing::hash_combine(seed, i);
            }
            return seed;
        }
    };
    template<> struct hash<std::array<uint32_t, 4>> {
        std::size_t operator()(const std::array<uint32_t, 4>& h) const noexcept {
            std::size_t seed = 4;
            for (int i = 0; i < 4; ++i) {
                dlplan::utils::hashing::hash_combine(seed, h[i]);
            }
            return seed;
        }
    };
}

#endif
