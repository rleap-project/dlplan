/// @brief Provides functionality for hashing basic std data types.

#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_HASH_H_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_HASH_H_

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <set>


namespace dlplan::utils {

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

template<>
inline void hash_combine(size_t& seed, const std::size_t& val)
{
    seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
struct hash_impl {
    std::size_t operator()(const T&) const {
        throw std::runtime_error("hash::operator() - not implemented.");
    }
};
template<typename T>
struct hash {
    std::size_t operator()(const T& value) const {
        return hash_impl<typename std::remove_const<T>::type>()(value);
    }
};
template<>
struct hash_impl<std::vector<unsigned>> {
    size_t operator()(const std::vector<unsigned>& data) const;
};
template<>
struct hash_impl<std::vector<int>> {
    size_t operator()(const std::vector<int>& data) const;
};


template<typename... Types>
inline size_t hash_combine(const Types&... args)
{
    size_t seed = 0;
    (hash_combine(seed, args), ...);
    return seed;
}

template<class T>
inline std::size_t hash_vector(const std::vector<T>& vector)
{
    const auto hash_function = std::hash<T>();
    std::size_t aggregated_hash = 0;
    for (const auto& item : vector)
    {
        const auto item_hash = hash_function(item);
        hash_combine(aggregated_hash, item_hash);
    }
    return aggregated_hash;
}

template<class T>
inline std::size_t hash_set(const std::set<T>& vector)
{
    const auto hash_function = std::hash<T>();
    std::size_t aggregated_hash = 0;
    for (const auto& item : vector)
    {
        const auto item_hash = hash_function(item);
        hash_combine(aggregated_hash, item_hash);
    }
    return aggregated_hash;
}

}

#endif