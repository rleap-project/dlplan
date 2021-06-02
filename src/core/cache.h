#ifndef DLP_SRC_CORE_CACHE_H_
#define DLP_SRC_CORE_CACHE_H_

#include <unordered_map>
#include <iostream>

//#include "boost/functional/hash.hpp"

#include "elements/types.h"


namespace dlp {
namespace core {

/* A simple cache for more complex keys.

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

    bool exists(const Key_T& key) {
        if (m_cache.find(key) != m_cache.end()) {
            return true;
        }
        return false;
    }

    Value_T& get(const Key_T& key) {
        return m_cache.find(key)->second;
    }

    std::pair<typename std::unordered_map<Key_T, Value_T>::iterator, bool> insert(std::pair<Key_T, Value_T>&& pair) {
        return m_cache.insert(std::move(pair));
    }

private:
    std::unordered_map<Key_T, Value_T, KeyHash> m_cache;
};
*/


class ElementCache {
private:
    /**
     * One cache for each template instantiated element.
     */
    std::unordered_map<std::string, element::Concept_Ptr> m_concept_element_cache;
    std::unordered_map<std::string, element::Role_Ptr> m_role_element_cache;
    std::unordered_map<std::string, element::Numerical_Ptr> m_numerical_element_cache;
    std::unordered_map<std::string, element::Boolean_Ptr> m_boolean_element_cache;

public:
    ElementCache();

    std::unordered_map<std::string, element::Concept_Ptr>& concept_element_cache();
    std::unordered_map<std::string, element::Role_Ptr>& role_element_cache();
    std::unordered_map<std::string, element::Numerical_Ptr>& numerical_element_cache();
    std::unordered_map<std::string, element::Boolean_Ptr>& boolean_element_cache();
};


}
}

#endif