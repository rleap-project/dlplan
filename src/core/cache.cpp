#include "cache.h"

namespace dlplan::core {

ElementCache::ElementCache() = default;

std::unordered_map<std::string, element::Concept_Ptr>& ElementCache::concept_element_cache() {
    return m_concept_element_cache;
}

std::unordered_map<std::string, element::Role_Ptr>& ElementCache::role_element_cache() {
    return m_role_element_cache;
}

std::unordered_map<std::string, element::Numerical_Ptr>& ElementCache::numerical_element_cache() {
    return m_numerical_element_cache;
}

std::unordered_map<std::string, element::Boolean_Ptr>& ElementCache::boolean_element_cache() {
    return m_boolean_element_cache;
}

}
