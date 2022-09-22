#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <mutex>
#include <cassert>
#include <memory>

#include "types.h"

#include "../../utils/hash_utils.h"

#include "../../../include/dlplan/utils/cache.h"
#include "../../../include/dlplan/core.h"


using namespace std::string_literals;

namespace dlplan::core::element {
class DenotationsCaches;

template<typename T>
using DENOTS = std::vector<T>;

template<typename T>
class Element : public utils::Cachable {
protected:
    /**
     * Index can be used for internal caching.
     */
    int m_index;

public:
    // Elements are not copieable because they must live in the cache.
    explicit Element(const VocabularyInfo&) : m_index(-1) { }
    Element(const Element& other) = delete;
    Element& operator=(const Element& other) = delete;
    Element(Element&& other) = delete;
    Element& operator=(Element&& other) = delete;
    virtual ~Element() = default;

    virtual int compute_complexity() const = 0;

    virtual void compute_repr(std::stringstream& out) const = 0;

    void set_index(int index) {
        m_index = index;
    }

    virtual std::string compute_repr() const {
        std::stringstream ss;
        compute_repr(ss);
        return ss.str();
    }

    int get_index() const {
        return m_index;
    }
};



template<typename T>
class PointerDenotationCache {
private:
    using KEY=std::unique_ptr<T>;
    using RAW_KEY=T*;

    struct PointerDenotationEqual {
        bool operator()(const KEY& l, const KEY& r) const {
            return *l == *r;
        }
    };

    struct PointerDenotationHasher {
        std::size_t operator()(const KEY& denotation) const {
            return std::hash<KEY>()(denotation);
        }
    };
private:
   std::unordered_set<std::unique_ptr<T>, PointerDenotationHasher, PointerDenotationEqual> m_storage;
public:
    PointerDenotationCache() { }
    PointerDenotationCache(const PointerDenotationCache& other) = delete;
    PointerDenotationCache& operator=(const PointerDenotationCache& other) = delete;
    PointerDenotationCache(PointerDenotationCache&& other) = default;
    PointerDenotationCache& operator=(PointerDenotationCache&&) = default;
    ~PointerDenotationCache() { }

    KEY get_new_denotation(int num_objects) const {
        return std::make_unique<T>(T(num_objects));
    }

    /**
     * Uniquely inserts a denotation and returns a reference to it.
     * Second alternative also creates mapping (instance, state, element) -> denotation*
     */
    RAW_KEY insert(KEY&& denotation) {
        return m_storage.insert(std::move(denotation)).first->get();
    }
};




template<typename T>
class PointerDenotationsCache {
private:
    using KEY=std::unique_ptr<DENOTS<T*>>;
    using RAW_KEY=DENOTS<T*>*;

    struct PointerDenotationsEqual {
        bool operator()(const KEY& l, const KEY& r) const {
            return *l == *r;
        }
    };
    struct PointerDenotationsHasher {
        std::size_t operator()(const KEY& denotations) const {
            return std::hash<KEY>()(denotations);
        }
    };

private:
    std::unordered_set<KEY, PointerDenotationsHasher, PointerDenotationsEqual> m_storage;

    // mapping from (element) -> std::vector<T*>*
    std::unordered_map<int, RAW_KEY> m_mapping;

    int m_num_states;

public:
    explicit PointerDenotationsCache(int num_states) : m_num_states(num_states) {}
    PointerDenotationsCache(const PointerDenotationsCache& other) = delete;
    PointerDenotationsCache& operator=(const PointerDenotationsCache& other) = delete;
    PointerDenotationsCache(PointerDenotationsCache&& other) = default;
    PointerDenotationsCache& operator=(PointerDenotationsCache&& other) = default;
    ~PointerDenotationsCache() { }

    KEY get_new_denotations() const {
        auto result = std::make_unique<std::vector<T*>>();
        result->reserve(m_num_states);
        return result;
    }

    RAW_KEY insert(KEY&& denotations, int element_index) {
        auto result = m_storage.insert(std::move(denotations));
        if (result.second) {
            m_mapping.insert(std::make_pair(element_index, result.first->get()));
        }
        return result.first->get();
    }

    RAW_KEY find(int element_index) const {
        auto result = m_mapping.find(element_index);
        if (result == m_mapping.end())
            return nullptr;
        return result->second;
    }
};


template<typename T>
class ValueDenotationsCache {
private:
    using KEY=std::unique_ptr<DENOTS<T>>;
    using RAW_KEY=DENOTS<T>*;

