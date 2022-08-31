#ifndef GP_SRC_CACHE_H_
#define GP_SRC_CACHE_H_

#include <unordered_map>
#include <memory>
#include <iostream>
#include <cassert>
#include <mutex>

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

/**
 * One cache for each template instantiated element.
 */
struct Caches {
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, core::Boolean>> m_boolean_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, core::Numerical>> m_numerical_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, BaseCondition>> m_condition_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, BaseEffect>> m_effect_cache;
    std::shared_ptr<utils::ReferenceCountedObjectCache<std::string, Rule>> m_rule_cache;

    Caches()
        : m_boolean_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, core::Boolean>>()),
          m_numerical_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, core::Numerical>>()),
          m_condition_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, BaseCondition>>()),
          m_effect_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, BaseEffect>>()),
          m_rule_cache(std::make_shared<utils::ReferenceCountedObjectCache<std::string, Rule>>()) { }
};


}

#endif