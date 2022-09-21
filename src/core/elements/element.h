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
    template<typename T1>
    struct PointerDenotationEqual {
    public:
        bool operator()(const std::unique_ptr<T1>& l, const std::unique_ptr<T1>& r) const {
            return *l == *r;
        }
    };


    template<typename T1>
    struct PointerDenotationHasher {
    public:
        std::size_t operator()(const std::unique_ptr<T1>& denotation) const {
            return denotation->compute_hash();
        }
    };
private:
   std::unordered_set<std::unique_ptr<T>, PointerDenotationHasher<T>, PointerDenotationEqual<T>> m_storage;
public:
    PointerDenotationCache() { }
    PointerDenotationCache(const PointerDenotationCache& other) = delete;
    PointerDenotationCache& operator=(const PointerDenotationCache& other) = delete;
    PointerDenotationCache(PointerDenotationCache&& other) = default;
    PointerDenotationCache& operator=(PointerDenotationCache&&) = default;
    ~PointerDenotationCache() { }

    std::unique_ptr<T> get_new_denotation(int num_objects) const {
        return std::make_unique<T>(T(num_objects));
    }

    /**
     * Uniquely inserts a denotation and returns a reference to it.
     * Second alternative also creates mapping (instance, state, element) -> denotation*
     */
    T* insert(std::unique_ptr<T>&& denotation) {
        return m_storage.insert(std::move(denotation)).first->get();
    }
};




template<typename T>
class PointerDenotationsCache {
private:
    template<typename T1>
    struct PointerDenotationsEqual {
        bool operator()(const std::unique_ptr<T1>& l, const std::unique_ptr<T1>& r) const {
            return *l == *r;
        }
    };

    template<typename T1>
    struct PointerDenotationsHasher {
        std::size_t operator()(const std::unique_ptr<T1>& denotations) const {
            return std::hash<T1>()(*denotations);
        }
    };

private:
    std::unordered_set<std::unique_ptr<DENOTS<T*>>, PointerDenotationsHasher<DENOTS<T*>>, PointerDenotationsEqual<DENOTS<T*>>> m_storage;

    // mapping from (element) -> std::vector<T*>*
    std::unordered_map<int, DENOTS<T*>*> m_mapping;

    int m_num_states;

public:
    explicit PointerDenotationsCache(int num_states) : m_num_states(num_states) {}
    PointerDenotationsCache(const PointerDenotationsCache& other) = delete;
    PointerDenotationsCache& operator=(const PointerDenotationsCache& other) = delete;
    PointerDenotationsCache(PointerDenotationsCache&& other) = default;
    PointerDenotationsCache& operator=(PointerDenotationsCache&& other) = default;
    ~PointerDenotationsCache() { }

    std::unique_ptr<DENOTS<T*>> get_new_denotations() const {
        auto result = std::make_unique<std::vector<T*>>();
        result->reserve(m_num_states);
        return result;
    }

    DENOTS<T*>* insert(std::unique_ptr<DENOTS<T*>>&& denotations, int element_index) {
        auto result = m_storage.insert(std::move(denotations));
        if (result.second) {
            m_mapping.insert(std::make_pair(element_index, result.first->get()));
        }
        return result.first->get();
    }

    DENOTS<T*>* find(int element_index) const {
        auto result = m_mapping.find(element_index);
        if (result == m_mapping.end())
            return nullptr;
        return result->second;
    }
};


template<typename T>
class ValueDenotationsCache {
private:
    template<typename T1>
    struct ValueDenotationsEqual {
    public:
        bool operator()(const std::unique_ptr<T1>& l, const std::unique_ptr<T1>& r) const {
            return *l == *r;
        }
    };


    template<typename T1>
    struct ValueDenotationsHasher {
    public:
        std::size_t operator()(const std::unique_ptr<T1>& denotations) const {
            return std::hash<T1>()(*denotations);
        }
    };
private:
    std::unordered_set<std::unique_ptr<DENOTS<T>>, ValueDenotationsHasher<DENOTS<T>>, ValueDenotationsEqual<DENOTS<T>>> m_storage;

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

    std::unique_ptr<DENOTS<T>> get_new_denotations() const {
        auto result = std::make_unique<std::vector<T>>();
        result->reserve(m_num_states);
        return result;
    }

    DENOTS<T>* insert(std::unique_ptr<DENOTS<T>>&& denotations, int element_index) {
        auto result = m_storage.insert(std::move(denotations));
        if (result.second) {
            m_mapping.insert(std::make_pair(element_index, result.first->get()));
        }
        return result.first->get();
    }

    DENOTS<T>* find(int element_index) const {
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
    /**
     * We provide custom specialization of std::hash that are injected in the namespace std
     * to be able to use standard hash containers.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<dlplan::core::ConceptDenotation> {
        std::size_t operator()(const dlplan::core::ConceptDenotation& denotation) const noexcept {
            return denotation.compute_hash();
        }
    };
    template<> struct hash<dlplan::core::RoleDenotation> {
        std::size_t operator()(const dlplan::core::RoleDenotation& denotation) const noexcept {
            return denotation.compute_hash();
        }
    };
    template<> struct hash<std::vector<dlplan::core::ConceptDenotation*>> {
        std::size_t operator()(const std::vector<dlplan::core::ConceptDenotation*>& denotations) const noexcept {
            auto denot_hasher = std::hash<dlplan::core::ConceptDenotation>();
            std::size_t seed = 0;
            for (const auto denot_ptr : denotations) {
                dlplan::utils::hashing::hash_combine(seed, denot_hasher(*denot_ptr));
            }
            return seed;
        }
    };
    template<> struct hash<std::vector<dlplan::core::RoleDenotation*>> {
        std::size_t operator()(const std::vector<dlplan::core::RoleDenotation*>& denotations) const noexcept {
            auto denot_hasher = std::hash<dlplan::core::RoleDenotation>();
            std::size_t seed = 0;
            for (const auto denot_ptr : denotations) {
                dlplan::utils::hashing::hash_combine(seed, denot_hasher(*denot_ptr));
            }
            return seed;
        }
    };
}

#endif
