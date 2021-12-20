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

static std::vector<int> bitset_to_num_vec(const std::vector<dynamic_bitset::DynamicBitset<unsigned>>& bitsets) {
    static_assert(sizeof(int) == sizeof(unsigned));
    size_t size = 0;
    for (const auto& b : bitsets) {
        size += b.get_blocks().size();
    }
    std::vector<int> result;
    result.reserve(size);
    for (const auto& b : bitsets) {
        result.insert(result.end(), b.get_blocks().begin(), b.get_blocks().end());
    }
    return result;
}


bool HashTable::insert_concept(const std::vector<core::ConceptDenotation>& denotation) {
    if (insert_concept_impl(bitset_to_num_vec(denotation))) {
        ++m_cache_misses;
        return true;
    } else {
        ++m_cache_hits;
        return false;
    }
}

bool HashTable::insert_role(const std::vector<core::RoleDenotation>& denotation) {
    if (insert_role_impl(bitset_to_num_vec(denotation))) {
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
