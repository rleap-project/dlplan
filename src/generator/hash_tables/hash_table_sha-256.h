#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLE_SHA_2_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLE_SHA_2_H_

#include <unordered_set>
#include <vector>
#include <array>

#include "hash_table.h"


namespace dlplan::generator {

class HashTableSha256 : public HashTable {
private:
    std::unordered_set<std::array<uint8_t, 32>> m_concept_denotation_cache;
    std::unordered_set<std::array<uint8_t, 32>> m_role_denotation_cache;
    std::unordered_set<std::array<uint8_t, 32>> m_numerical_denotation_cache;

protected:
    bool insert_concept_impl(const std::vector<int>& denotation) override;

    bool insert_role_impl(const std::vector<int>& denotation) override;

    bool insert_numerical_impl(const std::vector<int>& denotation) override;
};

}

#endif
