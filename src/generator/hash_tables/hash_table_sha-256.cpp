#include "hash_table_sha-256.h"

#include <vector>

#include "../../utils/sha-256.h"


namespace dlplan::generator {

static std::array<uint8_t, 32> compute_hash(const std::vector<int>& denotation) {
    std::array<uint8_t, 32> a;
    calc_sha_256(a.begin(), denotation.begin().base(), sizeof(int) * denotation.size());
    return a;
}

bool HashTableSha256::insert_concept_impl(const std::vector<int>& denotation) {
    return m_concept_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableSha256::insert_role_impl(const std::vector<int>& denotation) {
    return m_role_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableSha256::insert_numerical_impl(const std::vector<int>& denotation) {
    return m_numerical_denotation_cache.insert(compute_hash(denotation)).second;
}

bool HashTableSha256::insert_boolean_impl(const std::vector<bool>& denotation) {
    return m_boolean_denotation_cache.insert(denotation).second;
}

}