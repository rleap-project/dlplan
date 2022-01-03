#include "hash_table.h"


namespace dlplan::generator {

bool HashTable::insert(std::array<u_int32_t, 4>&& hash) {
    std::lock_guard<std::mutex> hold(m_mutex);
    if (m_hash_cache.insert(std::move(hash)).second) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

int HashTable::get_cache_hits() const {
    std::lock_guard<std::mutex> hold(m_mutex);
    return m_cache_hits;
}

int HashTable::get_cache_misses() const {
    std::lock_guard<std::mutex> hold(m_mutex);
    return m_cache_misses;
}

}
