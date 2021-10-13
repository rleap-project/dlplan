#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_LOSSLESS_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_LOSSLESS_H_

#include <unordered_set>
#include <vector>

#include "hash_table.h"


namespace dlplan::generator {

/**
 * In this version, we do not compress the denotations.
 * TODO(dominik): Compress the denotations.
 */
class HashTableLossLess : public HashTable {
private:
    std::unordered_set<std::vector<int>> m_concept_denotation_cache;
    std::unordered_set<std::vector<int>> m_role_denotation_cache;
    std::unordered_set<std::vector<int>> m_numerical_denotation_cache;

protected:
    bool insert_concept_impl(const std::vector<int>& denotation) override;

    bool insert_role_impl(const std::vector<int>& denotation) override;

    bool insert_numerical_impl(const std::vector<int>& denotation) override;

public:
    ~HashTableLossLess() = default;
};

}

#endif
