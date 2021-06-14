#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_LOSSLESS_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_LOSSLESS_H_

#include <unordered_set>
#include <vector>

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
    template<> struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for (const auto& i : denotation) {
                dlplan::generator::hash_combine(seed, i);
            }
            return seed;
        }
    };
}


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
    std::unordered_set<std::vector<bool>> m_boolean_denotation_cache;

protected:
    bool insert_concept_impl(const std::vector<int>& denotation) override;

    bool insert_role_impl(const std::vector<int>& denotation) override;

    bool insert_numerical_impl(const std::vector<int>& denotation) override;

    bool insert_boolean_impl(const std::vector<bool>& denotation) override;

public:
    ~HashTableLossLess() = default;
};

}

#endif
