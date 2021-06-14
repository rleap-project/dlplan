#include "hash_table_lossless.h"


namespace dlplan::generator {

bool HashTableLossLess::insert_concept_impl(const std::vector<int>& denotation) {
    return m_concept_denotation_cache.insert(denotation).second;
}

bool HashTableLossLess::insert_role_impl(const std::vector<int>& denotation) {
    return m_role_denotation_cache.insert(denotation).second;
}

bool HashTableLossLess::insert_numerical_impl(const std::vector<int>& denotation) {
    return m_numerical_denotation_cache.insert(denotation).second;
}

bool HashTableLossLess::insert_boolean_impl(const std::vector<bool>& denotation) {
    return m_boolean_denotation_cache.insert(denotation).second;
}

}
