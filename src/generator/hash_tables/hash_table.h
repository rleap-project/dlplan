#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_H_

#include <vector>
#include <array>

#include "utils.h"

#include "../types.h"


namespace std {
    /**
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
    template<> struct hash<std::array<uint8_t, 32>> {
        std::size_t operator()(const std::array<uint8_t, 32>& h) const noexcept {
            std::size_t seed = 32;
            for (int i = 0; i < 32; ++i) {
                dlplan::generator::hash_combine(seed, h[i]);
            }
            return seed;
        }
    };
    template<> struct hash<std::array<uint32_t, 4>> {
        std::size_t operator()(const std::array<uint32_t, 4>& h) const noexcept {
            std::size_t seed = 4;
            for (int i = 0; i < 4; ++i) {
                dlplan::generator::hash_combine(seed, h[i]);
            }
            return seed;
        }
    };
}


namespace dlplan::generator {

/**
 * ElementHashTable provides an interface for storing elements
 * while trying to maintain uniqueness of their evaluations.
 */
class HashTable {
protected:
    int m_cache_hits;
    int m_cache_misses;

protected:
    virtual bool insert_concept_impl(const std::vector<int>& denotation) = 0;

    virtual bool insert_role_impl(const std::vector<int>& denotation) = 0;

    virtual bool insert_numerical_impl(const std::vector<int>& denotation) = 0;

public:
    HashTable() : m_cache_hits(0), m_cache_misses(0) { }
    virtual ~HashTable() = default;

    bool insert_concept(const std::vector<int>& denotation);

    bool insert_role(const std::vector<int>& denotation);

    bool insert_numerical(const std::vector<int>& denotation);

    /** We cast boolean denoation to numerical and store it in a single hash map
      * because Booleans are a special case of Numericals.
      */
    bool insert_boolean(const std::vector<bool>& denotation);

    /**
     * Getters.
     */
    int get_cache_hits() const;
    int get_cache_misses() const;
};

}

#endif
