#ifndef DLP_SRC_CORE_CACHE_H_
#define DLP_SRC_CORE_CACHE_H_

#include <unordered_map>

#include "boost/functional/hash.hpp"

#include "elements/types.h"


namespace dlp {
namespace core {

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

    bool exists(const Key_T &key) {
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


class ElementCache {
private:
    /**
     * One cache for each template instantiated element.
     */
    Cache<std::string, element::ConceptElement_Ptr> m_concept_element_cache;
    Cache<std::string, element::RoleElement_Ptr> m_role_element_cache;
    Cache<std::string, element::NumericalElement_Ptr> m_numerical_element_cache;
    Cache<std::string, element::BooleanElement_Ptr> m_boolean_element_cache;

public:
    Cache<std::string, element::ConceptElement_Ptr>& concept_element_cache();
    Cache<std::string, element::RoleElement_Ptr>& role_element_cache();
    Cache<std::string, element::NumericalElement_Ptr>& numerical_element_cache();
    Cache<std::string, element::BooleanElement_Ptr>& boolean_element_cache();
};


}
}

#endif