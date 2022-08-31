#ifndef DLPLAN_SRC_CORE_CACHE_H_
#define DLPLAN_SRC_CORE_CACHE_H_

#include <unordered_map>
#include <memory>
#include <iostream>
#include <cassert>
#include <mutex>

#include "../../include/dlplan/core.h"


namespace dlplan::core {

/**
 * One cache for each template instantiated element.
 */
struct Caches {
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, element::Concept>> m_concept_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, element::Role>> m_role_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, element::Numerical>> m_numerical_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, element::Boolean>> m_boolean_cache;

    Caches()
        : m_concept_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, element::Concept>>()),
          m_role_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, element::Role>>()),
          m_numerical_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, element::Numerical>>()),
          m_boolean_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, element::Boolean>>()) { }
};


}

#endif