#ifndef DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_H_
#define DLPLAN_SRC_GENERATOR_HASH_TABLES_HASH_TABLE_H_

#include <vector>

#include "../types.h"


namespace dlplan::core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
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

    virtual bool insert_boolean_impl(const std::vector<bool>& denotation) = 0;

public:
    HashTable() : m_cache_hits(0), m_cache_misses(0) { }
    virtual ~HashTable() = default;

    bool insert_concept(const std::vector<int>& denotation);

    bool insert_role(const std::vector<int>& denotation);

    bool insert_numerical(const std::vector<int>& denotation);

    bool insert_boolean(const std::vector<bool>& denotation);

    /**
     * Getters.
     */
    int get_cache_hits() const;
    int get_cache_misses() const;
};

}

#endif
