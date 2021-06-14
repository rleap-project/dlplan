#include "hash_table.h"


namespace dlplan::generator {


bool HashTable::insert_concept(const std::vector<int>& denotation) {
    if (insert_concept_impl(denotation)) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

bool HashTable::insert_role(const std::vector<int>& denotation) {
    if (insert_role_impl(denotation)) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

bool HashTable::insert_numerical(const std::vector<int>& denotation) {
    if (insert_numerical_impl(denotation)) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

bool HashTable::insert_boolean(const std::vector<bool>& denotation) {
    if (insert_boolean_impl(denotation)) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

int HashTable::get_cache_hits() const { return m_cache_hits; }

int HashTable::get_cache_misses() const { return m_cache_misses; }

}
