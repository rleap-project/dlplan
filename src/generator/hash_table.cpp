#include "hash_table.h"


namespace dlplan::generator {

bool HashTable::insert(std::array<u_int32_t, 4>&& hash) {
    if (m_hash_cache.insert(std::move(hash)).second) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

int HashTable::get_cache_hits() const {
    return m_cache_hits;
}

int HashTable::get_cache_misses() const {
    return m_cache_misses;
}

}
