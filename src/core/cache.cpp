#include "cache.h"

namespace dlp {
namespace core {

ElementCache::ElementCache() { }

std::unordered_map<std::string, element::ConceptElement_Ptr>& ElementCache::concept_element_cache() {
    return m_concept_element_cache;
}

std::unordered_map<std::string, element::RoleElement_Ptr>& ElementCache::role_element_cache() {
    return m_role_element_cache;
}

std::unordered_map<std::string, element::NumericalElement_Ptr>& ElementCache::numerical_element_cache() {
    return m_numerical_element_cache;
}

std::unordered_map<std::string, element::BooleanElement_Ptr>& ElementCache::boolean_element_cache() {
    return m_boolean_element_cache;
}

}
}