    struct ValueDenotationsEqual {
        bool operator()(const KEY& l, const KEY& r) const {
            return *l == *r;
        }
    };
    struct ValueDenotationsHasher {
        std::size_t operator()(const KEY& denotations) const {
            return std::hash<KEY>()(denotations);
        }
    };

private:
    std::unordered_set<KEY, ValueDenotationsHasher, ValueDenotationsEqual> m_storage;

    // mapping from (element) -> std::vector<T*>*
    std::unordered_map<int, DENOTS<T>*> m_mapping;

    int m_num_states;
public:
    explicit ValueDenotationsCache(int num_states) : m_num_states(num_states) {}
    ValueDenotationsCache(const ValueDenotationsCache& other) = delete;
    ValueDenotationsCache& operator=(const ValueDenotationsCache& other) = delete;
    ValueDenotationsCache(ValueDenotationsCache&& other) = default;
    ValueDenotationsCache& operator=(ValueDenotationsCache&& other) = default;
    ~ValueDenotationsCache() { }

    KEY get_new_denotations() const {
        auto result = std::make_unique<std::vector<T>>();
        result->reserve(m_num_states);
        return result;
    }

    RAW_KEY insert(KEY&& denotations, int element_index) {
        auto result = m_storage.insert(std::move(denotations));
        if (result.second) {
            m_mapping.insert(std::make_pair(element_index, result.first->get()));
        }
        return result.first->get();
    }

    RAW_KEY find(int element_index) const {
        auto result = m_mapping.find(element_index);
        if (result == m_mapping.end())
            return nullptr;
        return result->second;
    }
};


struct DenotationsCaches {
    // Cache for single denotations.
    PointerDenotationCache<ConceptDenotation> m_c_denot_cache;
    PointerDenotationCache<RoleDenotation> m_r_denot_cache;
    // Cache for collections of denotations.
    ValueDenotationsCache<bool> m_b_denots_cache;
    ValueDenotationsCache<int> m_n_denots_cache;
    PointerDenotationsCache<ConceptDenotation> m_c_denots_cache;
    PointerDenotationsCache<RoleDenotation> m_r_denots_cache;

    DenotationsCaches(int num_states)
    : m_b_denots_cache(num_states),
      m_n_denots_cache(num_states),
      m_c_denots_cache(num_states),
      m_r_denots_cache(num_states) { }
};

}


namespace std {
    template<>
    struct hash<unique_ptr<dlplan::core::ConceptDenotation>> {
        size_t operator()(const unique_ptr<dlplan::core::ConceptDenotation>& denotation) const noexcept {
            return denotation->compute_hash();
        }
    };
    template<>
    struct hash<dlplan::core::RoleDenotation> {
        size_t operator()(const unique_ptr<dlplan::core::RoleDenotation>& denotation) const noexcept {
            return denotation->compute_hash();
        }
    };
    template<>
    struct hash<unique_ptr<vector<dlplan::core::ConceptDenotation*>>> {
        size_t operator()(const unique_ptr<vector<dlplan::core::ConceptDenotation*>>& denotations) const noexcept {
            size_t seed = 0;
            for (const auto denot_ptr : *denotations) {
                dlplan::utils::hashing::hash_combine(seed, denot_ptr);
            }
            return seed;
        }
    };
    template<>
    struct hash<unique_ptr<vector<dlplan::core::RoleDenotation*>>> {
        size_t operator()(const unique_ptr<vector<dlplan::core::RoleDenotation*>>& denotations) const noexcept {
            size_t seed = 0;
            for (const auto denot_ptr : *denotations) {
                dlplan::utils::hashing::hash_combine(seed, denot_ptr);
            }
            return seed;
        }
    };
    template<>
    struct hash<unique_ptr<vector<bool>>> {
        size_t operator()(const unique_ptr<vector<bool>>& denotations) const noexcept {
            return hash<vector<bool>>()(*denotations);
        }
    };
    template<>
    struct hash<unique_ptr<vector<int>>> {
        size_t operator()(const unique_ptr<vector<int>>& denotations) const noexcept {
            size_t seed = 0;
            for (const int denot : *denotations) {
                dlplan::utils::hashing::hash_combine(seed, denot);
            }
            return seed;
        }
    };
}

#endif
