#ifndef DLPLAN_SRC_CORE_CACHE_H_
#define DLPLAN_SRC_CORE_CACHE_H_

#include "../../include/dlplan/core.h"
#include "../utils/cache.h"

#include <memory>


namespace dlplan::core {

/**
 * One cache for each template instantiated element.
 */
class Caches {
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Concept>> m_concept_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Role>> m_role_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Numerical>> m_numerical_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Boolean>> m_boolean_cache;

public:
    Caches()
        : m_concept_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Concept>>()),
          m_role_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Role>>()),
          m_numerical_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Numerical>>()),
          m_boolean_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Boolean>>()) { }

    std::shared_ptr<const Concept> insert(std::unique_ptr<Concept>&& concept) {
        return m_concept_cache->insert(std::move(concept)).first;
    }

    std::shared_ptr<const Role> insert(std::unique_ptr<Role>&& role) {
        return m_role_cache->insert(std::move(role)).first;
    }

    std::shared_ptr<const Boolean> insert(std::unique_ptr<Boolean>&& boolean) {
        return m_boolean_cache->insert(std::move(boolean)).first;
    }

    std::shared_ptr<const Numerical> insert(std::unique_ptr<Numerical>&& numerical) {
        return m_numerical_cache->insert(std::move(numerical)).first;
    }
};


}

#endif