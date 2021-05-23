#include "cache.h"

namespace dlp {
namespace core {

Cache<std::string, ConceptElement_Ptr>& ElementCache::concept_element_cache() {
    return m_concept_element_cache;
}

Cache<std::string, RoleElement_Ptr>& ElementCache::role_element_cache() {
    return m_role_element_cache;
}

Cache<std::string, NumericalElement_Ptr>& ElementCache::numerical_element_cache() {
    return m_numerical_element_cache;
}

Cache<std::string, BooleanElement_Ptr>& ElementCache::boolean_element_cache() {
    return m_boolean_element_cache;
}

}
}
