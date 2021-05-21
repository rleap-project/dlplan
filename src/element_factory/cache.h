#ifndef DLP_SRC_ELEMENT_FACTORY_CACHE_H_
#define DLP_SRC_ELEMENT_FACTORY_CACHE_H_

#include <unordered_map>

#include "boost/functional/hash.hpp"


namespace dlp {

/**
 * A simple cache.
 */
template<typename Key_T, typename Value_T>
class Cache {
protected:
    struct KeyHash {
        std::size_t operator()(const Key_T &key) const {
            return boost::hash_value(key);
        }
    };
public:
    Cache() = default;
    virtual ~Cache() = default;

    bool exists(const Key_T &key) const {
        if (m_cache.find(key) != m_cache.end()) {
            return true;
        }
        return false;
    }

    Value_T& get(const Key_T &key) {
        return m_cache.find(key)->second;
    }

    void insert(const Key_T &key, Value_T &&value) {
        m_cache.insert(make_pair(key, std::move(value)));
    }
private:
    std::unordered_map<Key_T, Value_T, KeyHash> m_cache;
};


}

#endif