#ifndef DLPLAN_SRC_POLICY_CACHE_H_
#define DLPLAN_SRC_POLICY_CACHE_H_

#include "../../include/dlplan/policy.h"
#include "../utils/cache.h"

#include <memory>


namespace dlplan::policy {
/**
 * One cache for each template instantiated element.
 */
class Caches {
private:
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>> m_condition_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>> m_effect_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>> m_rule_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>> m_policy_cache;

public:
    Caches()
        : m_condition_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>>()),
          m_effect_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>>()),
          m_rule_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>>()),
          m_policy_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>>()) { }

    std::shared_ptr<const BaseCondition> insert(std::unique_ptr<BaseCondition>&& condition) {
        return m_condition_cache->insert(std::move(condition)).first;
    }

    std::shared_ptr<const BaseEffect> insert(std::unique_ptr<BaseEffect>&& effect) {
        return m_effect_cache->insert(std::move(effect)).first;
    }

    std::shared_ptr<const Rule> insert(std::unique_ptr<Rule>&& rule) {
        return m_rule_cache->insert(std::move(rule)).first;
    }

    std::shared_ptr<const Policy> insert(std::unique_ptr<Policy>&& policy) {
        return m_policy_cache->insert(std::move(policy)).first;
    }
};


}

#endif