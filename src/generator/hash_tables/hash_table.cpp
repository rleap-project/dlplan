#include "hash_table.h"


namespace dlplan::generator {

static std::vector<int> bool_vec_to_num_vec(const std::vector<bool>& bool_vec) {
    std::vector<int> num_vec;
    num_vec.reserve(bool_vec.size());
    for (size_t i = 0; i < bool_vec.size(); ++i) {
        num_vec.push_back(bool_vec[i]);
    }
    return num_vec;
}


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
    // cast vector<bool> to vector<int>
    if (insert_numerical_impl(bool_vec_to_num_vec(denotation))) {
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
