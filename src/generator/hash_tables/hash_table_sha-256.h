#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLE_SHA_2_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLE_SHA_2_H_

#include <unordered_set>
#include <vector>
#include <array>

#include "hash_table.h"
#include "utils.h"


namespace std {
    /**
     * For combining hash value we use the boost::hash_combine one-liner.
     * https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
     *
     * We provide custom specialization of std::hash that are injected in the namespace std.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<std::array<uint8_t, 32>> {
        std::size_t operator()(const std::array<uint8_t, 32>& h) const noexcept {
            std::size_t seed = 32;
            for (int i = 0; i < 32; ++i) {
                dlplan::generator::hash_combine(seed, h[i]);
            }
            return seed;
        }
    };
}


namespace dlplan::generator {

class HashTableSha2 : public HashTable {
private:
    // TODO(dominik): Should combine the caches?
    std::unordered_set<std::array<uint8_t, 32>> m_concept_denotation_cache;
    std::unordered_set<std::array<uint8_t, 32>> m_role_denotation_cache;
    std::unordered_set<std::array<uint8_t, 32>> m_numerical_denotation_cache;
    std::unordered_set<std::vector<bool>> m_boolean_denotation_cache;

protected:
    bool insert_concept_impl(const std::vector<int>& denotation) override;

    bool insert_role_impl(const std::vector<int>& denotation) override;

    bool insert_numerical_impl(const std::vector<int>& denotation) override;

    bool insert_boolean_impl(const std::vector<bool>& denotation) override;
};

}

#endif
