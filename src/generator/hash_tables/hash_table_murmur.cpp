#include "hash_table_murmur.h"

#include <vector>
#include <iostream>

#include "../../utils/MurmurHash3.h"


namespace dlplan::generator {

static std::array<uint32_t, 4> compute_hash(const std::vector<int>& denotation) {
    std::array<uint32_t, 4> a;
    MurmurHash3_x86_128(denotation.begin().base(), sizeof(int) * denotation.size(), denotation.size(), a.begin());
    return a;
}

bool HashTableMurmur::insert_concept_impl(const std::vector<int>& denotation) {
    return m_concept_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableMurmur::insert_role_impl(const std::vector<int>& denotation) {
    return m_role_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableMurmur::insert_numerical_impl(const std::vector<int>& denotation) {
    return m_numerical_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableMurmur::insert_boolean_impl(const std::vector<bool>& denotation) {
    return m_boolean_denotation_cache.insert(denotation).second;
}

}