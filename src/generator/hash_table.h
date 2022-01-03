#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLE_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLE_H_

#include <vector>
#include <array>
#include <mutex>

#include "hash_utils.h"

#include "../../include/dlplan/core.h"


namespace dlplan::generator {

/**
 * ElementHashTable provides an interface for storing elements
 * while trying to maintain uniqueness of their evaluations.
 */
class HashTable {
private:
    int m_cache_hits;
    int m_cache_misses;

    std::unordered_set<std::array<uint32_t, 4>> m_hash_cache;

    mutable std::mutex m_mutex;

public:
    HashTable() : m_cache_hits(0), m_cache_misses(0) { }
    virtual ~HashTable() = default;

    bool insert(std::array<u_int32_t, 4>&& hash);

    /**
     * Getters.
     */
    int get_cache_hits() const;
    int get_cache_misses() const;
};

}

#endif
