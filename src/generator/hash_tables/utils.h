#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLES_UTILS_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLES_UTILS_H_


namespace dlplan::generator {

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}


}

#endif
