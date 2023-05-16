#ifndef DLPLAN_SRC_POLICY_CACHE_H_
#define DLPLAN_SRC_POLICY_CACHE_H_

#include "../../include/dlplan/policy.h"

#include <memory>


namespace dlplan::policy {
/**
 * One cache for each template instantiated element.
 */
struct Caches {
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>> m_condition_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>> m_effect_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>> m_rule_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>> m_policy_cache;

    Caches()
        : m_condition_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>>()),
          m_effect_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>>()),
          m_rule_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>>()),
          m_policy_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>>()) { }
};


}

#endif